/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in 
** accordance with the Qt Commercial License Agreement provided with
** the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
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
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGSTAPPSRC_H
#define QGSTAPPSRC_H

#include <QtCore/qobject.h>
#include <QtCore/qiodevice.h>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappbuffer.h>

class QGstAppSrc  : public QObject
{
    Q_OBJECT
public:
    QGstAppSrc(QObject *parent = 0);
    ~QGstAppSrc();

    bool setup(GstElement *);
    bool isReady() const { return m_setup; }

    void setStream(QIODevice *);
    QIODevice *stream() const;

    GstAppSrc *element();

    qint64 queueSize() const { return m_maxBytes; }

    bool& enoughData() { return m_enoughData; }
    bool& dataRequested() { return m_dataRequested; }
    unsigned int& dataRequestSize() { return m_dataRequestSize; }

private slots:
    void pushDataToAppSrc();
    bool doSeek(qint64);
    void onDataReady();

private:
    static gboolean on_seek_data(GstAppSrc *element, guint64 arg0, gpointer userdata);
    static void on_enough_data(GstAppSrc *element, gpointer userdata);
    static void on_need_data(GstAppSrc *element, uint arg0, gpointer userdata);
    static void destroy_notify(gpointer data);

    void sendEOS();

    QIODevice *m_stream;
    GstAppSrc *m_appSrc;
    bool m_sequential;
    GstAppStreamType m_streamType;
    GstAppSrcCallbacks m_callbacks;
    qint64 m_maxBytes;
    bool m_setup;
    unsigned int m_dataRequestSize;
    bool m_dataRequested;
    bool m_enoughData;
    bool m_forceData;
};

#endif