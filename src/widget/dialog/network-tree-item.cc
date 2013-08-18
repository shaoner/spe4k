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
 * \file network-tree-item.cc
 * \author shaoner
 * \brief
 */

#include "network-tree-item.hh"

NetworkTreeItem::NetworkTreeItem(const Network& network, QTreeWidget* parent) :
    QTreeWidgetItem(parent),
    _network(network),
	_globalSettings(Parameters::get()),
    _addressItem(new QTreeWidgetItem(this)),
    _portItem(new QTreeWidgetItem(this)),
    _nicknameItem(new QTreeWidgetItem(this)),
    _userItem(new QTreeWidgetItem(this)),
    _realnameItem(new QTreeWidgetItem(this)),
    _altnickItem(new QTreeWidgetItem(this)),
    _modeItem(new QTreeWidgetItem(this)),
    _modeInvibleItem(new QTreeWidgetItem(_modeItem)),
    _modeReceiveWallopsItem(new QTreeWidgetItem(_modeItem))
{
    _modeInvibleItem->setText(0, "Invisible");
    _modeReceiveWallopsItem->setText(0, "Receive Wallops");
    fill_from_params(network);
}

NetworkTreeItem::~NetworkTreeItem()
{
	// All children are deleted automatically by the parent class
}

void
NetworkTreeItem::fill_from_params(const Network& network)
{
    setText(0, network.session.name());
    _addressItem->setText(0, "Address: " + network.session.hostname());
    _portItem->setText(0, "Port: " + QString::number(network.session.port()));
	if (network.globalIdentity)
	{
		_nicknameItem->setText(0, "Use global identity");
        _userItem->setHidden(true);
		_realnameItem->setHidden(true);
		_altnickItem->setHidden(true);
	}
	else
	{
        _userItem->setHidden(false);
		_realnameItem->setHidden(false);
		_altnickItem->setHidden(false);

		_nicknameItem->setText(0, "Nickname: " + network.session.nickname());
		_userItem->setText(0, "Identd: " + network.session.user());
		_realnameItem->setText(0, "Realname: " + network.session.realname());
		_altnickItem->setText(0, "Alternative nicknames");
		// Alternative nicks
		// 1. Remove old
		while (_altnickItem->childCount() > 0)
			delete _altnickItem->takeChild(0);
		const QStringList& altnicks = network.session.altnickname();
		_altnickItem->setDisabled(altnicks.count() == 0);
		// 2. Add new
		foreach (QString nick, altnicks)
		{
			QTreeWidgetItem* subAltnickItem = new QTreeWidgetItem(_altnickItem);
			subAltnickItem->setText(0, nick);
		}
	}
    // User modes
    _modeItem->setText(0, "User modes");
    _modeItem->setDisabled(true);
    _modeInvibleItem->setDisabled(true);
    _modeReceiveWallopsItem->setDisabled(true);
    if (network.session.has_uMode(irc::MODE_INVISIBLE))
    {
        _modeItem->setDisabled(false);
        _modeInvibleItem->setDisabled(false);
    }
    if (network.session.has_uMode(irc::MODE_RECEIVE_WALLOPS))
    {
        _modeItem->setDisabled(false);
        _modeReceiveWallopsItem->setDisabled(false);
    }
    _network = network;
}
