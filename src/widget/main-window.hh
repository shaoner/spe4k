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
 * \file mainwindow.hh
 * \author shaoner
 * \brief Main window
 */
#ifndef MAIN_WINDOW_HH
# define MAIN_WINDOW_HH

# include <QMainWindow>

namespace Ui
{
	class MainWindow;
} // namespace Ui

namespace irc
{
	class SessionParameters;
} // namespace irc

class QTreeWidgetItem;
class SessionListDialog;
class OptionDialog;
class SessionArea;


class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
	/// Ctor
    explicit MainWindow(QWidget* parent = 0);
	/// Dtor
    ~MainWindow();
public:
	/// Add a new session
    void add_session(const irc::SessionParameters& sessionParams);
private slots:
	/// Session event handlers
    void item_active_slot(QTreeWidgetItem* item, int column);
	void active_session_slot(int index);
	void remove_session(SessionArea* area);
    void session_connect_slot(SessionArea* area);
    void session_disconnect_slot(SessionArea* area);
    /// Actions menu & toolbar
    void on_actionTabs_toggled(bool enabled);
    void on_actionCascade_triggered();
    void on_actionTile_triggered();
    void on_actionTree_toggled(bool enabled);
    void on_actionTbConnection_triggered();
    void on_actionServers_triggered();
    void on_actionIdentities_triggered();
    void on_actionOptions_triggered();
private:
    Ui::MainWindow* _ui;
    SessionListDialog* _sessionListDialog;
	OptionDialog* _optionDialog;
};

#endif /* !MAIN_WINDOW_HH */
