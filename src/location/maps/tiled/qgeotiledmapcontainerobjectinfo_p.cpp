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

#include "qgeotiledmapcontainerobjectinfo_p.h"

#include "qgeotiledmapobjectinfo_p.h"
#include "qgeotiledmapdata.h"
#include "qgeotiledmapdata_p.h"

#include "qgeomapobject.h"
#include "qgeomapgroupobject.h"

#include <QGraphicsItem>
#include <QGraphicsPathItem>

QTM_BEGIN_NAMESPACE

QGeoTiledMapContainerObjectInfo::QGeoTiledMapContainerObjectInfo(QGeoMapData *mapData, QGeoMapObject *mapObject)
        : QGeoTiledMapObjectInfo(mapData, mapObject)
{
    group = static_cast<QGeoMapGroupObject*>(mapObject);

    connect(group,
            SIGNAL(visibleChanged(bool)),
            this,
            SLOT(visibleChanged(bool)));
    connect(group,
            SIGNAL(selectedChanged(bool)),
            this,
            SLOT(selectedChanged(bool)));

    pathItem = new QGraphicsPathItem();
    graphicsItem = pathItem;
    pathItem->setPos(0.0, 0.0);
    updateItem();
}

QGeoTiledMapContainerObjectInfo::~QGeoTiledMapContainerObjectInfo() {}

void QGeoTiledMapContainerObjectInfo::visibleChanged(bool visible)
{
    QList<QGeoMapObject*> objects = group->childObjects();
    for (int i = 0; i < objects.size(); ++i)
        objects[i]->setVisible(visible);
}

void QGeoTiledMapContainerObjectInfo::selectedChanged(bool selected)
{
    QList<QGeoMapObject*> objects = group->childObjects();
    for (int i = 0; i < objects.size(); ++i)
        objects[i]->setSelected(selected);
}

#include "moc_qgeotiledmapcontainerobjectinfo_p.cpp"

QTM_END_NAMESPACE