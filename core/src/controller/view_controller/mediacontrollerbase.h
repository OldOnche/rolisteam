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
#ifndef ABSTRACTMEDIACONTROLLER_H
#define ABSTRACTMEDIACONTROLLER_H

#include <QObject>
#include <memory>

#include "media/mediatype.h"

class QUndoCommand;
class MediaControllerBase : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(Core::ContentType contentType READ contentType CONSTANT)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool modified READ modified WRITE setModified NOTIFY modifiedChanged)
    Q_PROPERTY(bool localGM READ localGM WRITE setLocalGM NOTIFY localGMChanged)
    Q_PROPERTY(QString ownerId READ ownerId WRITE setOwnerId NOTIFY ownerIdChanged)
    Q_PROPERTY(QString localId READ localId WRITE setLocalId NOTIFY localIdChanged)
    Q_PROPERTY(bool remote READ remote CONSTANT)
public:
    MediaControllerBase(const QString& id, Core::ContentType contentType, QObject* parent= nullptr);
    virtual ~MediaControllerBase() override;

    QString name() const;
    QString uuid() const;
    QString path() const;
    Core::ContentType contentType() const;
    virtual QString title() const;
    bool isActive() const;
    bool localGM() const;
    bool modified() const;

    QString ownerId() const;
    QString localId() const;

    bool localIsOwner() const;
    bool remote() const;

    virtual void saveData() const= 0;
    virtual void loadData() const= 0;

signals:
    void nameChanged();
    void uuidChanged(QString);
    void closeContainer();
    void titleChanged();
    void activeChanged();
    void localGMChanged();
    void performCommand(QUndoCommand* cmd);
    void ownerIdChanged(QString id);
    void localIdChanged(QString id);
    void modifiedChanged(bool b);
    void pathChanged(QString path);
    void closeMe(QString id);

public slots:
    void setName(const QString& name);
    void aboutToClose();
    void setActive(bool b);
    void setLocalGM(bool b);
    void setUuid(const QString& uuid);
    void setOwnerId(const QString& id);
    void setLocalId(const QString& id);
    void setModified(bool b);
    void askToClose();
    void setPath(const QString& path);
    void setRemote(bool remote);

protected:
    QString m_uuid;
    QString m_name;
    QString m_path;
    Core::ContentType m_type;
    bool m_active= false;
    bool m_localGM= false;
    bool m_modified= true;
    bool m_remote= false;
    QString m_ownerId;
    QString m_localId;
};

#endif // ABSTRACTMEDIACONTROLLER_H
