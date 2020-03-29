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
#ifndef MEDIAUPDATER_H
#define MEDIAUPDATER_H

#include <QObject>
#include <QRectF>

namespace vmap
{
class VisualItemController;
}
class NetworkMessageReader;
class VMapItemControllerUpdater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool synchronized READ synchronized WRITE setSynchronized NOTIFY synchronizedChanged)
public:
    explicit VMapItemControllerUpdater(QObject* parent= nullptr);

    void addItemController(vmap::VisualItemController* ctrl);
    virtual bool updateItemProperty(NetworkMessageReader* msg, vmap::VisualItemController* ctrl);

    template <typename T>
    void sendOffVMapChanges(vmap::VisualItemController* ctrl, const QString& property);
    bool synchronized() const;

public slots:
    void setSynchronized(bool);
signals:
    void synchronizedChanged(bool);

protected:
    bool m_updatingFromNetwork= false;
    vmap::VisualItemController* updatingCtrl= nullptr;
    bool m_synchronized= true;
};

#endif // MEDIAUPDATER_H
