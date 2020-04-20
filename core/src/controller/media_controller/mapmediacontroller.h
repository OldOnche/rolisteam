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
#ifndef MAPMEDIACONTROLLER_H
#define MAPMEDIACONTROLLER_H

#include <QObject>
#include <memory>
#include <vector>

#include "mediacontrollerinterface.h"

class MapController;
class MapUpdater;
class MapMediaController : public MediaControllerInterface
{
    Q_OBJECT
    Q_PROPERTY(QColor fogColor READ fogColor WRITE setFogColor NOTIFY fogColorChanged)
public:
    explicit MapMediaController(QObject* parent= nullptr);
    ~MapMediaController() override;

    QColor fogColor() const;
    void setFogColor(const QColor& color);

    CleverURI::ContentType type() const override;
    bool openMedia(CleverURI* uri, const std::map<QString, QVariant>& args) override;
    void closeMedia(const QString& id) override;
    void registerNetworkReceiver() override;
    NetWorkReceiver::SendType processMessage(NetworkMessageReader* msg) override;
    void setUndoStack(QUndoStack* stack) override;

    void addMapFromImage(const QPixmap& map);
    void addMapController(CleverURI* uri, const QHash<QString, QVariant>& params);

signals:
    void mapControllerCreated(MapController* ctrl);
    void fogColorChanged(QColor color);

private:
    std::vector<std::unique_ptr<MapController>> m_maps;
    std::unique_ptr<MapUpdater> m_updater;
    QColor m_fogColor;
};

#endif // MAPMEDIACONTROLLER_H