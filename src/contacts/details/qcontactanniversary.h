/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCONTACTANNIVERSARY_H
#define QCONTACTANNIVERSARY_H

#include <QString>

#include "qtcontactsglobal.h"
#include "qcontactdetail.h"
#include "qcontact.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactAnniversary : public QContactDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldCalendarId;
    static const QLatin1Constant FieldOriginalDate;
    static const QLatin1Constant FieldEvent;
    static const QLatin1Constant FieldSubType;
    static const QLatin1Constant SubTypeWedding;
    static const QLatin1Constant SubTypeEngagement;
    static const QLatin1Constant SubTypeHouse;
    static const QLatin1Constant SubTypeEmployment;
    static const QLatin1Constant SubTypeMemorial;
#else
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactAnniversary, "Anniversary")
    Q_DECLARE_LATIN1_CONSTANT(FieldCalendarId, "CalendarId");
    Q_DECLARE_LATIN1_CONSTANT(FieldOriginalDate, "OriginalDate");
    Q_DECLARE_LATIN1_CONSTANT(FieldEvent, "Event");
    Q_DECLARE_LATIN1_CONSTANT(FieldSubType, "SubType");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeWedding, "Wedding");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeEngagement, "Engagement");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeHouse, "House");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeEmployment, "Employment");
    Q_DECLARE_LATIN1_CONSTANT(SubTypeMemorial, "Memorial");
#endif

    void setOriginalDate(const QDate& date) {setValue(FieldOriginalDate, date);}
    QDate originalDate() const {return value<QDate>(FieldOriginalDate);}
    void setOriginalDateTime(const QDateTime& dateTime) {setValue(FieldOriginalDate, dateTime);}
    QDateTime originalDateTime() const {return value<QDateTime>(FieldOriginalDate);}

    void setCalendarId(const QString& calendarId) {setValue(FieldCalendarId, calendarId);}
    QString calendarId() const {return value(FieldCalendarId);}
    void setEvent(const QString& event) {setValue(FieldEvent, event);}
    QString event() const {return value(FieldEvent);}

    void setSubType(const QString& subType) {setValue(FieldSubType, subType);}
    QString subType() const {return value(FieldSubType);}
};

QTM_END_NAMESPACE

#endif

