/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                               *
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
#include "ellipscontrollermanager.h"

#include "controller/view_controller/vectorialmapcontroller.h"
#include "updater/ellipsecontrollerupdater.h"
#include "vmap/controller/ellipsecontroller.h"
#include "worker/messagehelper.h"

#include <QDebug>

EllipsControllerManager::EllipsControllerManager(VectorialMapController* ctrl)
    : m_ctrl(ctrl), m_updater(new EllipseControllerUpdater)
{
    auto func= [this]() { m_updater->setSynchronized(m_ctrl->localGM() || m_ctrl->permission() == Core::PC_ALL); };
    connect(ctrl, &VectorialMapController::localGMChanged, this, func);
    connect(ctrl, &VectorialMapController::permissionChanged, this, func);
}

EllipsControllerManager::~EllipsControllerManager()= default;

QString EllipsControllerManager::addItem(const std::map<QString, QVariant>& params)
{
    std::unique_ptr<vmap::EllipseController> ellipse(new vmap::EllipseController(params, m_ctrl));
    emit ellipsControllerCreated(ellipse.get(), true);
    auto id= ellipse->uuid();
    prepareController(ellipse.get());
    emit itemAdded(ellipse->uuid());
    m_controllers.push_back(std::move(ellipse));
    return id;
}

void EllipsControllerManager::addController(vmap::VisualItemController* controller)
{
    auto ellipseCtrl= dynamic_cast<vmap::EllipseController*>(controller);
    if(nullptr == ellipseCtrl)
        return;

    std::unique_ptr<vmap::EllipseController> ellipse(ellipseCtrl);
    emit ellipsControllerCreated(ellipse.get(), false);
    emit itemAdded(ellipseCtrl->uuid());
    qDebug() << "add epllipse " << m_controllers.size();
    m_controllers.push_back(std::move(ellipse));
}

void EllipsControllerManager::removeItem(const QString& id)
{
    auto it= std::find_if(m_controllers.begin(), m_controllers.end(),
                          [id](const std::unique_ptr<vmap::EllipseController>& ctrl) { return id == ctrl->uuid(); });

    if(it == m_controllers.end())
        return;

    (*it)->aboutToBeRemoved();
    m_controllers.erase(it);
}

void EllipsControllerManager::prepareController(vmap::EllipseController* ctrl)
{
    auto id= m_ctrl->uuid();
    connect(ctrl, &vmap::EllipseController::initializedChanged, this,
            [id, ctrl]() { MessageHelper::sendOffEllispe(ctrl, id); });
    m_updater->addEllipseController(ctrl);
}

void EllipsControllerManager::processMessage(NetworkMessageReader* msg)
{
    if(msg->action() == NetMsg::AddItem && msg->category() == NetMsg::VMapCategory)
    {
        auto hash= MessageHelper::readEllipse(msg);
        auto newEllipse= new vmap::EllipseController(hash, m_ctrl);
        addController(newEllipse);
    }
    else if(msg->action() == NetMsg::UpdateItem && msg->category() == NetMsg::VMapCategory)
    {
        auto id= msg->string8();
        auto ctrl= findController(id);
        if(nullptr != ctrl)
            m_updater->updateItemProperty(msg, ctrl);
    }
}

bool EllipsControllerManager::loadItem(const QString& id)
{
    auto it= std::find_if(m_controllers.begin(), m_controllers.end(),
                          [id](const std::unique_ptr<vmap::EllipseController>& ctrl) { return id == ctrl->uuid(); });

    if(it == m_controllers.end())
        return false;

    emit ellipsControllerCreated(it->get(), false);
    return true;
}

vmap::EllipseController* EllipsControllerManager::findController(const QString& id)
{
    auto it= std::find_if(m_controllers.begin(), m_controllers.end(),
                          [id](const std::unique_ptr<vmap::EllipseController>& ctrl) { return id == ctrl->uuid(); });

    if(it == m_controllers.end())
        return nullptr;

    return it->get();
}

const std::vector<vmap::EllipseController*> EllipsControllerManager::controllers() const
{
    std::vector<vmap::EllipseController*> vect;
    std::transform(m_controllers.begin(), m_controllers.end(), std::back_inserter(vect),
                   [](const std::unique_ptr<vmap::EllipseController>& ctrl) { return ctrl.get(); });
    return vect;
}
