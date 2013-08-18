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
 * \file sessionlist-dialog.hh
 * \author shaoner
 * \brief
 */

#ifndef SESSIONLIST_DIALOG_HH
# define SESSIONLIST_DIALOG_HH

# include <QDialog>
# include <session-parameters.hh>

# include "common/common.hh"

class QSettings;
class QTreeWidgetItem;
class Parameters;
class Network;

namespace Ui
{
	class SessionListDialog;
} // namespace Ui

class NetworkDialog;

# define CONFIG_NETWORK "networks"

class SessionListDialog : public QDialog
{

	Q_OBJECT

public:
	SessionListDialog(QWidget *parent = 0);
	~SessionListDialog();
public:
	const irc::SessionParameters& sessionParams() const;
private:
	void fill_networks();
	void save_network(const Network& network);
	void add_item(const Network& network);
	void edit_item(QTreeWidgetItem* item);
	void init_connection(QTreeWidgetItem* item);
	bool contains(const QString& name);
private slots:
	void on_btnAdd_clicked();
	void on_btnConnect_clicked();
	void on_btnEdit_clicked();
	void on_lstSession_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_btnRemove_clicked();
	void on_edtGblNickname_editingFinished();
	void on_edtGblUsername_editingFinished();
	void on_edtGblRealname_editingFinished();
	void on_edtGblAltnicknames_editingFinished();
private:
	Ui::SessionListDialog* _ui;
	QSettings* _networkSettings;
	NetworkDialog* _networkDialog;
	Parameters* _globalSettings;
	irc::SessionParameters _sessionParams;
};

inline const irc::SessionParameters&
SessionListDialog::sessionParams() const
{
	return _sessionParams;
}

#endif /* !SESSIONLIST_DIALOG_HH */
