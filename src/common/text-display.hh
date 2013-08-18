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
 * \file text-display.hh
 * \author shaoner
 * \brief Formatting text for internal windows
 */
#ifndef TEXT_DISPLAY_HH
# define TEXT_DISPLAY_HH

# include <QString>

# include "common/parameters.hh"

# define CODE_PREFIX 0x80

/*!
 * \class TextDisplay
 * \brief Text to format
 */
class TextDisplay
{
/*
 * Format
 *
 * %c channel
 * %n nickname
 * %u user
 * %h hostname
 * %r reason
 * %m mode
 * %a arg
 * %t target
 * %s simple string
 * %i irc string (parse colors, etc)
 */
public:
	/// Ctor
    explicit TextDisplay(DisplayEventType displayType);
    TextDisplay(const QString& text, const QString& color);
public:
	/// Push arguments to parameters %
	void push_nick(const QString& nick);
	void push_nick(const QString& nick, const QString& tag);
	void push_user(const QString& user);
	void push_host(const QString& host);
	void push_modes(const QString& modes);
    void push_arg(const QString& arg);
	void push_target(const QString& target);
	void push_channel(const QString& channel);
    void push_raw(const QString& raw);
    void push_ircraw(const QString& raw);
	/// Final text
	QString richText() const;
	QString text() const;
private:
	/// Make a clickable nickname
	QString link_nick(const QString& nick, const QString& tag);
	QString link_nick(const QString& nick);
private:
	QString _text;
	QString _color;
	QString _nick;
	QString _tag;
	QString _user;
	QString _host;
	QString _modes;
	QString _arg;
	QString _target;
	QString _channel;
	QString _raw;
};

# include "text-display.hxx"

#endif /* !TEXT_DISPLAY_HH */
