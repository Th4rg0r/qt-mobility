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

#ifndef S60CAMERAIMAGEPROCESSINGCONTROL_H
#define S60CAMERAIMAGEPROCESSINGCONTROL_H

#include <QtCore/qobject.h>
#include "qimageprocessingcontrol.h"
#include "s60camerasettings.h"

QTM_USE_NAMESPACE

class S60CameraService;
class S60ImageCaptureSession;

class S60CameraImageProcessingControl : public QImageProcessingControl
{
    Q_OBJECT
public:
    S60CameraImageProcessingControl(QObject *parent = 0);
    S60CameraImageProcessingControl(QObject *session, QObject *parent = 0);
    ~S60CameraImageProcessingControl();

    QCamera::WhiteBalanceMode whiteBalanceMode() const;
    void setWhiteBalanceMode(QCamera::WhiteBalanceMode mode);
    QCamera::WhiteBalanceModes supportedWhiteBalanceModes() const;
    int manualWhiteBalance() const;
    void setManualWhiteBalance(int colorTemperature);

    int contrast() const;
    void setContrast(int value);

    int saturation() const;
    void setSaturation(int value);

    bool isSharpeningSupported() const;
    int sharpeningLevel() const;
    void setSharpeningLevel(int value);

    bool isDenoisingSupported() const;
    int denoisingLevel() const;
    void setDenoisingLevel(int value);
    bool isWhiteBalanceLocked() const;

public Q_SLOTS:
    void resetAdvancedSetting();
    void unlockWhiteBalance();
    void lockWhiteBalance();

private:
    S60ImageCaptureSession *m_session;
    S60CameraService *m_service;
    S60CameraSettings *m_advancedSettings;
};

#endif