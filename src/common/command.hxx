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
 * \file command.hxx
 * \author shaoner
 * \brief All internal commands
 */

#ifndef COMMAND_HXX
# define COMMAND_HXX

# include "command.hh"

# include <QStringList>
# include <session.hh>

# include "widget/window/window.hh"
# include "widget/window/status-window.hh"

inline
Command::Command(const QString& name,
				 const QString& usage,
				 const QString& description,
				 int minArgs) :
	_name(name),
	_usage(usage),
	_description(description),
	_minArgs(minArgs)
{
}

inline
Command::~Command()
{
}

inline bool
Command::process(Window* win, QStringList& args)
{
	if (args.count() >= _minArgs)
	{
		if (args.count() > 0)
			win->session.write(_name + " " + args.join(" "));
		else
			win->session.write(_name);
		return true;
	}
	return false;
}

inline void
Command::help(Window* win)
{
	win->display_info("Usage: " + _name + " " + _usage);
	win->display_info(_description);
}

inline
CommandAction::CommandAction() :
	Command("ME", "<message>",
			"Send an action message to the current window (except status)")
{
}

inline bool
CommandAction::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;

	if (argSize > 0)
	{
		const QString& msg = args.join(" ");
		win->display_action(session.nickname(), session.nickname(), msg);
		session.action(win->name(), msg);
		return true;
	}
	return false;
}

inline bool
CommandAction::process(StatusWindow* win, QStringList& args)
{
	Q_UNUSED(win);
	Q_UNUSED(args);
	// Cannot send action message directly on the status window
	return false;
}

inline
CommandAdmin::CommandAdmin() :
	Command("ADMIN", "[<server>]",
			"Get the name of the administrators of the given server", 0)
{
}

inline
CommandAway::CommandAway() :
	Command("AWAY", "[<message>]",
			"Set or reset auto reply")
{
}

inline bool
CommandAway::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
		win->session.away(args.join(" "));
	else
		win->session.away();
	return true;
}

inline
CommandClear::CommandClear() :
	Command("CLEAR", "", "Clear the current screen")
{
}

inline bool
CommandClear::process(Window* win, QStringList& args)
{
	Q_UNUSED(args);
	win->clear();
	return true;
}

inline
CommandCleartopic::CommandCleartopic() :
	Command("CLEARTOPIC", "[<channel>]",
			"Clear the topic of <channel>")
{
}

inline bool
CommandCleartopic::process(Window* win, QStringList& args)
{
	if ((args.count() > 0) && (win->session.is_channel(args[0])))
		win->session.cleartopic(args[0]);
	else if (win->session.is_channel(win->name()))
		win->session.cleartopic(win->name());
	else
		return false;
	return true;
}


inline
CommandConnect::CommandConnect() :
	Command("CONNECT", "<server> <port> [<remote>]",
			"Make a server to try to establish a new connection to another server", 2)
{
}

inline
CommandDie::CommandDie() :
	Command("DIE", "",
			"Shutdown the server", 0)
{
}

inline
CommandEcho::CommandEcho() :
	Command("ECHO", "[<text>]", "Display text on the current window")
{
}

inline bool
CommandEcho::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
		win->display(args.join(" "));
	else
		win->display("");
	return true;
}

inline
CommandHelp::CommandHelp(const QHash<QString, Command*>& cmdMap) :
	Command("HELP", "[<command>]",
			"List all commands or display help for specific commands"),
	_cmdMap(cmdMap)
{
}

inline bool
CommandHelp::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
	{
		QString cmd = args[0].toUpper();
		if (_cmdMap.contains(cmd))
		{
			_cmdMap[cmd]->help(win);
			return true;
		}
	}
	else
	{
		win->display_info("Available internal commands:");
		QStringList keys = _cmdMap.keys();
		keys.sort();
		foreach (QString cmd, keys)
		{
			win->display_info("-> " + cmd);
		}
		return true;
	}
	return false;
}


inline
CommandHop::CommandHop() :
	Command("HOP", "[<channel>{,<channel>}]", "Leave and enter channel")
{
}

inline bool
CommandHop::process(Window* win, QStringList& args)
{
    if ((args.count() > 0) && (win->session.is_channel(args[0])))
	{
		win->session.part(win->name());
		win->session.join(win->name());
		return true;
	}
    else if (win->session.is_channel(win->name()))
    {
        win->session.part(win->name());
        win->session.join(win->name());
        return true;
    }
	return false;
}

inline
CommandInfo::CommandInfo() :
	Command("INFO", "[<server>]",
			"Get information describing the server", 0)
{
}

inline
CommandInvite::CommandInvite() :
	Command("INVITE", "<nick> [<channel>]",
			"Send an invitation to a user to join a channel")
{
}

