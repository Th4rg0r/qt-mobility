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

#ifndef QORGANIZERITEMTODOPROGRESS_H
#define QORGANIZERITEMTODOPROGRESS_H

#include <QString>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerItemTodoProgress : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    const char* DefinitionName;
    const char* FieldStartedDateTime;
    const char* FieldFinishedDateTime;
    const char* FieldPercentageComplete;
    const char* FieldStatus;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerItemTodoProgress, "TodoProgress")
    Q_DECLARE_LATIN1_CONSTANT(FieldStartedDateTime, "StartDateTime");
    Q_DECLARE_LATIN1_CONSTANT(FieldFinishedDateTime, "FinishedDateTime");
    Q_DECLARE_LATIN1_CONSTANT(FieldPercentageComplete, "PercentageComplete");
    Q_DECLARE_LATIN1_CONSTANT(FieldStatus, "Status");
#endif

    void setStartedDateTime(const QDateTime& startedDateTime) {setValue(FieldStartedDateTime, startedDateTime);}
    QDateTime startedDateTime() const {return value<QDateTime>(FieldStartedDateTime);}

    void setFinishedDateTime(const QDateTime& finishedDateTime) {setValue(FieldFinishedDateTime, finishedDateTime);}
    QDateTime finishedDateTime() const {return value<QDateTime>(FieldFinishedDateTime);}

    void setPercentageComplete(int percentage)
    {
        if (percentage >=0 && percentage <= 100)
            setValue(FieldPercentageComplete, percentage);
    }
    int percentageComplete() const {return value<int>(FieldPercentageComplete);}

    enum Status {
        StatusNotStarted,
        StatusInProgress,
        StatusComplete,
    };

    void setStatus(Status status) {setValue(FieldStatus, (int) status);}
    Status status() const {return (Status) value<int>(FieldStatus);}
};

QTM_END_NAMESPACE

#endif
