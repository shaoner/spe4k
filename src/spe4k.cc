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
 * \file spe4k.cc
 * \author shaoner
 * \brief Entry point
 */
#include <QtGui/QApplication>
#include "widget/main-window.hh"

int main(int argc, char* argv[])
{
	int exitStatus = 0;
    QApplication a(argc, argv);
    MainWindow w;
	w.show();
	exitStatus = a.exec();
	return exitStatus;
}
