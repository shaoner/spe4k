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
 * \file channel-window.cc
 * \author shaoner
 * \brief Represents an IRC channel as a window
 */
#include "channel-window.hh"
#include <QDateTime>
#include <QTextBrowser>
#include <session.hh>
#include <command-event.hh>
#include <raw-event.hh>
#include <user.hh>
#include <user-list.hh>

#include "ui_channel-window.h"
#include "common/text-display.hh"
#include "text-box.hh"
#include "user-item.hh"
#include "edit-box.hh"
#include "widget/session-area.hh"

ChannelWindow::ChannelWindow(const QString& name, irc::Session& session, QWidget* parent) :
	Window(name, session, parent),
	_ui(new Ui::ChannelWindow),
	_me(0),
	_userList(0)
{
	_ui->setupUi(this);
	_ui->horizontalLayout_2->insertWidget(0, _textBox);
	// Attach IRC events listeners
	connect(&session, SIGNAL(onRaw(irc::RawEvent&)), this, SLOT(raw_slot(irc::RawEvent&)));
	connect(&session, SIGNAL(onChannelMessage(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(channelmessage_slot(irc::CommandEvent&, const QString&, const QString&)));
	connect(&session, SIGNAL(onChannelAction(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(channelaction_slot(irc::CommandEvent&, const QString&, const QString&)));
	connect(&session, SIGNAL(onUserList(const QString&, irc::UserList*)),
			this, SLOT(userlist_slot(const QString&, irc::UserList*)));
	connect(&session, SIGNAL(onJoin(irc::CommandEvent&, const QString&)),
			this, SLOT(join_slot(irc::CommandEvent&, const QString&)));
	connect(&session, SIGNAL(onPart(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(part_slot(irc::CommandEvent&, const QString&, const QString&)));
	connect(&session, SIGNAL(onKick(irc::CommandEvent&, const QString&, const QString&, const QString&)),
			this, SLOT(kick_slot(irc::CommandEvent&, const QString&, const QString&, const QString&)));
	connect(&session, SIGNAL(onChannelMode(irc::CommandEvent&, const QString&, const QString&, const QStringList&)),
			this, SLOT(channelmode_slot(irc::CommandEvent&, const QString&, const QString&, const QStringList&)));
	connect(&session, SIGNAL(onQuit(irc::CommandEvent&, const QString&)),
			this, SLOT(quit_slot(irc::CommandEvent&, const QString&)));
	connect(&session, SIGNAL(onNick(irc::CommandEvent&, const QString&)),
			this, SLOT(nick_slot(irc::CommandEvent&, const QString&)));
	connect(&session, SIGNAL(onChangeTopic(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(changetopic_slot(irc::CommandEvent&, const QString&, const QString&)));
	connect(&session, SIGNAL(onTopic(const QString&, const QString&)),
			this, SLOT(topic_slot(const QString&, const QString&)));
	connect(&session, SIGNAL(onNoTopic(const QString&, const QString&)),
			this, SLOT(notopic_slot(const QString&, const QString&)));
	connect(&session, SIGNAL(onTopicInfo(const QString&, const QString&, uint)),
			this, SLOT(topicinfo_slot(const QString&, const QString&, uint)));
	connect(&session, SIGNAL(onConnect()), this, SLOT(connect_slot()));
	connect(&session, SIGNAL(onDisconnect()), this, SLOT(disconnect_slot()));
	// When text is entered
	connect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(send_message()));
	// For completion
	connect(_textBox->input, SIGNAL(onTabKey(bool, int, int&)),
			this, SLOT(tabkey_slot(bool, int, int&)));
	// Click on highlight tokens (nicks, etc.)
	connect(_textBox->screen, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(click_link_slot(const QUrl&)));
}

ChannelWindow::~ChannelWindow()
{
	disconnect(_textBox->screen, SIGNAL(anchorClicked(const QUrl&)), this, SLOT(click_link_slot(const QUrl&)));
	disconnect(_textBox->input, SIGNAL(onTabKey(bool, int, int&)),
			this, SLOT(tabkey_slot(bool, int, int&)));
	disconnect(_textBox->input, SIGNAL(returnPressed()), this, SLOT(send_message()));
	// Unattach IRC event listeners
	disconnect(&session, SIGNAL(onDisconnect()), this, SLOT(disconnect_slot()));
	disconnect(&session, SIGNAL(onTopicInfo(const QString&, const QString&, uint)),
			this, SLOT(topicinfo_slot(const QString&, const QString&, uint)));
	disconnect(&session, SIGNAL(onNoTopic(const QString&, const QString&)),
			this, SLOT(notopic_slot(const QString&, const QString&)));
	disconnect(&session, SIGNAL(onTopic(const QString&, const QString&)),
			this, SLOT(topic_slot(const QString&, const QString&)));
	disconnect(&session, SIGNAL(onChangeTopic(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(changetopic_slot(irc::CommandEvent&, const QString&, const QString&)));
	disconnect(&session, SIGNAL(onNick(irc::CommandEvent&, const QString&)),
			this, SLOT(nick_slot(irc::CommandEvent&, const QString&)));
	disconnect(&session, SIGNAL(onQuit(irc::CommandEvent&, const QString&)),
			this, SLOT(quit_slot(irc::CommandEvent&, const QString&)));
	disconnect(&session, SIGNAL(onChannelMode(irc::CommandEvent&, const QString&, const QString&, const QStringList&)),
			this, SLOT(channelmode_slot(irc::CommandEvent&, const QString&, const QString&, const QStringList&)));
	disconnect(&session, SIGNAL(onKick(irc::CommandEvent&, const QString&, const QString&, const QString&)),
			this, SLOT(kick_slot(irc::CommandEvent&, const QString&, const QString&, const QString&)));
	disconnect(&session, SIGNAL(onPart(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(part_slot(irc::CommandEvent&, const QString&, const QString&)));
	disconnect(&session, SIGNAL(onJoin(irc::CommandEvent&, const QString&)),
			this, SLOT(join_slot(irc::CommandEvent&, const QString&)));
	disconnect(&session, SIGNAL(onUserList(const QString&, irc::UserList*)),
			   this, SLOT(userlist_slot(const QString&, irc::UserList*)));
	disconnect(&session, SIGNAL(onChannelAction(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(channelaction_slot(irc::CommandEvent&, const QString&, const QString&)));
	disconnect(&session, SIGNAL(onChannelMessage(irc::CommandEvent&, const QString&, const QString&)),
			this, SLOT(channelmessage_slot(irc::CommandEvent&, const QString&, const QString&)));
	disconnect(&session, SIGNAL(onRaw(irc::RawEvent&)), this, SLOT(raw_slot(irc::RawEvent&)));
	if (session.channels().contains(_name))
		session.part(_name);
	delete _ui;
}

void
ChannelWindow::clear()
{
	_ui->listWidget->clear();
	_userList = NULL;
	_me = NULL;
}

UserItem*
ChannelWindow::get_item(irc::User* user)
{
	int size = _ui->listWidget->count();
	int left = 0;
	int right = size - 1;
	// Dichotomic search, because users are sorted
	while (right >= left)
	{
		int mid = (left + right) / 2;
		UserItem* item = static_cast<UserItem*>(_ui->listWidget->item(mid));
		irc::User* curUser = item->user();
		if (*user == *curUser)
			return item;
		if (*user < *curUser)
			left = mid + 1;
		else
			right = mid - 1;
	}
	return NULL;
}

void
ChannelWindow::remove_user(irc::User* user)
{
	UserItem* item = get_item(user);
	if (item)
		delete _ui->listWidget->takeItem(_ui->listWidget->row(item));
}

void
ChannelWindow::remove_user(const QString& nick)
{
	if (_userList)
	{
		irc::User* user = _userList->get(nick);
		if (user)
			remove_user(user);
	}
}

void
ChannelWindow::add_user(const QString &nick)
{
	if (_userList)
	{
		irc::User* user = _userList->get(nick);
		if (user)
		{
			UserItem* item = new UserItem(user);
			_ui->listWidget->insertItem(0, item);
		}
	}
}

void
ChannelWindow::userlist_slot(const QString& channel, irc::UserList* users)
{
	if (channel == _name)
	{
		if (_ui->listWidget->count() > 0)
			_ui->listWidget->clear();
		_userList = users;
		_me = _userList->get(session.nickname());
		// Fill the nick list
		for (irc::UserList::iterator it = users->begin(), end = users->end();
			 it != end; ++it)
		{
			UserItem* item = new UserItem(*it);
			_ui->listWidget->addItem(item);
		}
		// There should always be a selected user
		if (_ui->listWidget->count() > 0)
			_ui->listWidget->setCurrentItem(_ui->listWidget->item(0));
	}
}

void
ChannelWindow::changetopic_slot(irc::CommandEvent &event, const QString &channel, const QString &subject)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_TOPIC_CHANGE))
		{
			TextDisplay td(DISPLAY_TOPIC_CHANGE);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_channel(channel);
			td.push_ircraw(subject);
			display_event(td);
		}
	}
}

void
ChannelWindow::topic_slot(const QString& channel, const QString& topic)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_TOPIC))
		{
			TextDisplay td(DISPLAY_TOPIC);
			td.push_channel(channel);
			td.push_ircraw(topic);
			display_event(td);
		}
	}
}

void
ChannelWindow::notopic_slot(const QString& channel, const QString& msg)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_NOTOPIC))
		{
			TextDisplay td(DISPLAY_NOTOPIC);
			td.push_channel(channel);
			td.push_raw(msg);
			display_event(td);
		}
	}
}

void
ChannelWindow::topicinfo_slot(const QString& channel, const QString& author, uint time)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_TOPIC_INFO))
		{
			QString datetime = QDateTime::fromTime_t(time).toString("yyyy/MM/dd hh:mm:ss");
			TextDisplay td(DISPLAY_TOPIC_INFO);
			td.push_nick(author);
			td.push_arg(datetime);
			display_event(td);
		}
	}
}

