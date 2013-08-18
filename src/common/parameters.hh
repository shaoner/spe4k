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
 * \file parameters.hh
 * \author shaoner
 * \brief Handle global parameters
 */
#ifndef PARAMETERS_HH
# define PARAMETERS_HH

# include <QColor>

# include "common.hh"
# include "default-parameters.hh"

class QSettings;

/// Settings filenames
# define SETTINGS_MAIN "settings"

/*!
 * \enum DisplayEventType
 * \brief Event types
 */
enum DisplayEventType
{
	DISPLAY_JOIN = 0x0,
	DISPLAY_PART = 0x1,
	DISPLAY_PART_REASON = 0x2,
	DISPLAY_KICK = 0x3,
	DISPLAY_MODE = 0x4,
	DISPLAY_NICK = 0x5,
	DISPLAY_QUIT = 0x6,
	DISPLAY_TOPIC_CHANGE = 0x7,
	DISPLAY_TOPIC = 0x8,
	DISPLAY_TOPIC_INFO = 0x9,
	DISPLAY_NOTOPIC = 0xa,
	DISPLAY_RAW = 0xb,
	DISPLAY_INFO = 0xc,
	DISPLAY_MESSAGE = 0xd,
	DISPLAY_ACTION = 0xe,
	DISPLAY_NOTICE = 0xf
};

/*!
 * \enum LightLevel
 * \brief Tree items depends on these levels
 */
enum LightLevel
{
	LIGHT_LVL_NONE = 0,
	LIGHT_LVL_EVENT = 1,
	LIGHT_LVL_MESSAGE = 2,
	LIGHT_LVL_HIGHLIGHT = 3
};

class Parameters;

/*!
 * \class EventScheme
 * \brief An event template to fill and display
 */
class EventScheme
{
public:
	/// Ctor
	EventScheme(const QString& key, QSettings* settings);
private:
	friend class Parameters;
	QString _keyFormat;
	QString _format;
	QString _keyColor;
	QString _color;
	QString _keyEnabled;
	bool _enabled;
};

/*!
 * \class Parameters
 * \brief This singleton is useful to set and get global settings
 */
class Parameters
{
public:
	static Parameters* get();
	static void reset();
public:
	const QColor& levelColor(LightLevel level) const;
	const QString& ircColor(int code) const;
	void set_ircColor(int code, const QString& color);
	QString eventFormat(DisplayEventType event) const;
	void set_eventFormat(DisplayEventType event, const QString& format);
	QString eventColor(DisplayEventType event) const;
	void set_eventColor(DisplayEventType event, const QString& color);
	bool eventEnabled(DisplayEventType event) const;
	void set_eventEnabled(DisplayEventType, bool enabled);
	void reset_event(DisplayEventType event);
	/// Simple aliases to get and set options
	template <typename T> T get(const QString& key) const;
	template <typename T> void set(const QString& key, T value);
private:
	void init_default();
	template <typename T> void init(const QString& key, const T& value);
private:
	/// Ctor
	Parameters();
	/// Dtor
	~Parameters();
private:
	static Parameters* _instance;
	static const QString _eventName[];
	static const QString _eventDefaultFormat[];
	static const QString _eventDefaultColor[];
private:
	QSettings* _settings;
	QList<EventScheme> _eventScheme;
	QList<QColor> _levelColor;
	QList<QString> _ircColor;
};

# include "parameters.hxx"

#endif /* !PARAMETERS_HH */
