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

#include "nearfieldtagndefrequest_symbian.h"
#include "nearfieldutility_symbian.h"
#include "nearfieldtagimplcommon_symbian.h"
#include "debug.h"

NearFieldTagNdefRequest::NearFieldTagNdefRequest(QNearFieldTagImplCommon& aOperator) : MNearFieldTagAsyncRequest(aOperator)
{
}

NearFieldTagNdefRequest::~NearFieldTagNdefRequest()
{
    BEGIN
    if (iRequestIssued)
    {
        iOperator.DoCancelNdefAccess();
    }
    END
}

void NearFieldTagNdefRequest::IssueRequest()
{
    BEGIN

    iRequestIssued = ETrue;
    LOG(iType);
    switch(iType)
    {
        case EReadRequest:
        {
            iOperator.DoReadNdefMessages(this);
            break;
        }
        case EWriteRequest:
        {
            iOperator.DoSetNdefMessages(iMessages, this);
            break;
        }
        default:
        {
            iRequestIssued = EFalse;
            break;
        }
    }

    END
}

void NearFieldTagNdefRequest::ReadComplete(TInt aError, RPointerArray<CNdefMessage> * aMessage)
{
    BEGIN
    LOG(aError);
    iRequestIssued = EFalse;
    TRAP_IGNORE(
    if (aMessage != 0)
    {
        for(int i = 0; i < aMessage->Count(); ++i)
        {
        QNdefMessage message = QNFCNdefUtility::CNdefMsg2QNdefMsgL(*(*aMessage)[i]);
        iReadMessages.append(message);
        }
    }
    else
    {
        iReadMessages.clear();
    }
    ) // TRAP end
    ProcessResponse(aError);
    END
}

void NearFieldTagNdefRequest::WriteComplete(TInt aError)
{
    BEGIN
    iRequestIssued = EFalse;
    ProcessResponse(aError);
    END
}

void NearFieldTagNdefRequest::ProcessEmitSignal(TInt aError)
{
    BEGIN
    LOG("error code is "<<aError<<" request type is "<<iType);
    if (aError != KErrNone)
    {
        iOperator.EmitError(aError, iId);
    }
    else
    {
        if (EReadRequest == iType)
        {
            // since there is no error, iReadMessages can't be NULL.

            LOG("message count is "<<iReadMessages.count());
            for(int i = 0; i < iReadMessages.count(); ++i)
            {
                LOG("emit signal ndef message read");
                iOperator.EmitNdefMessageRead(iReadMessages.at(i));
            }
            iOperator.EmitRequestCompleted(iId);
        }
        else if (EWriteRequest == iType)
        {
            iOperator.EmitNdefMessagesWritten();
            //iOperator.EmitRequestCompleted(iId);
        }
    }
    END
}

void NearFieldTagNdefRequest::ProcessTimeout()
{
    if (iWait)
    {
        if (iWait->IsStarted())
        {
            if (iRequestIssued)
            {
                iOperator.DoCancelNdefAccess();
                iRequestIssued = EFalse;
            }
            ProcessResponse(KErrTimedOut);
        }
    }
}

void NearFieldTagNdefRequest::HandleResponse(TInt /*aError*/)
{
    BEGIN
    END
}
