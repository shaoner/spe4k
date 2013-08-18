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
 * \file window.cc
 * \author shaoner
 * \brief Window interface
 */
#include "window.hh"
#include <QTime>
#include <QTextBrowser>
#include <QScrollBar>
#include <session.hh>

#include "common/parameters.hh"
#include "common/command-handler.hh"
#include "common/text-display.hh"
#include "text-box.hh"
#include "edit-box.hh"

Window::Window(const QString& name,
			   irc::Session& session_,
			   QWidget* parent) :
	QWidget(parent),
	session(session_),
	_textBox(new TextBox(this)),
	_name(name),
	_params(Parameters::get()),
	_item(new WindowItem(name))
{
	setFocusProxy(_textBox->input);
}

Window::~Window()
{
	delete _textBox;
	delete _item;
}

void
Window::inputFocus()
{
	_textBox->input->setFocus();
}

void
Window::display(const QString& text)
{
	if (text.isEmpty())
		return;
	_textBox->screen->moveCursor(QTextCursor::End);
	if (_params->get<bool>("Display/timestamp"))
	{
		QTime time = QTime::currentTime();
		QString strTime = time.toString(_params->get<QString>("Display/timestamp_format"));
		_textBox->screen->insertHtml(strTime + " " + text + "<br>");
	}
	else
	{
		_textBox->screen->insertHtml(text + "<br>");
	}
	// Need to reset the the scrollbar at its max
	QScrollBar* sb = _textBox->screen->verticalScrollBar();
	sb->setValue(sb->maximum());
	/*
	 * TODO: The scrollbar should be set at the max only if it is close the max
	 */
}

void
Window::display_message(const QString& fullnick, const QString& tag, const QString& text)
{
	if (text.isEmpty())
		return;
	if (_params->eventEnabled(DISPLAY_MESSAGE))
	{
		TextDisplay td(DISPLAY_MESSAGE);
		td.push_nick(fullnick, tag);
		td.push_ircraw(text);
		display(td.richText());
	}
}

void
Window::display_action(const QString& fullnick, const QString& tag, const QString& text)
{
	if (text.isEmpty())
		return;
	if (_params->eventEnabled(DISPLAY_ACTION))
	{
		TextDisplay td(DISPLAY_ACTION);
		td.push_nick(fullnick, tag);
		td.push_ircraw(text);
		display(td.richText());
	}
}

void
Window::display_event(TextDisplay& textDisplay)
{
	display(textDisplay.richText());
	if (!is_active())
		_item->set_level(LIGHT_LVL_EVENT);
}

void
Window::display_info(const QString& text)
{
	if (_params->eventEnabled(DISPLAY_INFO))
	{
		TextDisplay td(DISPLAY_INFO);
		td.push_raw(text);
		display(td.richText());
	}
}

void
Window::send(const QString& fullnick, const QString& tag)
{
	QString text = _textBox->input->text();
	_textBox->input->clear();
	if (text.startsWith('/'))
	{
		parse_command(text);
	}
	else
	{
		display_message(fullnick, tag, text);
		session.privmsg(_name, text);
	}
}

void
Window::cmd()
{
	QString text = _textBox->input->text();
	_textBox->input->clear();
	if (text.startsWith('/'))
	{
		parse_command(text);
	}
}

void
Window::parse_command(QString& text)
{
	QStringList arg = text.split(' ');
	QString command = arg.takeFirst().mid(1).toUpper();
	CommandHandler::get()->exec(this, command, arg);
}
