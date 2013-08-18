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
 * \file user-item.hh
 * \author shaoner
 * \brief
 */

#ifndef USER_ITEM_HH
# define USER_ITEM_HH

# include <QListWidgetItem>
# include <user.hh>

namespace irc
{
	class User;
} // namespace irc

class UserItem : public QObject, public QListWidgetItem
{

    Q_OBJECT

public:
	/// Ctor
    explicit UserItem(irc::User* user);
public:
	irc::User* user() const;
    virtual bool operator<(const QListWidgetItem& other) const;
private slots:
    void change_fullnick_slot(const QString& nick);
private:
	irc::User* _user;
};

inline irc::User*
UserItem::user() const
{
    return _user;
}

inline bool
UserItem::operator<(const QListWidgetItem& other) const
{
	const UserItem& item = static_cast<const UserItem&>(other);
	// The opposite because the listwidget put small elements at the top
	return !(*_user < *(item.user()));
}

#endif /* !USER_ITEM_HH */
