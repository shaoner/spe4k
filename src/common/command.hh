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
 * \file command.hh
 * \author shaoner
 * \brief All internal commands
 */
#ifndef COMMAND_HH
# define COMMAND_HH

# include <QString>

class QStringList;
class Window;
class StatusWindow;

/*!
 * \class Command
 * \brief Command interface
 */
class Command
{
public:
	/// Ctor
	Command(const QString& name,
			const QString& usage = "",
			const QString& description = "",
			int minArgs = 0);
	/// Dtor
	virtual ~Command();
public:
	virtual bool process(Window* win, QStringList& args);
	void help(Window* win);
protected:
	QString _name;
	QString _usage;
	QString _description;
	int _minArgs;
};

/*!
 * \class CommandAction
 * \brief /ACTION <message>
 */
struct CommandAction : public Command
{
	CommandAction();
	virtual bool process(Window* win, QStringList& args);
	virtual bool process(StatusWindow* win, QStringList& args);
};

/*!
 * \class CommandAdmin
 * \brief /ADMIN [<server>]
 */
struct CommandAdmin : public Command
{
	CommandAdmin();
};

/*!
 * \class CommandAway
 * \brief /AWAY [<message>]
 */
struct CommandAway : public Command
{
	CommandAway();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandClear
 * \brief /CLEAR
 */
struct CommandClear : public Command
{

	CommandClear();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandClearTopic
 * \brief /CLEARTOPIC [<channel>]
 */
struct CommandCleartopic : public Command
{
	CommandCleartopic();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandConnect
 * \brief /CONNECT <server> <port> [<remote>]
 */
struct CommandConnect : public Command
{
	CommandConnect();
};

/*!
 * \class CommandDie
 * \brief /DIE
 */
struct CommandDie : public Command
{
	CommandDie();
};

/*!
 * \class CommandEcho
 * \brief /ECHO [<text>]
 */
struct CommandEcho : public Command
{
	CommandEcho();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandHelp
 * \brief /HELP [<command>]
 */
struct CommandHelp : public Command
{
	CommandHelp(const QHash<QString, Command*>& cmdMap);
	virtual bool process(Window* win, QStringList& args);
private:
	const QHash<QString, Command*>& _cmdMap;
};

/*!
 * \class CommandHop
 * \brief /HOP [<channel>{,<channel>}]
 */
struct CommandHop : public Command
{
	CommandHop();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandInfo
 * \brief /INFO [<server>]
 */
struct CommandInfo : public Command
{
	CommandInfo();
};

/*!
 * \class CommandInvite
 * \brief /INVITE <nick> [<channel>]
 */
struct CommandInvite : public Command
{
	CommandInvite();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandJoin
 * \brief /JOIN [<channel>{,<channel>} [<key>{,<key>}]]
 */
struct CommandJoin : public Command
{
	CommandJoin();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandKick
 * \brief /KICK [<channel>] <nick>{,<nick>} [<reason>]
 */
struct CommandKick : public Command
{
	CommandKick();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandKill
 * \brief /KILL <nick> <reason>
 */
struct CommandKill : public Command
{
	CommandKill();
};

/*!
 * \class CommandKnock
 * \brief /KNOCK <channel> [<reason>]
 */
struct CommandKnock : public Command
{
	CommandKnock();
};

/*!
 * \class CommandLinks
 * \brief /LINKS [<server> [<mask>]]
 */
struct CommandLinks : public Command
{
	CommandLinks();
};

/*!
 * \class CommandList
 * \brief /LIST [<channel> [<server>]]
 */
struct CommandList : public Command
{
	CommandList();
};

/*!
 * \class CommandLusers
 * \brief /LUSERS [<mask> [<server>]]
 */
struct CommandLusers : public Command
{
	CommandLusers();
};

/*!
 * \class CommandMode
 * \brief /MODE [<target>] [<modes> {<mode-parameters>}]
 */
struct CommandMode : public Command
{
	CommandMode();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandMotd
 * \brief /MOTD [<server>]
 */
struct CommandMotd : public Command
{
	CommandMotd();
};

/*!
 * \class CommandMsg
 * \brief /MSG <target> <message>
 */
struct CommandMsg : public Command
{
	CommandMsg();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandNames
 * \brief /NAMES [<channel>{,<channel>}]
 */
struct CommandNames : public Command
{
	CommandNames();
};

/*!
 * \class CommandNick
 * \brief /NICK <new-nick>
 */
struct CommandNick : public Command
{
	CommandNick();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandNotice
 * \brief /NOTICE <target>{,<target>} <message>
 */
struct CommandNotice : public Command
{
	CommandNotice();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandOper
 * \brief /OPER <login> <password>
 */
struct CommandOper : public Command
{
	CommandOper();
};

/*!
 * \class CommandPart
 * \brief /PART [<channel>{,<channel>}] [<reason>]
 */
struct CommandPart : public Command
{
	CommandPart();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandQuit
 * \brief /QUIT [<reason>]
 */
struct CommandQuit : public Command
{
	CommandQuit();
};

/*!
 * \class CommandQuote
 * \brief /QUOTE <command> [<command-args>]
 */
struct CommandQuote : public Command
{
	CommandQuote();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandReconnect
 * \brief /RECONNECT
 */
struct CommandReconnect : public Command
{
	CommandReconnect();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandRehash
 * \brief /REHASH
 */
struct CommandRehash : public Command
{
	CommandRehash();
};

/*!
 * \class CommandRestart
 * \brief /RESTART
 */
struct CommandRestart : public Command
{
	CommandRestart();
};

/*!
 * \class CommandServer
 * \brief /SERVER <hostname>[:<port>] [<password>]
 */
struct CommandServer : public Command
{
	CommandServer();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandSquit
 * \brief /SQUIT <server> <comment>
 */
struct CommandSquit : public Command
{
	CommandSquit();
};

/*!
 * \class CommandStats
 * \brief /STATS [<query> [<server>]]
 */
struct CommandStats : public Command
{
	CommandStats();
};

/*!
 * \class CommandTime
 * \brief /TIME [<server>]
 */
struct CommandTime : public Command
{
	CommandTime();
};

/*!
 * \class CommandTopic
 * \brief /TOPIC [<channel>] [<subject>]
 */
struct CommandTopic : public Command
{
	CommandTopic();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandTrace
 * \brief /TRACE [<server>]
 */
struct CommandTrace : public Command
{
	CommandTrace();
};

/*!
 * \class CommandVersion
 * \brief /VERSION [<server>]
 */
struct CommandVersion : public Command
{
	CommandVersion();
};

/*!
 * \class CommandWallops
 * \brief /WALLOPS <message>
 */
struct CommandWallops : public Command
{
	CommandWallops();
	virtual bool process(Window* win, QStringList& args);
};

/*!
 * \class CommandWho
 * \brief /WHO [<mask>]
 */
struct CommandWho : public Command
{
	CommandWho();
};

/*!
 * \class CommandWhois
 * \brief /WHOIS <nick>{,<nick>}
 */
struct CommandWhois : public Command
{
	CommandWhois();
};

/*!
 * \class CommandWhowas
 * \brief /WHOWAS <nick>{,<nick>}
 */
struct CommandWhowas : public Command
{
	CommandWhowas();
};

# include "command.hxx"

#endif /* !COMMAND_HH */
