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
 * \file network-dialog.hh
 * \author shaoner
 * \brief Add a network
 */
#ifndef NETWORK_DIALOG_HH
# define NETWORK_DIALOG_HH

# include <QDialog>

# include "network.hh"

namespace Ui
{
	class NetworkDialog;
} // namespace Ui

class Parameters;

class NetworkDialog : public QDialog
{
	Q_OBJECT

public:
	explicit NetworkDialog(QWidget *parent = 0);
	~NetworkDialog();
public:
	int open();
	int open(const Network& network);
	Network get_network() const;
private:
	void reset();
	void enable_save();
private slots:
	void on_edtNetwork_textEdited(const QString&);
	void on_edtAddress_textEdited(const QString&);
	void on_edtNickname_textEdited(const QString&);
	void on_edtUser_textEdited(const QString&);
	void on_edtRealname_textEdited(const QString&);
	void on_btnSave_clicked();
	void on_btnCancel_clicked();
	void on_edtNickname_lostFocus();
	void on_chkInvisibleMode_clicked();
	void on_chkReceiveWallops_clicked();
	void on_edtAltNickname_textEdited(const QString &arg1);
	void on_chkGlobalIdentity_toggled(bool checked);
private:
	Ui::NetworkDialog* _ui;
	Parameters* _globalSettings;
};

#endif /* !NETWORK_DIALOG_HH */
