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
 * \file network-tree-item.hh
 * \author shaoner
 * \brief Network item in the session list tree
 */
#ifndef NETWORK_ITEM_HH
# define NETWORK_ITEM_HH

#include <QTreeWidgetItem>

#include "network.hh"
#include "common/parameters.hh"

class NetworkTreeItem : public QTreeWidgetItem
{
public:
	explicit NetworkTreeItem(const Network& network, QTreeWidget* parent = 0);
	~NetworkTreeItem();
public:
	void fill_from_params(const Network& network);
	const Network& network();
private:
	Network _network;
	Parameters* _globalSettings;
	QTreeWidgetItem* _addressItem;
	QTreeWidgetItem* _portItem;
	QTreeWidgetItem* _nicknameItem;
	QTreeWidgetItem* _userItem;
	QTreeWidgetItem* _realnameItem;
	QTreeWidgetItem* _altnickItem;
	QTreeWidgetItem* _modeItem;
	QTreeWidgetItem* _modeInvibleItem;
	QTreeWidgetItem* _modeReceiveWallopsItem;
};

inline const Network&
NetworkTreeItem::network()
{
	QStringList altNicks = _globalSettings->get<QString>("Identity/altnicknames").replace(" ", "").split(',');
	_network.session.nickname(_globalSettings->get<QString>("Identity/nickname"))
		.user(_globalSettings->get<QString>("Identity/nickname"))
		.realname(_globalSettings->get<QString>("Identity/nickname"))
		.altnickname(altNicks);
	return _network;
}

#endif /* !NETWORK_ITEM_HH */
