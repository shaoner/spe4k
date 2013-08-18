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
 * \file text-display.cc
 * \author shaoner
 * \brief Formatting text for internal windows
 */

#include "text-display.hh"

TextDisplay::TextDisplay(const QString& text, const QString& color) :
	_text(text),
	_color(color)
{
}

TextDisplay::TextDisplay(DisplayEventType displayType)
{
	static Parameters* params = Parameters::get();
	_text = params->eventFormat(displayType);
	_color = params->eventColor(displayType);
}

QString
TextDisplay::richText() const
{
	return "<span style=\"color:" + _color + ";\">" + text() + "</span>";
}

QString
TextDisplay::text() const
{
	QString text;
	text.reserve(int(_text.length() * 1.1));
	for (int i = 0, len = _text.length(); i < len; ++i)
	{
		if (_text[i] == '%' && ++i < len)
		{
			if (_text[i] == 'n')
				text += _nick;
			else if (_text[i] == 'u')
				text += _user;
			else if (_text[i] == 'h')
				text += _host;
			else if (_text[i] == 'c')
				text += _channel;
			else if (_text[i] == 'm')
				text += _modes;
			else if (_text[i] == 'a')
				text += _arg;
			else if (_text[i] == 't')
				text += _target;
			else if (_text[i] == 'r')
				text += _raw;
			else
				text += _text[i];
		}
		else
			text += _text[i];
	}
	return text;
}

QString
TextDisplay::link_nick(const QString& nick, const QString& tag)
{
	return QString("<a style=\"color: " + _color +
				   "; text-decoration: none;\" href=\"#u_" +
				   tag + "\">" + nick + "</a>");
}

QString
TextDisplay::link_nick(const QString& nick)
{
	return link_nick(nick, nick);
}
