/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QCoreApplication>
#include <QTimer>
#include <qremoteserviceclassregister.h>
#include <qremoteservicecontrol.h>
#include "qservicemanager.h"
#include <QDebug>

#include "qservicefilter.h" //only used to test custom metatype

QTM_USE_NAMESPACE
Q_DECLARE_METATYPE(QServiceFilter);
Q_DECLARE_METATYPE(QVariant);
Q_DECLARE_METATYPE(QList<QString>);

class SharedTestService : public QObject 
{
    Q_OBJECT
    Q_SERVICE(SharedTestService, "IPCExampleService", "com.nokia.qt.ipcunittest", "3.4")
    Q_PROPERTY(QString value READ value WRITE setValue RESET resetValue NOTIFY valueChanged SCRIPTABLE true DESIGNABLE true STORED true); 

public:
    SharedTestService(QObject* parent = 0) 
        : QObject(parent)
    {
        resetValue();
    }

    QString value() const
    {
        qDebug() << "SharedTestService: Reading property";
        return m_value;
    }

    void setValue(const QString& value)
    {
        qDebug() << "SharedTestService: Writing property";
        m_value = value;
        emit valueChanged();
    }

    void resetValue()
    {
        qDebug() << "SharedTestService: Resetting property";
        m_value = "FFF";
        emit valueChanged();
    }

    Q_INVOKABLE QString testFunctionWithReturnValue(int input)
    {
        qDebug() << "SharedTestService::testFunctionWithReturnValue(" << input << ")";
        QString output("%1 + 3 = %2");
        output = output.arg(input).arg(input+3);
        return output;
    }
    
    Q_INVOKABLE QVariant testFunctionWithVariantReturnValue(const QVariant& input)
    {
        qDebug() << "SharedTestService::testFunctionWithVariantReturnValue()";
        m_hash = qHash(input.toString());
        return input;
    }

    Q_INVOKABLE QServiceFilter testFunctionWithCustomReturnValue()
    {
        qDebug() << "SharedTestService::testFunctionWithCustomReturnValue()";
        QServiceFilter f("com.nokia.qt.ipcunittest", "6.2");
        f.setServiceName("MySharedService");
        return f;
    }

    Q_INVOKABLE uint slotConfirmation() const
    {
        return m_hash;
    }

    Q_INVOKABLE void setConfirmationHash(uint hash)
    {
        m_hash = hash;
    }

Q_SIGNALS:
    void signalWithIntParam(int);
    void signalWithVariousParam(QVariant,QString,QServiceFilter);
    void valueChanged();

public slots:
    void triggerSignalWithIntParam()
    {
        QTimer::singleShot(2000, this, SLOT(triggerSignalWithIntParamExecute()));
    }
    
    void triggerSignalWithVariousParam()
    {
        QTimer::singleShot(2000, this, SLOT(triggerSignalWithVariousParamExecute()));
    }

    void triggerSignalWithIntParamExecute()
    {
        qDebug() << "Emitting SharedTestService::signalWithIntParam()";
        emit signalWithIntParam( 5 );
    }

    void triggerSignalWithVariousParamExecute()
    {
        QServiceFilter f("com.nokia.qt.ipcunittest", "6.7");
        f.setServiceName("MyService");
        qDebug() << "Emitting SharedTestService::signalWithVariousParam()";
        emit signalWithVariousParam( QVariant(), QString("string-value"), f );
    }
    void testSlot()
    {
        m_hash = qHash(QString("testSlot()"));
        qDebug() << "SharedTestService::testSlot() called" << m_hash;
    }

    void testSlotWithArgs(const QByteArray& d, int a, const QVariant& variant)
    {
        QString output("%1, %2, %3, %4");
        output = output.arg(d.constData()).arg(a).arg(variant.toString()).arg(variant.isValid());
        m_hash = qHash(output);
        qDebug() << "SharedTestService::testSlotWithArgs(" << output << ") called" << m_hash;
    }
    
