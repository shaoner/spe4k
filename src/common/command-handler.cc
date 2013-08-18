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
 * \file command-handler.cc
 * \author shaoner
 * \brief Handles internal commands
 */
#include "command-handler.hh"

#include <QStringList>
#include "command.hh"

CommandHandler* CommandHandler::_instance = NULL;

CommandHandler*
CommandHandler::get()
{
	if (!_instance)
		_instance = new CommandHandler();
	return _instance;
}

void
CommandHandler::reset()
{
	delete _instance;
	_instance = NULL;
}

void
CommandHandler::exec(Window* win, const QString& commandName, QStringList& args)
{
	if (_internCmdMap.contains(commandName))
	{
		if (!_internCmdMap[commandName]->process(win, args))
			_internCmdMap[commandName]->help(win);
	}
	else
	{
		// If the command is unknown, we send it directly to the server
		if (args.count() > 0)
			win->session.write(commandName + " " + args.join(" "));
		else
			win->session.write(commandName);
	}
}

CommandHandler::CommandHandler()
{
	_internCmdMap.insert("ME", new CommandAction());
	_internCmdMap.insert("ADMIN", new CommandAdmin());
	_internCmdMap.insert("AWAY", new CommandAway());
	_internCmdMap.insert("CLEAR", new CommandClear());
	_internCmdMap.insert("CLEARTOPIC", new CommandCleartopic());
	_internCmdMap.insert("CONNECT", new CommandConnect());
	_internCmdMap.insert("DIE", new CommandDie());
	_internCmdMap.insert("ECHO", new CommandEcho());
	_internCmdMap.insert("HELP", new CommandHelp(_internCmdMap));
	_internCmdMap.insert("HOP", new CommandHop());
	_internCmdMap.insert("INFO", new CommandInfo());
	_internCmdMap.insert("INVITE", new CommandInvite());
	_internCmdMap.insert("JOIN", new CommandJoin());
	_internCmdMap.insert("KICK", new CommandKick());
	_internCmdMap.insert("KILL", new CommandKill());
	_internCmdMap.insert("KNOCK", new CommandKnock());
	_internCmdMap.insert("LINKS", new CommandLinks());
	_internCmdMap.insert("LIST", new CommandList());
	_internCmdMap.insert("LUSERS", new CommandLusers());
	_internCmdMap.insert("MODE", new CommandMode());
	_internCmdMap.insert("MOTD", new CommandMotd());
	_internCmdMap.insert("MSG", new CommandMsg());
	_internCmdMap.insert("NAMES", new CommandNames());
	_internCmdMap.insert("NICK", new CommandNick());
	_internCmdMap.insert("NOTICE", new CommandNotice());
	_internCmdMap.insert("OPER", new CommandOper());
	_internCmdMap.insert("PART", new CommandPart());
	_internCmdMap.insert("QUIT", new CommandQuit());
	_internCmdMap.insert("QUOTE", new CommandQuote());
	_internCmdMap.insert("RECONNECT", new CommandReconnect());
	_internCmdMap.insert("REHASH", new CommandRehash());
	_internCmdMap.insert("RESTART", new CommandRestart());
	_internCmdMap.insert("SERVER", new CommandServer());
	_internCmdMap.insert("SQUIT", new CommandSquit());
	_internCmdMap.insert("STATS", new CommandStats());
	_internCmdMap.insert("TIME", new CommandTime());
	_internCmdMap.insert("TOPIC", new CommandTopic());
	_internCmdMap.insert("TRACE", new CommandTrace());
	_internCmdMap.insert("VERSION", new CommandVersion());
	_internCmdMap.insert("WALLOPS", new CommandWallops());
	_internCmdMap.insert("WHO", new CommandWho());
	_internCmdMap.insert("WHOIS", new CommandWhois());
	_internCmdMap.insert("WHOWAS", new CommandWhowas());
}

CommandHandler::~CommandHandler()
{
    foreach(Command* cmd, _internCmdMap)
    {
        delete cmd;
    }
}
