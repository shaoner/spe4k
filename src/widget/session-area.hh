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
 * \file session-area.hh
 * \author shaoner
 * \brief Area for multiple windows dedicated to a session
 */
#ifndef SESSION_AREA_HH
# define SESSION_AREA_HH

# include <QMdiArea>

# include <QTimer>
# include <session.hh>

class QMdiSubWindow;
class QTreeWidget;

namespace irc
{
	class CommandEvent;
	class RawEvent;
} // namespace irc

class Window;
class StatusWindow;
class ChannelWindow;
class UserWindow;
class Parameters;

class SessionArea : public QMdiArea
{

	Q_OBJECT

public:
	/// Ctor
	explicit SessionArea(QTreeWidget* tree, const irc::SessionParameters& sessionParams, QWidget *parent = 0);
	/// Dtor
	~SessionArea();
public:
	irc::Session& session();
	void remove_window(const QString& name);
	const QString& name;
signals:
	void onWindowCreated(int serverId, int id, const QString& name, bool active);
	void onWindowRemoved(int serverId, int id);
	void onSessionConnect(SessionArea* area);
	void onSessionDisconnect(SessionArea* area);
	void onClose(SessionArea* area);
protected:
	virtual void closeEvent(QCloseEvent* event);
private:
	/// Handle adding and removing windows
	int index_of(const QString& name);
	void add_window(Window* win, bool active);
	ChannelWindow* add_channelwindow(const QString& name);
	UserWindow* add_userwindow(const QString& name);
	void remove_window(int idx);
	/// Reconnect handler
	void reconnect();
private slots:
	void privatemessage_slot(irc::CommandEvent& event, const QString& message);
	void join_slot(irc::CommandEvent& event, const QString& channel);
	void part_slot(irc::CommandEvent& event, const QString& channel, const QString& reason);
	void kick_slot(irc::CommandEvent& event, const QString& channel, const QString& target, const QString& reason);
	void connect_slot();
	void disconnect_slot();
	void error_slot(QAbstractSocket::SocketError error);
	void active_window_slot(QMdiSubWindow*);
	void timeout_reconnect();
private:
	QTreeWidget* _tree;
	irc::Session _session;
	StatusWindow* _statusWindow;
	QTimer _timerReconnect;
	Parameters* _params;
	int _reconnectDelay;
	int _reconnectRetry;
};

inline irc::Session&
SessionArea::session()
{
	return _session;
}

#endif /* !SESSION_AREA_HH */
