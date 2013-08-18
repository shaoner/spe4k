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
 * \file edit-box.hh
 * \author shaoner
 * \brief Input
 */
#ifndef EDIT_BOX_HH
# define EDIT_BOX_HH

# include <QLineEdit>

class QKeyEvent;

namespace Ui
{
	class EditBox;
} // namespace Ui

class EditBox : public QLineEdit
{
	Q_OBJECT

public:
	/// Ctor
	explicit EditBox(QWidget* parent = 0);
	/// Dtor
	~EditBox();
protected:
	void keyPressEvent(QKeyEvent* event);
signals:
	void onTabKey(bool previousTab, int begPos, int& len);
private slots:
	void entered_slot();
private:
	Ui::EditBox* _ui;
	bool _previousTab;
	int _previousPos;
	int _previousLen;
	QList<QString> _history;
	int _historyIdx;
	bool _keepBuffer;
};

#endif /* !EDIT_BOX_HH */