    void testSlotWithCustomArg(const QServiceFilter& f)
    {
        QString output("%1: %2 - %3.%4");
        output = output.arg(f.serviceName()).arg(f.interfaceName())
                .arg(f.majorVersion()).arg(f.minorVersion());
        m_hash = qHash(output);
        qDebug() << "SharedTestService::testSlotWithCustomArg(" << output << ") called" << m_hash;
    }
    
    void testSlotWithUnknownArg(const QServiceInterfaceDescriptor& )
    {
        qDebug() << "SharedTestService::testSlotWithUnknownArg(const QServiceInterfaceDescriptor& d)";
        m_hash = 1;
    }
    
    void testIpcFailure() {
      qApp->exit(0); // exit to show failure
    }

private:
    QString m_value;
    uint m_hash;
};

class UniqueTestService : public QObject 
{
    Q_OBJECT
    Q_SERVICE(UniqueTestService, "IPCExampleService", "com.nokia.qt.ipcunittest", "3.5")
    Q_CLASSINFO("UniqueTestService", "First test");
    Q_CLASSINFO("Key", "Value");

    Q_PROPERTY(QString value READ value WRITE setValue RESET resetValue NOTIFY valueChanged SCRIPTABLE true DESIGNABLE true STORED true); 
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged);
    Q_PROPERTY(ServiceFlag serviceFlags READ flags WRITE setFlags);
    Q_ENUMS(Priority);
    Q_FLAGS(ServiceFlag ServiceFlags);

public:
    enum ServiceFlag {
        FirstBit = 0x01,
        SecondBit = 0x02,
        ThirdBit = 0x04
    };
    Q_DECLARE_FLAGS(ServiceFlags, ServiceFlag)

    ServiceFlags flags() const { return m_flags; }
    void setFlags(ServiceFlags f) {
        m_flags = f;
    }

    enum Priority { High, Low, VeryLow, ExtremelyLow };
    
    void setPriority(Priority p) {
        m_priority = p;
        emit priorityChanged();
    }
    
    Priority priority() const
    {
        return m_priority;
    }

    UniqueTestService(QObject* parent = 0) 
        : QObject(parent), m_priority(UniqueTestService::High), m_flags(ThirdBit)
    {
        resetValue();
    }

    QString value() const
    {
        qDebug() << "UniqueTestService: Reading property";
        return m_value;
    }

    void setValue(const QString& value)
    {
        qDebug() << "UniqueTestService: Writing property";
        m_value = value;
        emit valueChanged();
    }

    void resetValue()
    {
        qDebug() << "UniqueTestService: Resetting value";
        m_value = "FFF";
        emit valueChanged();
    }

    Q_INVOKABLE QString testFunctionWithReturnValue(int input)
    {
        qDebug() << "UniqueTestService::testFunctionWithReturnValue(" << input << ")";
        QString output("%1 x 3 = %2");
        output = output.arg(input).arg(input*3);
        return output;
    }

    Q_INVOKABLE QVariant testFunctionWithVariantReturnValue(const QVariant& input)
    {
        qDebug() << "UniqueTestService::testFunctionWithVariantReturnValue()";
        return input;
    }

    Q_INVOKABLE QServiceFilter testFunctionWithCustomReturnValue()
    {
        qDebug() << "UniqueTestService::testFunctionWithCustomReturnValue()";
        QServiceFilter f("com.nokia.qt.ipcunittest", "6.7");
        f.setServiceName("MyUniqueService");
        return f;
    }
    
    Q_INVOKABLE QList<QString> testFunctionWithListReturn()
    {
        qDebug() << "UniqueTestService::testFunctionWithListReturn()";
        QList<QString> list;
        list << "1" << "2" << "3";
        return list;
    }
    
    Q_INVOKABLE uint slotConfirmation() const
    {
        return m_hash;
    }

    Q_INVOKABLE void setConfirmationHash(uint hash)
    {
        m_hash = hash;
    }

Q_SIGNALS:
    void signalWithIntParam(int);
    void signalWithVariousParam(QVariant,QString,QServiceFilter,QVariant);
    void valueChanged();
    void priorityChanged();

