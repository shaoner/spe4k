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
 * \file text-box.hh
 * \author shaoner
 * \brief Output widget i.e. an edit box + a screen
 */

#ifndef TEXT_BOX_HH
# define TEXT_BOX_HH

# include <QWidget>

class EditBox;
class QTextBrowser;

namespace Ui
{
	class TextBox;
} // namespace Ui

class TextBox : public QWidget
{

	Q_OBJECT

public:
	/// Ctor
	explicit TextBox(QWidget* parent = 0);
	/// Dtor
	~TextBox();
public:
    EditBox* input;
    QTextBrowser* screen;
private:
	Ui::TextBox* _ui;
};

#endif /* !TEXT_BOX_HH */
