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
 * \file option-dialog.hh
 * \author shaoner
 * \brief Dialog to configure all the settings
 */
#ifndef OPTION_DIALOG_HH
#define OPTION_DIALOG_HH

# include <QDialog>

class Parameters;

namespace Ui
{
	class OptionDialog;
	class EventFormat;
} // namespace Ui

class OptionDialog : public QDialog
{

	Q_OBJECT

public:
	static OptionDialog* get();
	static void reset();
private:
	/// Ctor
	OptionDialog();
	/// Dtor
	~OptionDialog();
private slots:
	void on_lstOptions_itemSelectionChanged();
	void on_btnSave_clicked();
	void on_comboEvent_currentIndexChanged(int index);
private:
	static OptionDialog* _instance;
private:
	Ui::OptionDialog* _ui;
	Parameters* _params;
};

#endif /* !OPTION_DIALOG_HH */
