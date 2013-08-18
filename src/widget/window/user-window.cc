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
 * \file user-window.cc
 * \author shaoner
 * \brief Window for private messages
 */
#include "user-window.hh"

#include <session.hh>
#include <command-event.hh>

#include "ui_user-window.h"
#include "common/text-display.hh"
#include "text-box.hh"
#include "edit-box.hh"

UserWindow::UserWindow(const QString& name, irc::Session& session, QWidget *parent) :
	Window(name, session, parent),
	_ui(new Ui::UserWindow)
{
	_ui->setupUi(this);
	_ui->horizontalLayout->insertWidget(0, _textBox);
	// Attach IRC event listeners
	connect(&session, SIGNAL(onPrivateMessage(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(privatemessage_slot(irc::CommandEvent&, const QString&)));
	connect(&session, SIGNAL(onQuit(irc::CommandEvent&, const QString&)),
			this, SLOT(quit_slot(irc::CommandEvent&, const QString&)));
	connect(&session, SIGNAL(onNick(irc::CommandEvent&, const QString&)),
			this, SLOT(nick_slot(irc::CommandEvent&, const QString&)));

	connect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(send_message()));
}

UserWindow::~UserWindow()
{
	disconnect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(send_message()));
	// Unattach IRC event listeners
	disconnect(&session, SIGNAL(onNick(irc::CommandEvent&, const QString&)),
			this, SLOT(nick_slot(irc::CommandEvent&, const QString&)));

	disconnect(&session, SIGNAL(onQuit(irc::CommandEvent&, const QString&)),
			this, SLOT(quit_slot(irc::CommandEvent&, const QString&)));
	disconnect(&session, SIGNAL(onPrivateMessage(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(privatemessage_slot(irc::CommandEvent&, const QString&)));
	delete _ui;
}

void
UserWindow::privatemessage_slot(irc::CommandEvent& event, const QString& msg)
{
	if (event.nick() == _name)
	{
		display_message(event.nick(), event.nick(), msg);
		if (!is_active())
		{
			item()->set_level(LIGHT_LVL_MESSAGE);
		}
	}
}

void
UserWindow::nick_slot(irc::CommandEvent& event, const QString& newNick)
{
	if (event.nick() == _name)
	{
		if (_params->eventEnabled(DISPLAY_NICK))
		{
			TextDisplay td(DISPLAY_NICK);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_target(newNick);
			display_event(td);
		}
		_name = newNick;
		setWindowTitle(_name);
	}
}

void
UserWindow::quit_slot(irc::CommandEvent& event, const QString& reason)
{
	if (event.nick() == _name)
	{
		if (_params->eventEnabled(DISPLAY_QUIT))
		{
			TextDisplay td(DISPLAY_QUIT);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_raw(reason);
			display_event(td);
		}
	}
}

void
UserWindow::disconnect_slot()
{
	display_info(tr("Disconnected"));
}

void
UserWindow::send_message()
{
	send(session.nickname(), session.nickname());
}
