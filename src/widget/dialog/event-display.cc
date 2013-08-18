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
 * \file event-display.cc
 * \author shaoner
 * \brief Option widget to format an event
 */

#include "event-display.hh"

#include <QColorDialog>
#include "ui_event-display.h"
#include "common/parameters.hh"

EventDisplay::EventDisplay(DisplayEventType displayType, QWidget* parent) :
	QWidget(parent),
	_ui(new Ui::EventDisplay),
	_displayType(displayType),
	_params(Parameters::get())
{

	_ui->setupUi(this);
	init();
}

EventDisplay::~EventDisplay()
{
	delete _ui;
}

void
EventDisplay::save() const
{
	_params->set_eventFormat(_displayType, _format);
	_params->set_eventColor(_displayType, _color.name());
	_params->set_eventEnabled(_displayType, _ui->chkEventEnabled->isChecked());
}

void
EventDisplay::init()
{
	_format = _params->eventFormat(_displayType);
	_color = _params->eventColor(_displayType);
	_ui->chkEventEnabled->setChecked(_params->eventEnabled(_displayType));
	_ui->edtEventFormat->setText(_format);
	_ui->lblColor->setText(_color.name());
	_ui->lblColor->setPalette(QPalette(_color));
}

void
EventDisplay::on_btnEventColor_clicked()
{
	QColor color = QColorDialog::getColor(_color, this);
	if (color.isValid())
	{
		_color = color;
		_ui->edtEventVisual->setHtml("<span style=\"color:" + _color.name() + ";\">"
									 + _format + "</span>");
		_ui->lblColor->setText(_color.name());
		_ui->lblColor->setPalette(QPalette(_color));
	}
}

void
EventDisplay::on_edtEventFormat_textChanged(const QString& fmt)
{
	_format = fmt;
	_ui->edtEventVisual->setHtml("<span style=\"color:" + _color.name() + ";\">"
								 + fmt + "</span>");
}

void
EventDisplay::on_btnReset_clicked()
{
	_params->reset_event(_displayType);
	init();
}
