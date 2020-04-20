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
#ifndef CHARACTERITEMCONTROLLERMANAGER_H
#define CHARACTERITEMCONTROLLERMANAGER_H

#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QPolygonF>
#include <memory>
#include <vector>

#include "visualitemcontrollermanager.h"
#include "vmap/controller/characteritemcontroller.h"

class VectorialMapController;
class CharacterItemControllerManager : public VisualItemControllerManager
{
    Q_OBJECT
public:
    CharacterItemControllerManager(VectorialMapController* ctrl);

    QString addItem(const std::map<QString, QVariant>& params) override;
    void addController(vmap::VisualItemController* controller) override;
    void removeItem(const QString& id) override;
    void processMessage(NetworkMessageReader* msg) override;

    const std::vector<vmap::CharacterVisionData> characterVisions() const;

    const std::vector<vmap::CharacterItemController*> controllers() const;

    int playableCharacterCount() const;

signals:
    void characterControllerCreated(vmap::CharacterItemController* ctrl);
    void playableCharacterControllerCreated();
    void playableCharacterControllerDestroyed();

private:
    std::vector<std::unique_ptr<vmap::CharacterItemController>> m_controllers;
    QPointer<VectorialMapController> m_ctrl;
};

#endif // CHARACTERITEMCONTROLLERMANAGER_H