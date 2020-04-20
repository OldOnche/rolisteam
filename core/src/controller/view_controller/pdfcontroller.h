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
#ifndef PDFCONTROLLER_H
#define PDFCONTROLLER_H

#include <QObject>

#include "abstractmediacontroller.h"
#include "data/cleveruri.h"

class CleverURI;
class PdfController : public AbstractMediaContainerController
{
    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data NOTIFY dataChanged)
public:
    explicit PdfController(CleverURI* uri, const QByteArray& data= QByteArray(), QObject* parent= nullptr);
    ~PdfController() override;

    void saveData() const override;
    void loadData() const override;

    QByteArray data() const;

public slots:
    // actions
    void shareImageIntoImage(const QPixmap& image);
    void shareImageIntoMap(const QPixmap& image);
    void shareImageIntoVMap(const QPixmap& image);
    void shareAsPdf();

signals:
    void dataChanged(QByteArray data);
    void openImageAs(const QPixmap&, CleverURI::ContentType);
    void sharePdf(QString id);

private:
    QByteArray m_data;
    bool m_overlayVisibility= false;
};

#endif // PDFCONTROLLER_H