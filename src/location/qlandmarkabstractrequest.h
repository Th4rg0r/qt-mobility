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

#ifndef QLANDMARKABSTRACTREQUEST_H
#define QLANDMARKABSTRACTREQUEST_H

#include "qlandmarkmanager.h"
#include <QObject>

QTM_BEGIN_NAMESPACE

class QLandmarkAbstractRequestPrivate;
class Q_LOCATION_EXPORT QLandmarkAbstractRequest : public QObject
{
    Q_OBJECT

public:
    enum State {
        InactiveState = 0,
        ActiveState,
        CanceledState,
        FinishedState
    };

    enum RequestType {
        InvalidRequest = 0,
        LandmarkIdFetchRequest,
        CategoryIdFetchRequest,
        LandmarkFetchRequest,
        CategoryFetchRequest,
        LandmarkSaveRequest,
        LandmarkRemoveRequest,
        CategorySaveRequest,
        CategoryRemoveRequest,
        ImportRequest,
        ExportRequest
    };

    QLandmarkAbstractRequest(QLandmarkManager *manager, QObject *parent = 0);
    virtual ~QLandmarkAbstractRequest();

    RequestType type() const;

    State state();
    bool isInactive()const;
    bool isActive() const;
    bool isFinished() const;
    bool isCanceled() const;

    QLandmarkManager::Error error() const;
    QString errorString() const;

    QLandmarkManager * manager() const;
    void setManager(QLandmarkManager *manager);

public slots:
    bool start();
    bool cancel();
    bool waitForFinished(int msecs = 0);

signals:
    void resultsAvailable();
    void stateChanged(QLandmarkAbstractRequest::State newState);
protected:
    QLandmarkAbstractRequest(QLandmarkAbstractRequestPrivate *otherd, QObject *parent = 0);
    QLandmarkAbstractRequestPrivate *d_ptr;
private:
    Q_DISABLE_COPY(QLandmarkAbstractRequest)
    friend class QLandmarkManagerEngine;
};

QTM_END_NAMESPACE

#endif