void
ChannelWindow::nick_slot(irc::CommandEvent& event, const QString& newNick)
{
	if (_userList && _userList->get(event.nick()))
	{
		if (_params->eventEnabled(DISPLAY_NICK))
		{
			TextDisplay td(DISPLAY_NICK);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_target(newNick);
			display_event(td);
		}
	}
}

void
ChannelWindow::join_slot(irc::CommandEvent& event, const QString& channel)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_JOIN))
		{
			TextDisplay td(DISPLAY_JOIN);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_channel(channel);
			display_event(td);
		}
		add_user(event.nick());
	}
}

void
ChannelWindow::part_slot(irc::CommandEvent& event, const QString& channel, const QString& reason)
{
	if (channel == _name)
	{
		DisplayEventType format;
		if (reason.isEmpty())
			format = DISPLAY_PART;
		else
			format = DISPLAY_PART_REASON;
		if (_params->eventEnabled(format))
		{
			TextDisplay td(format);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_channel(channel);
			td.push_raw(reason);
			display_event(td);
		}
		if (event.nick() == session.nickname())
			clear();
		else
			remove_user(event.nick());
	}
}

void
ChannelWindow::kick_slot(irc::CommandEvent& event, const QString& channel, const QString& target, const QString& reason)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_KICK))
		{
			TextDisplay td(DISPLAY_KICK);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_channel(channel);
			td.push_target(target);
			td.push_raw(reason);
			display_event(td);
		}
		if (target == session.nickname())
			clear();
		else
			remove_user(event.nick());
	}
}

