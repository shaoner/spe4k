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
 * \file sessionlist-dialog.cc
 * \author shaoner
 * \brief
 */

#include "sessionlist-dialog.hh"

#include <QSettings>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "ui_sessionlist-dialog.h"
#include "common/parameters.hh"
#include "network.hh"
#include "network-dialog.hh"
#include "network-tree-item.hh"

SessionListDialog::SessionListDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::SessionListDialog),
    _networkSettings(new QSettings(ORG_NAME, CONFIG_NETWORK)),
    _networkDialog(new NetworkDialog(this)),
    _globalSettings(Parameters::get()),
    _sessionParams("")
{
    _ui->setupUi(this);
    _ui->edtGblNickname->setText(_globalSettings->get<QString>("Identity/nickname"));
    _ui->edtGblUsername->setText(_globalSettings->get<QString>("Identity/user"));
    _ui->edtGblRealname->setText(_globalSettings->get<QString>("Identity/realname"));
    _ui->edtGblAltnicknames->setText(_globalSettings->get<QString>("Identity/altnicknames"));
    fill_networks();
}

SessionListDialog::~SessionListDialog()
{
    delete _networkDialog;
    delete _networkSettings;
    delete _ui;
}

void
SessionListDialog::fill_networks()
{
    QStringList groups = _networkSettings->childGroups();
    foreach (QString name, groups)
    {
        _networkSettings->beginGroup(name);

		QString hostname = _networkSettings->value("address").toString();
		int port = _networkSettings->value("port", 6667).toInt();
		QString nickname;
		QString user;
		QString realname;
        QString altnickname;
		Network network;

        if (_networkSettings->contains("use_globalIdentity"))
        {
			nickname = _globalSettings->get<QString>("Identity/nickname");
			user = _globalSettings->get<QString>("Identity/user");
			realname = _globalSettings->get<QString>("Identity/realname");
			altnickname = _globalSettings->get<QString>("Identity/altnicknames");
			network.globalIdentity = true;
        }
        else
        {
            nickname = _networkSettings->value("nickname").toString();
			user = _networkSettings->value("user").toString();
			realname = _networkSettings->value("realname").toString();
            altnickname =  _networkSettings->value("alternative_nicknames", "").toString();
			network.globalIdentity = false;
        }
		network.session.name(name)
			.hostname(hostname)
			.port(port)
			.nickname(nickname)
			.user(user)
			.realname(realname);
        QStringList altnicklist = altnickname.replace(" ", "").split(',');
		network.session.altnickname(altnicklist);
        if (_networkSettings->value("mode_invisible", false).toBool())
            network.session.set_uMode(irc::MODE_INVISIBLE);
        if (_networkSettings->value("mode_receive_wallops", false).toBool())
            network.session.set_uMode(irc::MODE_RECEIVE_WALLOPS);
		add_item(network);
        _networkSettings->endGroup();
    }
}

bool
SessionListDialog::contains(const QString& name)
{

    for(int i = 0, len = _ui->lstSession->topLevelItemCount(); i < len; ++i)
    {
        NetworkTreeItem* networkItem = static_cast<NetworkTreeItem*>(_ui->lstSession->topLevelItem(i));
        if (networkItem->network().session.name() == name)
            return true;
    }
    return false;
}

void
SessionListDialog::save_network(const Network& network)
{
    _networkSettings->beginGroup(network.session.name());
    _networkSettings->setValue("address", network.session.hostname());
    _networkSettings->setValue("port", network.session.port());
	if (network.globalIdentity)
	{
		_networkSettings->setValue("use_globalIdentity", true);
	}
	else
	{
		_networkSettings->setValue("nickname", network.session.nickname());
		_networkSettings->setValue("user", network.session.user());
		_networkSettings->setValue("realname", network.session.realname());
		QStringList altNicks = network.session.altnickname();
		if (altNicks.count() > 0)
			_networkSettings->setValue("alternative_nicknames", altNicks.join(","));
	}
    _networkSettings->setValue("mode_invisible", network.session.has_uMode(irc::MODE_INVISIBLE));
    _networkSettings->setValue("mode_receive_wallops", network.session.has_uMode(irc::MODE_RECEIVE_WALLOPS));
    _networkSettings->endGroup();
    _networkSettings->sync();
}

void
SessionListDialog::add_item(const Network& network)
{
    NetworkTreeItem* networkItem = new NetworkTreeItem(network, _ui->lstSession);
	Q_UNUSED(networkItem);
}

void
SessionListDialog::edit_item(QTreeWidgetItem* item)
{
    if (item->parent())
        item = item->parent();
    NetworkTreeItem* networkItem = static_cast<NetworkTreeItem*>(item);
    if (_networkDialog->open(networkItem->network()) == QDialog::Accepted)
    {
		Network network = _networkDialog->get_network();
        save_network(network);
        networkItem->fill_from_params(network);
    }
}

void
SessionListDialog::init_connection(QTreeWidgetItem* item)
{
    if (item->parent())
        item = item->parent();
    NetworkTreeItem* networkItem = static_cast<NetworkTreeItem*>(_ui->lstSession->currentItem());
    _sessionParams = networkItem->network().session;
    accept();
}

void
SessionListDialog::on_btnAdd_clicked()
{
    if (_networkDialog->open() == QDialog::Accepted)
    {
		Network network = _networkDialog->get_network();
        if (contains(network.session.name()))
            return;
        save_network(network);
        add_item(network);
    }
}

void
SessionListDialog::on_btnConnect_clicked()
{
    if (_ui->lstSession->currentItem())
    {
        QTreeWidgetItem* item = _ui->lstSession->currentItem();
        init_connection(item);
    }
}

void
SessionListDialog::on_btnEdit_clicked()
{
    if (_ui->lstSession->currentItem())
    {
        edit_item(_ui->lstSession->currentItem());
    }
}

void
SessionListDialog::on_lstSession_itemDoubleClicked(QTreeWidgetItem* item, int)
{
    if (item)
    {
        init_connection(item);
    }
}

void
SessionListDialog::on_btnRemove_clicked()
{
    QTreeWidgetItem* item = _ui->lstSession->currentItem();
    if (item)
    {
        // Since the top level items are sets with session name
        // this is safe for now
        QString networkName = item->text(0);
        if (QMessageBox::warning(this, "Remove " + networkName,
                                 "Do you want to remove network " + networkName + "?",
                                 QMessageBox::No | QMessageBox::Yes) == QMessageBox::Yes)
        {
            _networkSettings->remove(networkName);
            delete item;
        }
    }
}

void
SessionListDialog::on_edtGblNickname_editingFinished()
{
    _globalSettings->set("Identity/nickname", _ui->edtGblNickname->text());
}

void
SessionListDialog::on_edtGblUsername_editingFinished()
{
    _globalSettings->set("Identity/user", _ui->edtGblUsername->text());
}

void
SessionListDialog::on_edtGblRealname_editingFinished()
{
    _globalSettings->set("Identity/realname", _ui->edtGblRealname->text());
}

void
SessionListDialog::on_edtGblAltnicknames_editingFinished()
{
    _globalSettings->set("Identity/altnicknames", _ui->edtGblAltnicknames->text());
}
