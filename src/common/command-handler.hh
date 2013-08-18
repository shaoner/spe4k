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
 * \file command-handler.hh
 * \author shaoner
 * \brief Handles internal commands
 */
#ifndef COMMAND_HANDLER_HH
# define COMMAND_HANDLER_HH

# include <QHash>
# include <QString>

class QStringList;
class Command;
class Window;

/*!
 * \class CommandHandler
 * \brief This singleton allows to execute an internal command
 */
class CommandHandler
{
public:
	static CommandHandler* get();
	static void reset();
public:
	/// Execute the associated command
	void exec(Window* win, const QString& commandName, QStringList& args);
private:
	static CommandHandler* _instance;
private:
	/// Ctor
	CommandHandler();
	/// Dtor
	~CommandHandler();
private:
	QHash<QString, Command*> _internCmdMap;
};

#endif /* !COMMAND_HANDLER_HH */
