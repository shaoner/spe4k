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
 * \file parameters.cc
 * \author shaoner
 * \brief Handle global parameters
 */
#include "parameters.hh"

#include <QSettings>
#include "text-formatter.hh"

EventScheme::EventScheme(const QString& key, QSettings* settings) :
	_keyFormat("Format/" + key),
	_format(settings->value(_keyFormat).toString()),
	_keyColor("Color/" + key),
	_color(settings->value(_keyColor).toString()),
	_keyEnabled("Enabled/" + key),
	_enabled(settings->value(_keyEnabled).toBool())
{
}

Parameters* Parameters::_instance = NULL;

const QString Parameters::_eventName[PRM_NUMBER_EVT] =
{
	PRM_EVT_JOIN,
	PRM_EVT_PART,
	PRM_EVT_PART_REASON,
	PRM_EVT_KICK,
	PRM_EVT_MODE,
	PRM_EVT_NICK,
	PRM_EVT_QUIT,
	PRM_EVT_TOPIC_CHANGE,
	PRM_EVT_TOPIC,
	PRM_EVT_TOPIC_INFO,
	PRM_EVT_NOTOPIC,
	PRM_EVT_RAW,
	PRM_EVT_INFO,
	PRM_EVT_MESSAGE,
	PRM_EVT_ACTION,
	PRM_EVT_NOTICE
};

const QString Parameters::_eventDefaultFormat[PRM_NUMBER_EVT] =
{
	DEF_FORMAT_JOIN,
	DEF_FORMAT_PART,
	DEF_FORMAT_PART_REASON,
	DEF_FORMAT_KICK,
	DEF_FORMAT_MODE,
	DEF_FORMAT_NICK,
	DEF_FORMAT_QUIT,
	DEF_FORMAT_TOPIC_CHANGE,
	DEF_FORMAT_TOPIC,
	DEF_FORMAT_TOPIC_INFO,
	DEF_FORMAT_NOTOPIC,
	DEF_FORMAT_RAW,
	DEF_FORMAT_INFO,
	DEF_FORMAT_MESSAGE,
	DEF_FORMAT_ACTION,
	DEF_FORMAT_NOTICE
};

const QString Parameters::_eventDefaultColor[PRM_NUMBER_EVT] =
{
	DEF_COLOR_JOIN,
	DEF_COLOR_PART,
	DEF_COLOR_PART_REASON,
	DEF_COLOR_KICK,
	DEF_COLOR_MODE,
	DEF_COLOR_NICK,
	DEF_COLOR_QUIT,
	DEF_COLOR_TOPIC_CHANGE,
	DEF_COLOR_TOPIC,
	DEF_COLOR_TOPIC_INFO,
	DEF_COLOR_NOTOPIC,
	DEF_COLOR_RAW,
	DEF_COLOR_INFO,
	DEF_COLOR_MESSAGE,
	DEF_COLOR_ACTION,
	DEF_COLOR_NOTICE
};

Parameters*
Parameters::get()
{
	if (!_instance)
		_instance = new Parameters();
	return _instance;
}

void
Parameters::reset()
{
	delete _instance;
	_instance = NULL;
}

void
Parameters::init_default()
{
	// General
	init("rejoin_onkick", DEF_COMMON_REJOIN_ONKICK);
	init("reconnect_onkill", DEF_COMMON_RECONNECT_ONKILL);
	init("closewins_ondisconnect", DEF_COMMON_CLOSEWINS_ONDISCONNECT);
	init("rejoin_onconnect", DEF_COMMON_REJOIN_ONCONNECT);
	init("join_oninvite", DEF_COMMON_JOIN_ONINVITE);
	init("closewin_onpart", DEF_COMMON_CLOSEWIN_ONPART);
	init("reconnect_delay", DEF_COMMON_RECONNECT_DELAY);
	init("reconnect_retry", DEF_COMMON_RECONNECT_RETRY);
	// Display
	init("Display/timestamp", DEF_COMMON_TIMESTAMP);
	init("Display/timestamp_format", DEF_COMMON_TIMESTAMP_FORMAT);
	// Format / Color
	for (int i = 0; i < PRM_NUMBER_EVT; ++i)
	{
		init("Format/" + _eventName[i], _eventDefaultFormat[i]);
		init("Color/" + _eventName[i], _eventDefaultColor[i]);
		init("Enabled/" + _eventName[i], true);
	}
	init("Color/00", DEF_COLOR_00);
	init("Color/01", DEF_COLOR_01);
	init("Color/02", DEF_COLOR_02);
	init("Color/03", DEF_COLOR_03);
	init("Color/04", DEF_COLOR_04);
	init("Color/05", DEF_COLOR_05);
	init("Color/06", DEF_COLOR_06);
	init("Color/07", DEF_COLOR_07);
	init("Color/08", DEF_COLOR_08);
	init("Color/09", DEF_COLOR_09);
	init("Color/10", DEF_COLOR_10);
	init("Color/11", DEF_COLOR_11);
	init("Color/12", DEF_COLOR_12);
	init("Color/13", DEF_COLOR_13);
	init("Color/14", DEF_COLOR_14);
	init("Color/15", DEF_COLOR_15);
	init("Color/light_event", DEF_COLOR_LVL_EVENT);
	init("Color/light_message", DEF_COLOR_LVL_MESSAGE);
	init("Color/light_highlight", DEF_COLOR_LVL_HIGHLIGHT);
	_settings->sync();

}


Parameters::Parameters() :
	_settings(new QSettings(ORG_NAME, SETTINGS_MAIN))
{
	// Initialize each option to a default value if the option is empty
	// And save the settings
	init_default();
	_settings->beginGroup("Color");
	_ircColor.append(get<QString>("00"));
	_ircColor.append(get<QString>("01"));
	_ircColor.append(get<QString>("02"));
	_ircColor.append(get<QString>("03"));
	_ircColor.append(get<QString>("04"));
	_ircColor.append(get<QString>("05"));
	_ircColor.append(get<QString>("06"));
	_ircColor.append(get<QString>("07"));
	_ircColor.append(get<QString>("08"));
	_ircColor.append(get<QString>("09"));
	_ircColor.append(get<QString>("10"));
	_ircColor.append(get<QString>("11"));
	_ircColor.append(get<QString>("12"));
	_ircColor.append(get<QString>("13"));
	_ircColor.append(get<QString>("14"));
	_ircColor.append(get<QString>("15"));
	// This is for level none
	_levelColor.append(QColor(Qt::black));
	_levelColor.append(get<QColor>("light_event"));
	_levelColor.append(get<QColor>("light_message"));
	_levelColor.append(get<QColor>("light_highlight"));
	_settings->endGroup();
	for (int i = 0; i < PRM_NUMBER_EVT; ++i)
	{
		_eventScheme.append(EventScheme(_eventName[i], _settings));
	}
}

Parameters::~Parameters()
{
	delete _settings;
}