inline bool
CommandInvite::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
	{
		if (args.count() > 1)
			win->session.invite(args[0], args[1]);
		else if (win->session.is_channel(win->name()))
			win->session.invite(args[0], win->name());
		else
			return false;
		return true;
	}
	return false;
}

inline
CommandJoin::CommandJoin() :
	Command("JOIN", "[<channel>{,<channel>} [<key>{,<key>}]]",
			"Enter channels")
{
}

inline bool
CommandJoin::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;

	if ((argSize == 0) && (session.is_channel(win->name())))
	{
		session.join(win->name());
		return true;
	}
 	else if ((argSize > 0) && (session.is_channel(args[0])))
	{
		if (argSize > 1)
			session.join(args[0], args[1]);
		else
			session.join(args[0]);
		return true;
	}
	return false;
}

inline
CommandKick::CommandKick() :
	Command("KICK", "[<channel>] <nick>{,<nick>} [<reason>]",
			"For a user to leave a channel")
{
}

inline bool
CommandKick::process(Window* win, QStringList& args)
{
	irc::Session& session = win->session;
	int argSize = args.count();
	if ((argSize > 1) && (session.is_channel(args[0])))
	{
		if (argSize > 2)
		{
			const QString& channel = args.takeFirst();
			const QString& user = args.takeFirst();
			session.kick(channel, user, args.join(" "));
		}
		else
			session.kick(args[0], args[1]);
		return true;
	}
	else if ((argSize > 0) && (session.is_channel(win->name())))
	{
		if (argSize > 1)
		{
			const QString& user = args.takeFirst();
			session.kick(win->name(), user, args.join(" "));
		}
		else
			session.kick(win->name(), args[0]);
		return true;
	}
	return false;
}

inline
CommandKill::CommandKill() :
	Command("KILL", "<nick> <reason>",
			"Force a user to leave the server", 2)
{
}

inline
CommandKnock::CommandKnock() :
	Command("KNOCK", "<channel> [<reason>]",
			"Ask an invitation to enter a channel", 1)
{
}

inline
CommandLinks::CommandLinks() :
	Command("LINKS", "[<server> [<mask>]]",
			"List servers which are linked together", 0)
{
}

inline
CommandList::CommandList() :
	Command("LIST", "[<channel> [<server>]]",
			"List channels and their topic", 0)
{
}

inline
CommandLusers::CommandLusers() :
	Command("LUSERS", "[<mask> [<server>]]",
			"Get statistics about users on the network", 0)
{
}

inline
CommandMode::CommandMode() :
	Command("MODE", "[<target>] [<modes> {<mode-parameters>}]",
			"Sets mode to the target, that can be a channel or yourself")
{
}

inline bool
CommandMode::process(Window* win, QStringList& args)
{
	int argSize = args.size();
	irc::Session& session = win->session;

	if (argSize == 0)
	{
		if (session.is_channel(win->name()))
			session.mode(win->name());
		else
			session.mode(session.nickname());
		return true;
	}
	else if (argSize > 0)
	{
		if ((args[0].startsWith('+') || (args[0].startsWith('-'))))
		{
			if (session.is_channel(win->name()))
				session.mode(win->name(), args.join(" "));
			else
				session.mode(session.nickname(), args.join(" "));
		}
		else
		{
			if (argSize > 1)
			{
				const QString& target = args.takeFirst();
				session.mode(target, args.join(" "));
			}
			else
				session.mode(args[0]);
		}
		return true;
	}
	return false;
}

inline
CommandMotd::CommandMotd() :
	Command("MOTD", "[<server>]",
			"Get the Message Of The Day", 0)
{
}

inline
CommandMsg::CommandMsg() :
	Command("MSG", "<target> <message>",
			"Send a raw message to the target (a nick or a channel)")
{
}

inline bool
CommandMsg::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;

	if (argSize > 1)
	{
		const QString& target = args.takeFirst();
		session.privmsg(target, args.join(" "));
		return true;
	}
	return false;
}

inline
CommandNames::CommandNames() :
	Command("NAMES", "[<channel>{,<channel>}]",
			"List nicknames for <channel>", 0)
{
}

inline
CommandNick::CommandNick() :
	Command("NICK", "<new-nick>", "Change your nickname")
{
}

inline bool
CommandNick::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
	{
		// Only care about args[0], and ignore other arguments
		win->session.change_nickname(args[0]);
		return true;
	}
	return false;
}

inline
CommandNotice::CommandNotice() :
	Command("NOTICE", "<target>{,<target>} <message>",
			"Send a notice message to the target")
{
}

inline bool
CommandNotice::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;

	if (argSize > 1)
	{
		const QString& target = args.takeFirst();
		const QString& message = args.join(" ");
		win->display_info("-> " + target + ": " + message);
		session.notice(target, message);
		return true;
	}
	return false;
}

