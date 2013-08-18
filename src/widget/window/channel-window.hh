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
 * \file channel-window.hh
 * \author shaoner
 * \brief Represents an IRC channel as a window
 */
#ifndef CHANNEL_WINDOW_HH
# define CHANNEL_WINDOW_HH

# include <QStringList>

# include "window.hh"

namespace Ui
{
	class ChannelWindow;
} // namespace Ui

namespace irc
{
	class User;
	class UserList;
} // namespace irc

class UserItem;

class ChannelWindow : public Window
{

	Q_OBJECT

public:
    /// Ctor
    explicit ChannelWindow(const QString& name, irc::Session& session, QWidget* parent = 0);
	/// Dtor
	~ChannelWindow();
public slots:
    /// IRC event listeners
    void userlist_slot(const QString& channel, irc::UserList* users);
    void changetopic_slot(irc::CommandEvent& event, const QString& channel, const QString& subject);
    void topic_slot(const QString& channel, const QString& topic);
    void notopic_slot(const QString& channel, const QString& msg);
    void topicinfo_slot(const QString& channel, const QString& author, uint datetime);
    void nick_slot(irc::CommandEvent& event, const QString& newNick);
    void join_slot(irc::CommandEvent& event, const QString& channel);
    void part_slot(irc::CommandEvent& event, const QString& channel, const QString& reason);
    void kick_slot(irc::CommandEvent& event, const QString& channel, const QString& target, const QString& reason);
    void channelmode_slot(irc::CommandEvent& event, const QString& channel, const QString& modes, const QStringList& args);
    void quit_slot(irc::CommandEvent& event, const QString& reason);
    void raw_slot(irc::RawEvent& event);
    void channelmessage_slot(irc::CommandEvent& event, const QString& channel, const QString& msg);
    void channelaction_slot(irc::CommandEvent& event, const QString& channel, const QString& msg);
    void disconnect_slot();
    void connect_slot();
private slots:
    /// UI event listeners
	void send_message();
    void tabkey_slot(bool previousTab, int begPos, int& len);
    void click_link_slot(const QUrl& url);
private:
	/// Handle userlist
    UserItem* get_item(irc::User* user);
	void clear();
    void remove_user(irc::User* user);
    void remove_user(const QString& nick);
    void add_user(const QString& nick);
	/// Useful lists for completion
    QStringList nicks_start_with(const QString& token);
    QStringList channels_start_with(const QString& token);
private:
    Ui::ChannelWindow* _ui;
	irc::User* _me;
	irc::UserList* _userList;
    QStringList _completionList;
    int _compIdx;
};

#endif /* !CHANNEL_WINDOW_HH */
