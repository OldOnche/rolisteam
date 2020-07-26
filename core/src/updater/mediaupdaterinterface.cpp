/***************************************************************************
 *	Copyright (C) 2020 by Renaud Guezennec                               *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "mediaupdaterinterface.h"

#include "controller/media_controller/mediamanagerbase.h"

#include "network/networkmessagewriter.h"
#include "worker/convertionhelper.h"

MediaUpdaterInterface::MediaUpdaterInterface(QObject *object)
: QObject(object)
{
    // Make compiler generate proper template
    sendOffChanges<double>(nullptr, QString());
    sendOffChanges<int>(nullptr, QString());
    sendOffChanges<QColor>(nullptr, QString());
    sendOffChanges<Core::ScaleUnit>(nullptr, QString());
    sendOffChanges<QString>(nullptr, QString());
    sendOffChanges<Core::ScaleUnit>(nullptr, QString());
    sendOffChanges<Core::Layer>(nullptr, QString());
    sendOffChanges<Core::GridPattern>(nullptr, QString());
    sendOffChanges<Core::PermissionMode>(nullptr, QString());
    sendOffChanges<Core::VisibilityMode>(nullptr, QString());
    sendOffChanges<bool>(nullptr, QString());
}

template<typename T>
void MediaUpdaterInterface::sendOffChanges(MediaControllerBase *ctrl, const QString &property)
{
    if(nullptr == ctrl || m_updatingFromNetwork)
        return;

    NetworkMessageWriter msg(NetMsg::MediaCategory, NetMsg::UpdateMediaProperty);
    msg.uint8(static_cast<int>(ctrl->contentType()));
    msg.string8(ctrl->uuid());
    msg.string16(property);
    auto val= ctrl->property(property.toLocal8Bit().data());
    Helper::variantToType<T>(val.value<T>(), msg);
    msg.sendToServer();
}


