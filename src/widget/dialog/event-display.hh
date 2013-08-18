/*
 *
 * Copyright 2013, Alexandre Laurent <shaoner>
 *
 * This file is part of spe4k.
 *
 * spe4k is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spe4k is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spe4k. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*!
 * \file event-display.hh
 * \author shaoner
 * \brief
 */

#ifndef EVENT_DISPLAY_HH
#define EVENT_DISPLAY_HH

#include <QWidget>

#include "common/parameters.hh"

namespace Ui
{
    class EventDisplay;
} // namespace Ui

class Parameters;

class EventDisplay : public QWidget
{
    Q_OBJECT
    
public:
    explicit EventDisplay(DisplayEventType displayType, QWidget* parent = 0);
    ~EventDisplay();
public:
    void save() const;
private:
    void init();
private slots:
    void on_btnEventColor_clicked();

    void on_edtEventFormat_textChanged(const QString& fmt);

    void on_btnReset_clicked();

private:
    Ui::EventDisplay* _ui;
    DisplayEventType _displayType;
    Parameters* _params;
    QString _format;
    QColor _color;
};

#endif /* !EVENT_DISPLAY_HH */
