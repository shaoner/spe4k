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
 * \file status-window.hh
 * \author shaoner
 * \brief Status window to get server information
 */
#ifndef STATUS_WINDOW_HH
# define STATUS_WINDOW_HH

# include "window.hh"

namespace Ui
{
	class StatusWindow;
} // namespace Ui

class StatusWindow : public Window
{
	Q_OBJECT

public:
	/// Ctor
	explicit StatusWindow(const QString& name,
						  irc::Session& session,
						  QWidget* parent = 0);
	/// Dtor
	~StatusWindow();
private slots:
	/// IRC event listeners
	void raw_slot(irc::RawEvent& event);
	void connecting_slot();
	void connect_slot();
	void disconnect_slot();
private:
	Ui::StatusWindow* _ui;
};

#endif /* !STATUS_WINDOW_HH */
