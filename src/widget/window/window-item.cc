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
 * \file window-item.cc
 * \author shaoner
 * \brief Tree item dedicated to windows
 */

#include "window-item.hh"

WindowItem::WindowItem(const QString& name) :
    _level(LIGHT_LVL_NONE)
{
	setText(0, name);
}

void
WindowItem::set_subwindow(QMdiSubWindow* subwindow)
{
	_subwindow = subwindow;
}

void
WindowItem::set_level(LightLevel level)
{
	// Colorize the item only if the message is more important
	if (level > _level)
	{
		_level = level;
		QColor col = Parameters::get()->levelColor(_level);
		setTextColor(0, col);
	}
}

void
WindowItem::reset_level()
{
    _level = LIGHT_LVL_NONE;
    QColor col = Parameters::get()->levelColor(_level);
	setTextColor(0, col);
}
