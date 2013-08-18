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
 * \file session-area.cc
 * \author shaoner
 * \brief Area for multiple windows dedicated to a session
 */
#include "session-area.hh"

#include <QMdiSubWindow>
#include <QCloseEvent>
#include <command-event.hh>
#include <raw-event.hh>

#include "window/status-window.hh"
#include "window/channel-window.hh"
#include "window/user-window.hh"
#include "window/window-item.hh"
#include "window/status-subwindow.hh"

SessionArea::SessionArea(QTreeWidget* tree, const irc::SessionParameters& sessionParams, QWidget* parent) :
	QMdiArea(parent),
	name(sessionParams.name()),
	_tree(tree),
	_session(sessionParams),
	_statusWindow(new StatusWindow(name, _session, this)),
	_params(Parameters::get()),
	_reconnectDelay(0),
	_reconnectRetry(0)
{
	// Irc events
	connect(&_session, SIGNAL(onSocketError(QAbstractSocket::SocketError)), this, SLOT(error_slot(QAbstractSocket::SocketError)));
	connect(&_session, SIGNAL(onJoin(irc::CommandEvent&, const QString&)), this, SLOT(join_slot(irc::CommandEvent&, const QString&)));
	connect(&_session, SIGNAL(onPart(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(part_slot(irc::CommandEvent&, const QString&, const QString&)));
	connect(&_session, SIGNAL(onKick(irc::CommandEvent&, const QString&, const QString&, const QString&)),
			this, SLOT(kick_slot(irc::CommandEvent&, const QString&, const QString&, const QString&)));
	connect(&_session, SIGNAL(onPrivateMessage(irc::CommandEvent&, const QString&)),
			this, SLOT(privatemessage_slot(irc::CommandEvent&, const QString&)));
	connect(&_session, SIGNAL(onConnect()), this, SLOT(connect_slot()));
	connect(&_session, SIGNAL(onDisconnect()), this, SLOT(disconnect_slot()));
	// On active window
	connect(this, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(active_window_slot(QMdiSubWindow*)));
	// Enable timer for reconnection
	connect(&_timerReconnect, SIGNAL(timeout()), this, SLOT(timeout_reconnect()));
	// Initialize the status window
	WindowItem* item = _statusWindow->item();
	StatusSubWindow* subWin = new StatusSubWindow(this);
	subWin->setAttribute(Qt::WA_DeleteOnClose);
	subWin->setWidget(_statusWindow);
	addSubWindow(subWin);
	subWin->setWindowTitle(tr("Status ") + name);
	// Set the new window as active
	setActiveSubWindow(subWin);
	// For completion
	subWin->setFocusPolicy(Qt::NoFocus);
	// For the window to be maximized at startup
	subWin->showMaximized();
	item->set_subwindow(subWin);
	// Add the tree item
	_tree->addTopLevelItem(item);
	_tree->expandItem(item);
	_tree->setCurrentItem(item);
	// Starts the connection
	_session.start();
}

SessionArea::~SessionArea()
{
	_timerReconnect.stop();
	WindowItem* item = _statusWindow->item();
	// Remove channels and query window
	while (item->childCount())
	{
		remove_window(0);
	}
	// Disconnect the IRC session
	_session.stop();
	// Remove the status window
	removeSubWindow(item->subwindow());
	delete item->subwindow();
}

void
SessionArea::active_window_slot(QMdiSubWindow* subWin)
{
	if (!subWin)
		return;
	Window* win = static_cast<Window*>(subWin->widget());
	win->setFocus();
	Q_ASSERT(win != 0);
	WindowItem* item = win->item();
	Q_ASSERT(item != 0);
	// Clear the level color in the tree
	win->item()->reset_level();
	_tree->setCurrentItem(item);
}

int
SessionArea::index_of(const QString& name)
{
	WindowItem* item = _statusWindow->item();
	int size = item->childCount();
	int left = 0;
	int right = size - 1;
	// Simple dichotomic search
	while (right >= left)
	{
		int mid = (left + right) / 2;
		QString curName = item->child(mid)->text(0);
		if (name == curName)
			return mid;
		if (name.compare(curName) > 0)
			left = mid + 1;
		else
			right = mid - 1;
	}
	return -1;
}

void
SessionArea::add_window(Window* win, bool active)
{
	WindowItem* item = _statusWindow->item();
	QMdiSubWindow* subWin = addSubWindow(win);
	//subWin->setAttribute(Qt::WA_DeleteOnClose);
	subWin->setWindowTitle(win->name());
	// For completion
	subWin->setFocusPolicy(Qt::NoFocus);
	win->item()->set_subwindow(subWin);

	int size = item->childCount();
	int left = 0;
	int right = size - 1;
	int mid = 0;
	int idx = 0;
	// The treeitem is inserted in a sorted list of items
	while (right >= left)
	{
		// We reduce the left and the right marker
		// until we find the right place to insert the user
		mid = (left + right) / 2;
		QString curName = item->child(mid)->text(0);
		if (win->name().compare(curName) > 0)
			left = mid + 1;
		else
			right = mid - 1;
	}
	if (left > right)
		idx = left;
	else
		idx = right;

	item->insertChild(idx, win->item());
	if (active)
	{
		_tree->setCurrentItem(win->item());
		setActiveSubWindow(subWin);
		subWin->showMaximized();
	}
}

ChannelWindow*
SessionArea::add_channelwindow(const QString& name)
{
	ChannelWindow* channelWin = new ChannelWindow(name, _session, this);
	add_window(channelWin, true);
	return channelWin;
}

UserWindow*
SessionArea::add_userwindow(const QString& name)
{
	UserWindow* userWin = new UserWindow(name, _session, this);
	add_window(userWin, false);
	return userWin;
}

void
SessionArea::remove_window(int idx)
{
	QTreeWidgetItem* treeItem = _statusWindow->item()->child(idx);
	WindowItem* subItem = static_cast<WindowItem*>(treeItem);
	QWidget* window = subItem->subwindow()->widget();
	removeSubWindow(window);
	delete window;
	delete subItem->subwindow();
}

void
SessionArea::remove_window(const QString& name)
{
	int idx = index_of(name);
	Q_ASSERT(idx > -1);
	if (idx > -1)
	{
		remove_window(idx);
	}
}

void
SessionArea::privatemessage_slot(irc::CommandEvent& event, const QString& message)
{
	int idx = index_of(event.nick());
	if (idx < 0)
	{
		UserWindow* userWin = add_userwindow(event.nick());
		userWin->display_message(event.nick(), event.nick(), message);
	}
}

void
SessionArea::join_slot(irc::CommandEvent& event, const QString& channel)
{
	if (event.nick() == _session.nickname())
	{
		int idx = index_of(channel);
		if (idx < 0)
		{
			ChannelWindow* channelWin = add_channelwindow(channel);
			channelWin->join_slot(event, channel);
		}
	}
}

void
SessionArea::part_slot(irc::CommandEvent& event, const QString& channel, const QString& reason)
{
	Q_UNUSED(reason);
	if (event.nick() == _session.nickname())
	{
		if (_params->get<bool>("closewin_onpart"))
			remove_window(channel);
	}
}

void
SessionArea::kick_slot(irc::CommandEvent&, const QString& channel, const QString& target, const QString&)
{
	if (target == _session.nickname())
	{
		if (_params->get<bool>("closewin_onpart"))
			remove_window(channel);
		if (_params->get<bool>("rejoin_onkick"))
			_session.join(channel);
	}
}

void
SessionArea::connect_slot()
{
	emit onSessionConnect(this);
	_reconnectRetry = 0;
}

void
SessionArea::disconnect_slot()
{
	emit onSessionDisconnect(this);
	_reconnectDelay = 0;
	if (_params->get<bool>("closewins_ondisconnect"))
	{
		while (_statusWindow->item()->childCount())
		{
			remove_window(0);
		}
	}
}

void
SessionArea::reconnect()
{
	if (_reconnectRetry >= _params->get<int>("reconnect_retry"))
		return;
	_statusWindow->display_info(tr("Try reconnecting (") + QString::number(_reconnectRetry + 1) + ")");
	if (_params->get<int>("reconnect_delay") > 0)
	{
		_timerReconnect.start(1000);
	}
	else if (_reconnectRetry++ <= _params->get<int>("reconnect_retry"))
	{
		_session.start();
	}
}

void
SessionArea::timeout_reconnect()
{
	int delay = _params->get<int>("reconnect_delay");

	if (_reconnectDelay == delay)
	{
		_reconnectDelay = 0;
		++_reconnectRetry;
		_timerReconnect.stop();
		_session.start();
	}
	else
		_statusWindow->display_info(tr("Reconnecting in %n second(s)", "", delay - _reconnectDelay++));
}

void
SessionArea::error_slot(QAbstractSocket::SocketError error)
{
	switch (error)
	{
		case QAbstractSocket::RemoteHostClosedError:
			//_statusWindow->display_info("The remote host closed the connection");
			break;
		case QAbstractSocket::HostNotFoundError:
			_statusWindow->display_info(tr("Server host not found"));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			_statusWindow->display_info(tr("Connection refused."));
			reconnect();
			break;
		case QAbstractSocket::SocketTimeoutError:
			_statusWindow->display_info(tr("Connection timeout."));
			reconnect();
			break;
		default:
			return;
	}
}

void
SessionArea::closeEvent(QCloseEvent*)
{
	emit onClose(this);
}
