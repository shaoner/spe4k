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
#ifndef TEXT_DISPLAY_HXX
# define TEXT_DISPLAY_HXX

# include "text-formatter.hh"

inline void
TextDisplay::push_nick(const QString& nick)
{
	_nick = nick;
	_tag = nick;
}

inline void
TextDisplay::push_nick(const QString& nick, const QString& tag)
{
	_nick = nick;
	_tag = tag;
}

inline void
TextDisplay::push_user(const QString& user)
{
	_user = user;
}

inline void
TextDisplay::push_host(const QString& host)
{
	_host = host;
}

inline void
TextDisplay::push_modes(const QString& modes)
{
	_modes = modes;
}

inline void
TextDisplay::push_arg(const QString& arg)
{
	_arg = arg;
}

inline void
TextDisplay::push_target(const QString& target)
{
	_target = target;
}

inline void
TextDisplay::push_channel(const QString& channel)
{
	_channel = channel;
}

inline void
TextDisplay::push_raw(const QString& raw)
{
	_raw = TextFormatter::html_escape(raw);
}

inline void
TextDisplay::push_ircraw(const QString& raw)
{
	_raw = TextFormatter::irc_to_html(raw);
}

#endif /* !TEXT_DISPLAY_HXX */
