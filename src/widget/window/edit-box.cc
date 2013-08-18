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
 * \file edit-box.cc
 * \author shaoner
 * \brief
 */

#include "edit-box.hh"

#include <QKeyEvent>

#include "ui_edit-box.h"
#include "common/text-display.hh"

EditBox::EditBox(QWidget *parent) :
	QLineEdit(parent),
	_ui(new Ui::EditBox),
	_previousTab(false),
	_previousPos(0),
	_previousLen(0),
	_historyIdx(0),
	_keepBuffer(false)
{
	_ui->setupUi(this);
	_history.append("");
	connect(this, SIGNAL(returnPressed()), this, SLOT(entered_slot()));
}

EditBox::~EditBox()
{
	delete _ui;
}

void
EditBox::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Tab)
	{
		if (!_previousTab)
		{
			QString line = text();
			int end = cursorPosition();
			if (!end)
				return;
			int start = end - 1;
			for (; start >= 0 && line[start] != ' '; --start);
			if (start)
				++start;
			 _previousPos = start;
			 _previousLen = end - start + 1;
		}
		emit onTabKey(_previousTab, _previousPos, _previousLen);
		_previousTab = true;
	}
	else if (event->key() == Qt::Key_Up)
	{
		// Save current text in the history
		if (_keepBuffer)
		{
			QString cur = text();
			if (!cur.isEmpty())
			{
				_history.append(cur);
				_keepBuffer = false;
			}
		}
		// circular index
		if (_historyIdx == 0)
			_historyIdx = _history.count() - 1;
		else
			--_historyIdx;
		clear();
		setText(_history[_historyIdx]);
	}
	else if (event->key() == Qt::Key_Down)
	{
		// Save current text in the history
		if (_keepBuffer)
		{
			QString cur = text();
			if (!cur.isEmpty())
			{
				_history.append(cur);
				_keepBuffer = false;
			}
		}

		if (_historyIdx >= (_history.count() - 1))
			_historyIdx = 0;
		else
			++_historyIdx;
		clear();
		setText(_history[_historyIdx]);
	}
	else if ((event->modifiers().testFlag(Qt::MetaModifier)) && (event->key() == Qt::Key_C))
	{
		/*
		 * TODO: Insert a special character that stands for a color character.
		 */
	}
	else if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter))
	{
		// Necessary because of commands that delete the parent window
		// If the parent window is destroyed then this is destroyed
		// And it makes keypressEvent to crash
		emit returnPressed();
	}
	else
	{
		QLineEdit::keyPressEvent(event);
		_previousTab = false;
	}
}

void
EditBox::entered_slot()
{
	QString t = text();
	if (_history.last() != t)
		_history.append(t);
	_historyIdx = _history.count();
}
