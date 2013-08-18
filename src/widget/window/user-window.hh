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
 * \file user-window.hh
 * \author shaoner
 * \brief Window for private messages
 */
#ifndef USER_WINDOW_HH
# define USER_WINDOW_HH

# include "window.hh"

namespace Ui
{
	class UserWindow;
} // namespace Ui

class UserWindow : public Window
{

	Q_OBJECT

public:
	/// Ctor
	explicit UserWindow(const QString& name, irc::Session& session, QWidget* parent = 0);
	/// Dtor
	~UserWindow();
private slots:
	/// IRC event listeners
	void privatemessage_slot(irc::CommandEvent& event, const QString& msg);
	void nick_slot(irc::CommandEvent& event, const QString& newNick);
	void quit_slot(irc::CommandEvent& event, const QString& reason);
	void disconnect_slot();
	void send_message();
private:
	Ui::UserWindow* _ui;
};

#endif /* !USER_WINDOW_HH */