void
ChannelWindow::channelmode_slot(irc::CommandEvent& event, const QString& channel,
								const QString& modes, const QStringList& args)
{
	if (channel == _name)
	{
		if (_params->eventEnabled(DISPLAY_MODE))
		{
			TextDisplay td(DISPLAY_MODE);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_channel(channel);
			td.push_modes(modes);
			td.push_arg(args.join(" "));
			display_event(td);
		}
	}
}

void
ChannelWindow::quit_slot(irc::CommandEvent& event, const QString& reason)
{
	irc::User* user = NULL;
	if (_userList && (user = _userList->get(event.nick())))
	{
		if (_params->eventEnabled(DISPLAY_QUIT))
		{
			TextDisplay td(DISPLAY_QUIT);
			td.push_nick(event.nick());
			td.push_user(event.user());
			td.push_host(event.host());
			td.push_raw(reason);
			display_event(td);
			remove_user(user);
		}
	}
}

void
ChannelWindow::raw_slot(irc::RawEvent& event)
{
	Q_UNUSED(event);
}

void
ChannelWindow::channelmessage_slot(irc::CommandEvent& event, const QString& channel, const QString& msg)
{
	if (channel == _name)
	{
		irc::User* user = NULL;
		// There is no guarantee that the userlist was got
		if (_userList && (user = _userList->get(event.nick())))
			display_message(user->fullnick(), user->nick(), msg);
		else
			display_message(event.nick(), event.nick(), msg);
		if (!is_active())
		{
			item()->set_level(LIGHT_LVL_MESSAGE);
		}
	}
}

