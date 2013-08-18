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
 * \file status-window.cc
 * \author shaoner
 * \brief Status window to get server information
 */

#include "status-window.hh"

#include <QCloseEvent>
#include <session.hh>
#include <raw-event.hh>

#include "ui_status-window.h"
#include "common/text-display.hh"
#include "text-box.hh"
#include "edit-box.hh"

StatusWindow::StatusWindow(const QString& name,
						   irc::Session& session,
						   QWidget* parent) :
	Window(name, session, parent),
	_ui(new Ui::StatusWindow)
{
    _ui->setupUi(this);
    _ui->horizontalLayout->insertWidget(0, _textBox);
	// Attach IRC event listeners
    connect(&session, SIGNAL(onRaw(irc::RawEvent&)), this, SLOT(raw_slot(irc::RawEvent&)));
    connect(&session, SIGNAL(onConnecting()), this, SLOT(connecting_slot()));
    connect(&session, SIGNAL(onConnect()), this, SLOT(connect_slot()));
    connect(&session, SIGNAL(onDisconnect()), this, SLOT(disconnect_slot()));
	// When text is entered
    connect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(cmd()));
}

StatusWindow::~StatusWindow()
{
	// Unattach IRC event listeners
    disconnect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(cmd()));
    disconnect(&session, SIGNAL(onDisconnect()), this, SLOT(disconnect_slot()));
    disconnect(&session, SIGNAL(onConnect()), this, SLOT(connect_slot()));
    disconnect(&session, SIGNAL(onConnecting()), this, SLOT(connecting_slot()));
    disconnect(&session, SIGNAL(onRaw(irc::RawEvent&)), this, SLOT(raw_slot(irc::RawEvent&)));
	delete _ui;
}

void
StatusWindow::raw_slot(irc::RawEvent& event)
{
    if (_params->eventEnabled(DISPLAY_RAW))
    {
        TextDisplay td(DISPLAY_RAW);
        td.push_raw(event.msg());
        display_event(td);
    }
}

void
StatusWindow::connecting_slot()
{
    display_info("Connecting to " + session.hostname() + "...");
}

void
StatusWindow::connect_slot()
{
    display_info("Connection etablished (" + session.hostname() + ":" + QString::number(session.port()) + ")");
}

void
StatusWindow::disconnect_slot()
{
    display_info("Disconnected");
}
