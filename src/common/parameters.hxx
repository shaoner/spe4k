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
 * \file parameters.hxx
 * \author shaoner
 * \brief Handle global parameters
 */
#ifndef PARAMETERS_HXX
# define PARAMETERS_HXX

# include "parameters.hh"

# include <QSettings>
# include "text-formatter.hh"

template <typename T> inline T
Parameters::get(const QString& key) const
{
	return _settings->value(key).value<T>();
}

template <typename T> inline void
Parameters::set(const QString& key, T value)
{
	_settings->setValue(key, value);
}

inline const QString&
Parameters::ircColor(int code) const
{
	return _ircColor[code % 15];
}

inline void
Parameters::set_ircColor(int code, const QString& color)
{
	_ircColor[code] = color;
	set("Color/" + QString((code < 10 ? "0" : "") + QString::number(code)), color);
}

inline const QColor&
Parameters::levelColor(LightLevel level) const
{
	return _levelColor[(int)level];
}

inline QString
Parameters::eventFormat(DisplayEventType event) const
{
	return _eventScheme[event]._format;
}

inline void
Parameters::set_eventFormat(DisplayEventType event, const QString& format)
{
	_eventScheme[event]._format = format;
	set(_eventScheme[event]._keyFormat, format);
}

inline QString
Parameters::eventColor(DisplayEventType event) const
{
	return _eventScheme[event]._color;
}

inline void
Parameters::set_eventColor(DisplayEventType event, const QString& color)
{
	_eventScheme[event]._color = color;
	set(_eventScheme[event]._keyColor, color);
}

inline bool
Parameters::eventEnabled(DisplayEventType event) const
{
	return _eventScheme[event]._enabled;
}

inline void
Parameters::set_eventEnabled(DisplayEventType event, bool enabled)
{
	_eventScheme[event]._enabled = enabled;
	set(_eventScheme[event]._keyEnabled, enabled);
}

inline void
Parameters::reset_event(DisplayEventType event)
{
	set_eventFormat(event, _eventDefaultFormat[event]);
	set_eventColor(event, _eventDefaultColor[event]);
	set_eventEnabled(event, true);
}

template <typename T> inline void
Parameters::init(const QString& key, const T& value)
{
	if (!_settings->contains(key))
		_settings->setValue(key, value);
}

#endif /* !PARAMETERS_HXX */
