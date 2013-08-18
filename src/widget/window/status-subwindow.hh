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
 * \file status-subwindow.hh
 * \author shaoner
 * \brief Override Mdisubwindow to handle the close event
 */

#ifndef STATUS_SUBWINDOW_HH
# define STATUS_SUBWINDOW_HH

# include <QMdiSubWindow>
# include <QCloseEvent>

class StatusSubWindow : public QMdiSubWindow
{
public:
	StatusSubWindow(QWidget* parent);
protected:
	virtual void closeEvent(QCloseEvent* event);
};

inline
StatusSubWindow::StatusSubWindow(QWidget* parent) :
	QMdiSubWindow(parent)
{
}

inline void
StatusSubWindow::closeEvent(QCloseEvent* event)
{
	mdiArea()->close();
	event->ignore();
}

#endif /* !STATUS_SUBWINDOW_HH */