void
ChannelWindow::channelaction_slot(irc::CommandEvent& event, const QString& channel, const QString& msg)
{
	if (channel == _name)
	{
		irc::User* user = NULL;
		// There is no guarantee that the userlist was got
		if (_userList && (user = _userList->get(event.nick())))
			display_action(user->fullnick(), user->nick(), msg);
		else
			display_action(event.nick(), event.nick(), msg);
		if (!is_active())
		{
			item()->set_level(LIGHT_LVL_MESSAGE);
		}
	}
}

void
ChannelWindow::connect_slot()
{
	if (_params->get<bool>("rejoin_onconnect"))
	{
		session.join(_name);
	}
}

void
ChannelWindow::disconnect_slot()
{
	display_info("Disconnected");
	clear();
}

void
ChannelWindow::send_message()
{
	if (_me)
	{
		send(_me->fullnick(), _me->nick());
	}
	else
	{
		send(session.nickname(), session.nickname());
	}
}

QStringList
ChannelWindow::nicks_start_with(const QString& token)
{
	QStringList ql;
	// Fill a list of nicks for completion
	for (int i = 0, len = _ui->listWidget->count(); i < len; ++i)
	{
		UserItem* item = static_cast<UserItem*>(_ui->listWidget->item(i));
		if (item)
		{
			const QString& nick = item->user()->nick();
			if (nick.startsWith(token))
				ql.append(nick);
		}
	}
	return ql;
}

QStringList
ChannelWindow::channels_start_with(const QString& token)
{
	QStringList cl;
	// Fill a list of channels for completion
	QList<QString> channels = session.channels().keys();
	foreach (QString channel, channels)
	{
		if (channel.startsWith(token))
			cl.append(channel);
	}
	return cl;
}

void
ChannelWindow::tabkey_slot(bool previousTab, int begPos, int& len)
{
	EditBox* editBox = _textBox->input;
	if (!previousTab)
	{
		// This is a new try for completion
		QString token = editBox->text().mid(begPos, len);
		if (token.isEmpty())
			return;
		// If this looks like a channel, we get a channel list
		// otherwise we get a nick list
		if (session.is_channel(token))
			_completionList = channels_start_with(token);
		else
			_completionList = nicks_start_with(token);
		// Append the current token because of the cycle
		_completionList.append(token);
		_compIdx = 1;
		// Replace the current token with the first item of the list
		const QString& compTok = _completionList[0];
		len = compTok.size();
		editBox->setText(editBox->text().replace(begPos, len, ""));
		editBox->setText(editBox->text().replace(begPos, 1, compTok));
	}
	else
	{
		// Replace the current token with the current item of the list
		const QString& compTok = _completionList[_compIdx++ % _completionList.size()];
		editBox->setText(editBox->text().replace(begPos, len, ""));
		editBox->setText(editBox->text().replace(begPos, 1, compTok));
		len = compTok.size();
	}
}

void
ChannelWindow::click_link_slot(const QUrl& url)
{
	QString anchor = url.fragment();
	// Check if the link is #u_<nick>
	if (anchor.startsWith("u_"))
	{
		QString nick = anchor.mid(2);
		irc::User* user = NULL;
		if (_userList && (user = _userList->get(nick)))
		{
			QListWidgetItem* item = get_item(user);
			if (item)
			{
				item->setSelected(true);
			}
		}
	}
}
