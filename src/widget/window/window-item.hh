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
 * \file window-item.hh
 * \author shaoner
 * \brief Tree item dedicated to windows
 */

#ifndef WINDOW_ITEM_HH
# define WINDOW_ITEM_HH

# include <QTreeWidgetItem>

# include "common/parameters.hh"

class QMdiSubWindow;

class WindowItem : public QTreeWidgetItem
{
public:
	/// Ctor
	explicit WindowItem(const QString& name);
public:
	QMdiSubWindow* subwindow() const;
	void set_subwindow(QMdiSubWindow* subwindow);
	void set_level(LightLevel level);
	void reset_level();
private:
	QMdiSubWindow* _subwindow;
	LightLevel _level;
};

inline QMdiSubWindow*
WindowItem::subwindow() const
{
	return _subwindow;
}

#endif /* !WINDOW_ITEM_HH */
