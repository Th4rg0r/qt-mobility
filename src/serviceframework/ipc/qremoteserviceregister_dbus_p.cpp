/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qremoteserviceregister_p.h"
#include "qremoteserviceregister_dbus_p.h"
#include "ipcendpoint_p.h"
#include "objectendpoint_dbus_p.h"

#include <QDataStream>
#include <QTimer>


QTM_BEGIN_NAMESPACE

//IPC based on QDBus

#define SERVER 0
#define CLIENT 1

class DBusEndPoint : public QServiceIpcEndPoint
{
    Q_OBJECT

public:
    DBusEndPoint(QDBusInterface* iface, int type, QObject* parent = 0)
        : QServiceIpcEndPoint(parent), interface(iface), endType(type)
    {
        Q_ASSERT(interface);
        interface->setParent(this);
        connect(interface, SIGNAL(packageReceived(QByteArray,int,QString)), this, SLOT(readPackage(QByteArray,int,QString)));
    }

    ~DBusEndPoint() 
    {
    }

protected:
    void flushPackage(const QServicePackage& package)
    {
        QDBusConnection *connection = new QDBusConnection(QDBusConnection::sessionBus());
        if (!connection->isConnected()) {
            qWarning() << "Cannot connect to DBus";
        }
       
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_6);
        out << package;

        packageId = package.d->messageId;
        interface->asyncCall("writePackage", block, endType, packageId);
    }

protected slots:
    void readPackage(const QByteArray &package, int type, const QString &id) {
        // Check that its of a client-server nature
        if (endType != type) {
            // Client to Server
            if (type != SERVER) {
                readIncoming(package);
            } else {
            // Server to Client
                if (id == packageId) {
                    readIncoming(package);
                }
            }
        }
    }

    void readIncoming(const QByteArray &package)
    {
        QDataStream data(package);
        QServicePackage pack;
        data >> pack;
   
        incoming.enqueue(pack);
        emit readyRead();
    }

private:
    QDBusInterface* interface;
    QString packageId;
    int endType;
};

class DBusSessionAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.qtmobility.sfw.DBusSession")

public:
    DBusSessionAdaptor(QObject *parent);
    ~DBusSessionAdaptor() {}

public slots:
    QByteArray writePackage(const QByteArray &package, int type, const QString &id) { 
        QByteArray ret;
        QMetaObject::invokeMethod(parent(), "writePackage",
                                  Q_RETURN_ARG(QByteArray, ret),
                                  Q_ARG(QByteArray, package),
                                  Q_ARG(int, type),
                                  Q_ARG(QString, id));
        return ret;
    }

    bool processIncoming() {
        bool ret;
        QMetaObject::invokeMethod(parent(), "processIncoming",
                                  Q_RETURN_ARG(bool, ret));
        return ret;
    }

    void acceptIncoming(bool accept) {
        QMetaObject::invokeMethod(parent(), "acceptIncoming",
                                  Q_ARG(bool, accept));
    }

    void closeIncoming() {
        QMetaObject::invokeMethod(parent(), "closeIncoming");
    }

signals:
    void packageReceived(const QByteArray &package, int type, const QString &id);
    void newConnection(int pid, int uid);
    void closeConnection();
};

DBusSessionAdaptor::DBusSessionAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

QRemoteServiceRegisterDBusPrivate::QRemoteServiceRegisterDBusPrivate(QObject* parent)
    : QRemoteServiceRegisterPrivate(parent), instanceCount(0)
{
}

QRemoteServiceRegisterDBusPrivate::~QRemoteServiceRegisterDBusPrivate()
{
}

void QRemoteServiceRegisterDBusPrivate::publishServices(const QString& ident)
{
    createServiceEndPoint(ident);
}

