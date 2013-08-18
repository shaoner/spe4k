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
 * \file network-dialog.cc
 * \author shaoner
 * \brief Add a network
 */
#include "network-dialog.hh"

#include "ui_network-dialog.h"
#include "common/parameters.hh"

NetworkDialog::NetworkDialog(QWidget *parent) :
	QDialog(parent),
	_ui(new Ui::NetworkDialog),
	_globalSettings(Parameters::get())
{
	_ui->setupUi(this);
}

NetworkDialog::~NetworkDialog()
{
	delete _ui;
}

int
NetworkDialog::open()
{
	reset();
	return exec();
}

int
NetworkDialog::open(const Network& network)
{
	_ui->edtNetwork->setText(network.session.name());
	_ui->edtAddress->setText(network.session.hostname());
	_ui->spinPort->setValue(network.session.port());
	_ui->edtPassword->setText(network.session.password());
	if (network.globalIdentity)
	{
		_ui->edtNickname->setText(_globalSettings->get<QString>("Identity/nickname"));
		_ui->edtUser->setText(_globalSettings->get<QString>("Identity/user"));
		_ui->edtRealname->setText(_globalSettings->get<QString>("Identity/realname"));
		_ui->edtAltNickname->setText(_globalSettings->get<QString>("Identity/altnicknames"));
	}
	else
	{
		_ui->edtNickname->setText(network.session.nickname());
		_ui->edtUser->setText(network.session.user());
		_ui->edtRealname->setText(network.session.realname());
		_ui->edtAltNickname->setText(network.session.altnickname().join(","));
	}
	_ui->chkGlobalIdentity->setChecked(network.globalIdentity);
	_ui->chkInvisibleMode->setChecked(network.session.has_uMode(irc::MODE_INVISIBLE));
	_ui->chkReceiveWallops->setChecked(network.session.has_uMode(irc::MODE_RECEIVE_WALLOPS));
	return exec();
}

void
NetworkDialog::reset()
{
	_ui->edtNetwork->clear();
	_ui->edtAddress->clear();
	_ui->spinPort->setValue(6667);
	_ui->edtPassword->clear();
	_ui->edtNickname->clear();
	_ui->edtUser->clear();
	_ui->edtRealname->clear();
	_ui->edtAltNickname->clear();
	_ui->chkGlobalIdentity->setChecked(true);
	_ui->chkInvisibleMode->setChecked(true);
	_ui->chkReceiveWallops->setChecked(true);

}

Network
NetworkDialog::get_network() const
{
	Network network;
	network.session.name(_ui->edtNetwork->text())
		.hostname(_ui->edtAddress->text())
		.port(_ui->spinPort->value())
		.nickname(_ui->edtNickname->text())
		.user(_ui->edtUser->text())
		.realname(_ui->edtRealname->text());
	if (!_ui->edtAltNickname->text().isEmpty())
	{
		QStringList nicknames = _ui->edtAltNickname->text().replace(" ", "").split(',');
		network.session.altnickname(nicknames);
	}
	if (_ui->chkReceiveWallops->isChecked())
		network.session.set_uMode(irc::MODE_RECEIVE_WALLOPS);
	if (_ui->chkInvisibleMode->isChecked())
		network.session.set_uMode(irc::MODE_INVISIBLE);
	network.globalIdentity = _ui->chkGlobalIdentity->isChecked();
	return network;
}

void
NetworkDialog::enable_save()
{
	_ui->btnSave->setEnabled(!_ui->edtNetwork->text().isEmpty() &&
							 !_ui->edtAddress->text().isEmpty() &&
							 !_ui->edtNickname->text().isEmpty() &&
							 !_ui->edtUser->text().isEmpty() &&
							 !_ui->edtRealname->text().isEmpty());
}

void
NetworkDialog::on_edtNetwork_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_edtAddress_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_edtNickname_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_edtUser_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_edtRealname_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_btnSave_clicked()
{
	accept();
}

void
NetworkDialog::on_btnCancel_clicked()
{
	reject();
}

void
NetworkDialog::on_edtNickname_lostFocus()
{
	if (!_ui->edtNickname->text().isEmpty())
	{
		if (_ui->edtUser->text().isEmpty())
			_ui->edtUser->setText(_ui->edtNickname->text());
		if (_ui->edtAltNickname->text().isEmpty())
			_ui->edtAltNickname->setText(_ui->edtNickname->text() + "_");
	}
}

void
NetworkDialog::on_chkInvisibleMode_clicked()
{
	enable_save();
}

void
NetworkDialog::on_chkReceiveWallops_clicked()
{
	enable_save();
}

void
NetworkDialog::on_edtAltNickname_textEdited(const QString&)
{
	enable_save();
}

void
NetworkDialog::on_chkGlobalIdentity_toggled(bool checked)
{
	static Parameters* params = Parameters::get();
	if (checked)
	{
		_ui->wgtIdentity->setEnabled(false);
		_ui->edtNickname->setText(params->get<QString>("Identity/nickname"));
		_ui->edtUser->setText(params->get<QString>("Identity/user"));
		_ui->edtRealname->setText(params->get<QString>("Identity/realname"));
		_ui->edtAltNickname->setText(params->get<QString>("Identity/altnicknames"));
	}
	else
	{
		_ui->wgtIdentity->setEnabled(true);
	}
}