public slots:
    void triggerSignalWithIntParam()
    {
        QTimer::singleShot(2000, this, SLOT(triggerSignalWithIntParamExecute()));
    }

    void triggerSignalWithVariousParam()
    {
        QTimer::singleShot(2000, this, SLOT(triggerSignalWithVariousParamExecute()));
    }

    void triggerSignalWithIntParamExecute()
    {
        qDebug() << "Emitting UniqueTestService::signalWithIntParam()";
        emit signalWithIntParam( 5 );
    }
    
    void triggerSignalWithVariousParamExecute()
    {
        QServiceFilter f("com.nokia.qt.ipcunittest", "6.7");
        f.setServiceName("MyService");
        qDebug() << "Emitting UniqueTestService::signalWithVariousParam()";
        emit signalWithVariousParam( QVariant("CAN'T BE NULL"), QString("string-value"), f, QVariant(5) );
    }

    void testSlot() {
        m_hash = qHash(QString("testSlot()"));
        qDebug() << "UniqueTestService::testSlot() called" << m_hash;
    }
    
    void testSlotWithArgs(const QByteArray& d, int a, const QVariant& variant)
    {
        QString output("%1, %2, %3, %4");
        output = output.arg(d.constData()).arg(a).arg(variant.toString()).arg(variant.isValid());
        m_hash = qHash(output);
        qDebug() << "UniqueTestService::testSlotWithArgs(" << output << ") called" << m_hash;
    }

    void testSlotWithCustomArg(const QServiceFilter& f)
    {
        QString output("%1: %2 - %3.%4");
        output = output.arg(f.serviceName()).arg(f.interfaceName())
                .arg(f.majorVersion()).arg(f.minorVersion());
        m_hash = qHash(output);
        qDebug() << "UniqueTestService::testSlotWithCustomArg(" << output << ") called" << m_hash;
    }
    
    void testSlotWithUnknownArg(const QServiceInterfaceDescriptor& )
    {
        qDebug() << "UniqueTestService::testSlotWithUnknownArg(const QServiceInterfaceDescriptor& d)";
        m_hash = 1;
    }
    
    void testSlotWithListArg(QList<QString> list)
    {
        QString output;
        for (int i=0; i<list.size(); i++) {
            output += list[i];
            if (i<list.size()-1)
                output += ", ";
        }
        m_hash = qHash(output);
        qDebug() << "UniqueTestService::testSlotWithListArg( QList(" << output << " ) ) called" << m_hash;
    }

    void testIpcFailure() {
      qApp->exit(0); // exit to show failure
    }
    
private:
    QString m_value;
    Priority m_priority;
    ServiceFlags m_flags;
    uint m_hash;
};

void unregisterExampleService()
{
    QServiceManager m;
    m.removeService("IPCExampleService");
}

void registerExampleService()
{
    unregisterExampleService();
    QServiceManager m;
    const QString path = QCoreApplication::applicationDirPath() + "/xmldata/ipcexampleservice.xml";
    bool r = m.addService(path);
    if (!r)
        qWarning() << "Cannot register IPCExampleService" << path;
}

Q_DECLARE_METATYPE(QMetaType::Type);

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    qRegisterMetaType<QServiceFilter>();
    qRegisterMetaTypeStreamOperators<QServiceFilter>("QServiceFilter");
    
    qRegisterMetaType<QVariant>();    
    qRegisterMetaTypeStreamOperators<QVariant>("QVariant");
  
    qRegisterMetaType<QList<QString> >();    
    qRegisterMetaTypeStreamOperators<QList<QString> >("QList<QString>");
   
    registerExampleService();

    QRemoteServiceClassRegister::registerType<SharedTestService>(QRemoteServiceClassRegister::SharedInstance);
    QRemoteServiceClassRegister::registerType<UniqueTestService>(QRemoteServiceClassRegister::UniqueInstance);

    //this only works
    QRemoteServiceControl* control = new QRemoteServiceControl();
    control->publishServices("qt_sfw_example_ipc_unittest");
    int res =  app.exec();
    delete control;
    unregisterExampleService();
    return res;
}


#include "main.moc"