/*!
    Creates endpoint on service side.
*/
bool QRemoteServiceRegisterDBusPrivate::createServiceEndPoint(const QString& /*ident*/)
{
    InstanceManager *iManager = InstanceManager::instance();
    QList<QRemoteServiceRegister::Entry> list = iManager->allEntries();
   
    if (list.size() > 0) {
        QDBusConnection *connection = new QDBusConnection(QDBusConnection::sessionBus());
        if (!connection->isConnected()) {
            qWarning() << "Cannot connect to DBus";
            return 0;
        }

        // MAYBE A FOR-LOOP FOR EACH SERVICENAME (ie DBUSExample, IPCExample)

        // TODO: do we want to always re-register services to dbus?
        QString serviceName = "com.nokia.qtmobility.sfw." + list[0].serviceName();
        connection->unregisterService(serviceName);

        if (!connection->registerService(serviceName)) {
            qWarning() << "Cannot register service to DBus";
            return 0;
        }

        // Create and register our DBusSession server/client
        session = new DBusSession();
        new DBusSessionAdaptor(session);
        QObject::connect(session, SIGNAL(newConnection(int,int)), this, SLOT(processIncoming(int,int)));
        QObject::connect(session, SIGNAL(closeConnection()), this, SLOT(processClosing()));

        QString path = "/" + list[0].interfaceName() + "/DBusSession";
        path.replace(QString("."), QString("/"));
        if (!connection->registerObject(path, session)) {
            qWarning() << "Cannot register service session to DBus";
            return 0;
        }

        iface = new QDBusInterface(serviceName, path, "", QDBusConnection::sessionBus());
        if (!iface->isValid()) {
            qWarning() << "Cannot connect to remote service" << serviceName << path;;
            return 0;
        }

        DBusEndPoint* ipcEndPoint = new DBusEndPoint(iface, SERVER);
        ObjectEndPoint* endpoint = new ObjectEndPoint(ObjectEndPoint::Service, ipcEndPoint, this);
        Q_UNUSED(endpoint);
        return true;
    }

    return false;
}

void QRemoteServiceRegisterDBusPrivate::processIncoming(int pid, int uid)
{
    if (getSecurityFilter()) {
        QRemoteServiceRegisterCredentials cred;
        cred.fd = -1;
        cred.pid = pid;
        cred.uid = uid;
        cred.gid = -1;
        
        if(!getSecurityFilter()(reinterpret_cast<const void *>(&cred))){
            session->acceptIncoming(false);

            // Close service if only client
            if (instanceCount <= 0 && quitOnLastInstanceClosed())
                QCoreApplication::exit();

            return;
        }
    }
    
    session->acceptIncoming(true);
    instanceCount++;
}

void QRemoteServiceRegisterDBusPrivate::processClosing()
{
    instanceCount--;

    if (instanceCount <= 0 && quitOnLastInstanceClosed())
        QCoreApplication::exit();
}

QRemoteServiceRegisterPrivate* QRemoteServiceRegisterPrivate::constructPrivateObject(QObject *parent)
{
    return new QRemoteServiceRegisterDBusPrivate(parent);
}

/*!
    Creates endpoint on client side.
*/
QObject* QRemoteServiceRegisterPrivate::proxyForService(const QRemoteServiceRegister::Entry& entry, const QString& /*location*/)
{
    const QString serviceName = "com.nokia.qtmobility.sfw." + entry.serviceName();
    QString path = "/" + entry.interfaceName() + "/DBusSession";
    path.replace(QString("."), QString("/"));

    QDBusConnection *connection = new QDBusConnection(QDBusConnection::sessionBus());
    if (!connection->isConnected()) {
        qWarning() << "Cannot connect to DBus";
        return 0;
    }

    // Dummy call to autostart the service if not running
    connection->call(QDBusMessage::createMethodCall(serviceName, path, "", "q_autostart"));

    QDBusInterface *iface = new QDBusInterface(serviceName, path, "", QDBusConnection::sessionBus());
    if (!iface->isValid()) {
        qWarning() << "Cannot connect to remote service" << serviceName << path;
        return 0;
    }

    QDBusReply<bool> reply = iface->call("processIncoming");
    if (reply.value()) {
        DBusEndPoint* ipcEndPoint = new DBusEndPoint(iface, CLIENT);
        ObjectEndPoint* endPoint = new ObjectEndPoint(ObjectEndPoint::Client, ipcEndPoint);

        QObject *proxy = endPoint->constructProxy(entry);

        QObject::connect(proxy, SIGNAL(destroyed()), endPoint, SLOT(deleteLater()));
        QObject::connect(proxy, SIGNAL(destroyed()), iface, SLOT(closeIncoming()));
        return proxy;
    }

    qDebug() << "Insufficient credentials to load a service instance";
    return 0;
}

#include "moc_qremoteserviceregister_dbus_p.cpp"
#include "qremoteserviceregister_dbus_p.moc"
QTM_END_NAMESPACE