inline
CommandOper::CommandOper() :
	Command("OPER", "<login> <password>",
			"Obtain IRC Operator privileges", 2)
{
}

inline
CommandPart::CommandPart() :
	Command("PART", "[<channel>{,<channel>} [<reason>]]",
			"Leave channels")
{
}

inline bool
CommandPart::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;

	if ((argSize == 0) && (session.is_channel(win->name())))
	{
		session.part(win->name());
		return true;
	}
	else if ((args.count() > 0) && (session.is_channel(args[0])))
	{
		if (argSize > 1)
		{
			const QString& channel = args.takeFirst();
			session.part(channel, args.join(" "));
		}
		else
			session.part(args[0]);
		return true;
	}
	return false;
}

inline
CommandQuit::CommandQuit() :
	Command("QUIT", "[<reason>]",
			"Leave the server", 0)
{
}

inline
CommandQuote::CommandQuote() :
	Command("QUOTE", "<command> [<command-args>]",
			"Send raw irc command to the server", 1)

{
}

inline bool
CommandQuote::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
	{
		win->session.write(args.join(" "));
		return true;
	}
	return false;
}

inline
CommandReconnect::CommandReconnect() :
	Command("RECONNECT", "",
			"Reconnection to the server")
{
}

inline bool
CommandReconnect::process(Window* win, QStringList& args)
{
	Q_UNUSED(args);
	if (win->session.is_connected())
		win->session.stop();
	win->session.start();
	return true;
}

inline
CommandRehash::CommandRehash() :
	Command("REHASH", "",
			"Reload server configuration", 0)
{
}

inline
CommandRestart::CommandRestart() :
	Command("RESTART", "",
			"Restart the server", 0)
{
}

inline
CommandServer::CommandServer() :
	Command("SERVER", "hostname[:port] [password]",
			"Connect to the server at the given address")
{
}

inline bool
CommandServer::process(Window* win, QStringList& args)
{
	int argSize = args.count();
	irc::Session& session = win->session;
	if (argSize > 0)
	{
		quint16 port = 6667;
		QString password = "";
		QStringList address = args[0].split(':');
		if (address.count() == 2)
		{
			bool ok;
			quint16 tmpPort;
			if ((tmpPort = address[1].toUShort(&ok)) && ok)
				port = tmpPort;
		}
		if (argSize > 1)
			password = args[1];
		if (session.is_connected())
			session.stop();
		session.start(args[0], port, password);
		return true;
	}
	return false;
}

inline
CommandSquit::CommandSquit() :
	Command("SQUIT", "<server> <comment>",
			"Disconnect a server link", 2)
{
}

inline
CommandStats::CommandStats() :
	Command("STATS", "[<query> [<server>]]",
			"Query some specific statistics", 0)
{
}

inline
CommandTime::CommandTime() :
	Command("TIME", "[<server>]",
			"Get local time from the server", 0)
{
}

inline
CommandTopic::CommandTopic() :
	Command("TOPIC", "[<channel>] [<subject>]",
			"Set or get the topic for <channel>")
{
}

inline bool
CommandTopic::process(Window* win, QStringList& args)
{
	irc::Session& session = win->session;
	int argSize = args.count();

	if ((argSize == 0) && (session.is_channel(win->name())))
	{
		session.topic(win->name());
		return true;
	}
	else if (argSize > 0)
	{
		if (session.is_channel(args[0]))
		{
			if (argSize > 1)
			{
				const QString& channel = args.takeFirst();
				session.topic(channel, args.join(" "));
			}
			else
				session.topic(args[0]);
		}
		else if (session.is_channel(win->name()))
			session.topic(win->name(), args.join(" "));
		return true;
	}
	return false;
}

inline
CommandTrace::CommandTrace() :
	Command("TRACE", "[<server>]",
			"Get route to the specific server", 0)
{
}

inline
CommandVersion::CommandVersion() :
	Command("VERSION", "[<server>]",
			"Get the version of the server", 0)
{
}

inline
CommandWallops::CommandWallops() :
	Command("WALLOPS", "<message>",
			"Send a message to all users")
{
}

inline bool
CommandWallops::process(Window* win, QStringList& args)
{
	if (args.count() > 0)
	{
		win->session.wallops(args.join(" "));
	}
	return false;
}

inline
CommandWho::CommandWho() :
	Command("WHO", "[<mask>]",
			"Search for a mask with search options that can be a channel, a nickname, a server, etc.", 0)
{
}

inline
CommandWhois::CommandWhois() :
	Command("WHOIS", "<nick>{,<nick>}",
			"Ask for information about nick to the server", 1)
{
}

inline
CommandWhowas::CommandWhowas() :
	Command("WHOWAS", "<nick>{,<nick>} [count [target]]",
			"Ask for information about nick which may no longer exist", 1)
{
}

#endif /* !COMMAND_HXX */
