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
 * \file default-parameters.hh
 * \author shaoner
 * \brief Default values for global settings
 */
#ifndef DEFAULT_PARAMETERS_HH
# define DEFAULT_PARAMETERS_HH

/// Event names
# define PRM_NUMBER_EVT 16
# define PRM_EVT_JOIN "join"
# define PRM_EVT_PART "part"
# define PRM_EVT_PART_REASON "part_reason"
# define PRM_EVT_KICK "kick"
# define PRM_EVT_MODE "mode"
# define PRM_EVT_QUIT "quit"
# define PRM_EVT_NICK "nick"
# define PRM_EVT_TOPIC_CHANGE "topic_change"
# define PRM_EVT_TOPIC "topic"
# define PRM_EVT_TOPIC_INFO "topic_info"
# define PRM_EVT_NOTOPIC "notopic"
# define PRM_EVT_RAW "raw"
# define PRM_EVT_INFO "info"
# define PRM_EVT_MESSAGE "message"
# define PRM_EVT_ACTION "action"
# define PRM_EVT_NOTICE "notice"

# define PRM_FORMAT(Event) "Format/" PRM_EVT_ ## Event
# define PRM_COLOR(Event) "Color/" PRM_EVT_ ## Event

/// Default values
# define DEF_COMMON_REJOIN_ONKICK true
# define DEF_COMMON_RECONNECT_ONKILL true
# define DEF_COMMON_CLOSEWINS_ONDISCONNECT false
# define DEF_COMMON_REJOIN_ONCONNECT true
# define DEF_COMMON_JOIN_ONINVITE false
# define DEF_COMMON_CLOSEWIN_ONPART false
# define DEF_COMMON_RECONNECT_DELAY 5
# define DEF_COMMON_RECONNECT_RETRY 15
# define DEF_COMMON_TIMESTAMP true
# define DEF_COMMON_TIMESTAMP_FORMAT "[HH:mm:ss]"

# define DEF_FORMAT_JOIN "* %n (%u@%h) has joined %c"
# define DEF_FORMAT_PART "* %n (%u@%h) has left %c"
# define DEF_FORMAT_PART_REASON "* %n (%u@%h) has left %c (%r)"
# define DEF_FORMAT_KICK "* %t has been kicked on %c by %n (%r)"
# define DEF_FORMAT_MODE "* %n sets mode [ %m %a ] on %c"
# define DEF_FORMAT_QUIT "* %n (%u@%h) has quit server (%r)"
# define DEF_FORMAT_NICK "* %n has changed his nick for %t"
# define DEF_FORMAT_TOPIC_CHANGE "* %n sets topic: %r"
# define DEF_FORMAT_TOPIC "* Topic on %c: %r"
# define DEF_FORMAT_NOTOPIC "* No topic set"
# define DEF_FORMAT_TOPIC_INFO "* Topic set by %n on %a"
# define DEF_FORMAT_RAW "* %r"
# define DEF_FORMAT_INFO "* %r"
# define DEF_FORMAT_MESSAGE "&lt;%n&gt; %r"
# define DEF_FORMAT_ACTION "* %n %r"
# define DEF_FORMAT_NOTICE "(%n -&gt; %t) %r"

# define DEF_COLOR_JOIN "#698B69"
# define DEF_COLOR_PART "#c4c4c4"
# define DEF_COLOR_PART_REASON "#c4c4c4"
# define DEF_COLOR_KICK "#ff0000"
# define DEF_COLOR_MODE "#0000ff"
# define DEF_COLOR_QUIT "#cccccc"
# define DEF_COLOR_NICK "#ccccff"
# define DEF_COLOR_TOPIC_CHANGE "#ffcccc"
# define DEF_COLOR_TOPIC "#ffcccc"
# define DEF_COLOR_TOPIC_INFO "#ffcccc"
# define DEF_COLOR_NOTOPIC "#ffcccc"
# define DEF_COLOR_RAW "#000000"
# define DEF_COLOR_INFO "#3a3a3a"
# define DEF_COLOR_MESSAGE "#000000"
# define DEF_COLOR_ACTION "#000000"
# define DEF_COLOR_NOTICE "#000000"

# define DEF_COLOR_00 "#ffffff"
# define DEF_COLOR_01 "#000000"
# define DEF_COLOR_02 "#000080"
# define DEF_COLOR_03 "#308014"
# define DEF_COLOR_04 "#ff0000"
# define DEF_COLOR_05 "#993300"
# define DEF_COLOR_06 "#7f00ff"
# define DEF_COLOR_07 "#ff7f00"
# define DEF_COLOR_08 "#ffff00"
# define DEF_COLOR_09 "#00ff00"
# define DEF_COLOR_10 "#008080"
# define DEF_COLOR_11 "#00e5ee"
# define DEF_COLOR_12 "#3299cc"
# define DEF_COLOR_13 "#ff00ff"
# define DEF_COLOR_14 "#545454"
# define DEF_COLOR_15 "#a8a8a8"

# define DEF_COLOR_LVL_EVENT "#000080"
# define DEF_COLOR_LVL_MESSAGE "#ff0000"
# define DEF_COLOR_LVL_HIGHLIGHT "#00e5ee"

#endif /* !DEFAULT_PARAMETERS_HH */
