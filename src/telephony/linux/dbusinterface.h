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

#ifndef INTERFACE_H_1275967729
#define INTERFACE_H_1275967729

#include "qmobilityglobal.h"
#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "message.h"

QT_BEGIN_HEADER
QTM_BEGIN_NAMESPACE
/*
 * Proxy class for interface org.freedesktop.Telepathy.Connection.Interface.Requests
 */
class Requests: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freedesktop.Telepathy.Connection.Interface.Requests"; }

public:
    Requests(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~Requests();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> createNewChannels(ChannelsArray channelsarray)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(channelsarray);
        return asyncCallWithArgumentList(QLatin1String("createNewChannels"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void NewChannels(ChannelsArray channelsarray);
};

namespace org {
    namespace freedesktop {
        namespace Telepathy {
            namespace Connection {
                namespace Interface {
                    typedef QTM_PREPEND_NAMESPACE(Requests) Requests;
                }
            }
        }
    }
}

QTM_END_NAMESPACE
QT_END_HEADER

#endif //INTERFACE_H_1275967729