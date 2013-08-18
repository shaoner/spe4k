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
 * \file mainwindow.cc
 * \author shaoner
 * \brief Main window
 */
#include "main-window.hh"

#include <QTreeWidgetItem>
#include <session-parameters.hh>

#include "ui_main-window.h"
#include "common/command-handler.hh"
#include "dialog/sessionlist-dialog.hh"
#include "dialog/option-dialog.hh"
#include "session-area.hh"
#include "window/window-item.hh"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	_ui(new Ui::MainWindow),
    _sessionListDialog(new SessionListDialog(this)),
    _optionDialog(OptionDialog::get())
{
    _ui->setupUi(this);
    connect(_ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(item_active_slot(QTreeWidgetItem*, int)));
    connect(_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(item_active_slot(QTreeWidgetItem*, int)));
	connect(_ui->stackedWidget, SIGNAL(currentChanged(int)),
			this, SLOT(active_session_slot(int)));
    showMaximized();
    if (_sessionListDialog->exec() == QDialog::Accepted)
    {
        add_session(_sessionListDialog->sessionParams());
    }
}

MainWindow::~MainWindow()
{
    disconnect(_ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(item_active_slot(QTreeWidgetItem*, int)));
    disconnect(_ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(item_active_slot(QTreeWidgetItem*, int)));
    // Destroy session areas
    while (_ui->stackedWidget->count())
    {
        delete _ui->stackedWidget->widget(0);
    }
    // Clean singletons
    OptionDialog::reset();
    CommandHandler::reset();
    Parameters::reset();
    delete _sessionListDialog;
	delete _ui;
}

void
MainWindow::add_session(const irc::SessionParameters& sessionParams)
{
    SessionArea* area = new SessionArea(_ui->treeWidget, sessionParams, _ui->stackedWidget);
	connect(area, SIGNAL(onClose(SessionArea*)),
			this, SLOT(remove_session(SessionArea*)));
    connect(area, SIGNAL(onSessionConnect(SessionArea*)), this, SLOT(session_connect_slot(SessionArea*)));
    connect(area, SIGNAL(onSessionDisconnect(SessionArea*)), this, SLOT(session_disconnect_slot(SessionArea*)));

    _ui->stackedWidget->addWidget(area);
    _ui->stackedWidget->setCurrentWidget(area);
}

void
MainWindow::remove_session(SessionArea* area)
{
	int idx = -1;
	if ((idx = _ui->stackedWidget->indexOf(area)) != -1)
	{
		_ui->treeWidget->takeTopLevelItem(idx);
		_ui->stackedWidget->removeWidget(area);
	}
	delete area;
}

void
MainWindow::session_connect_slot(SessionArea* area)
{
    // There should be at least one sessionarea in the stack
    SessionArea* curArea = static_cast<SessionArea*>(_ui->stackedWidget->currentWidget());
    if (area == curArea)
    {
        _ui->actionTbConnection->setChecked(true);
    }
}

void
MainWindow::session_disconnect_slot(SessionArea* area)
{
    SessionArea* curArea = static_cast<SessionArea*>(_ui->stackedWidget->currentWidget());
    if (area == curArea)
    {
        _ui->actionTbConnection->setChecked(false);
    }
}

void
MainWindow::item_active_slot(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);
    if (!item)
        return;
    WindowItem* subItem = 0;
    SessionArea* sessionArea = 0;
    Q_ASSERT(subItem != 0);

    int serverId = -1;
    QTreeWidgetItem* parent = item->parent();
	// We find the top level tree item
    if (!parent)
    {
        serverId = _ui->treeWidget->indexOfTopLevelItem(item);
    }
    else
    {
        serverId = _ui->treeWidget->indexOfTopLevelItem(parent);
    }
    subItem = static_cast<WindowItem*>(item);
	// Now we can set the right session as active
    _ui->stackedWidget->setCurrentIndex(serverId);
	sessionArea = static_cast<SessionArea*>(_ui->stackedWidget->widget(serverId));
    sessionArea->setActiveSubWindow(subItem->subwindow());
}

void
MainWindow::active_session_slot(int index)
{
	if (index < 0)
	{
        _ui->actionTbConnection->setChecked(false);
        return;
	}
    SessionArea* area = static_cast<SessionArea*>(_ui->stackedWidget->widget(index));
    _ui->actionTbConnection->setChecked(area->session().is_connected());
}

void
MainWindow::on_actionTabs_toggled(bool checked)
{
    QMdiArea::ViewMode viewMode = QMdiArea::SubWindowView;
    if (checked)
        viewMode = QMdiArea::TabbedView;
    for (int i = 0, len = _ui->treeWidget->topLevelItemCount(); i < len; ++i)
    {
        SessionArea* sessionArea = static_cast<SessionArea*>(_ui->stackedWidget->widget(i));
        sessionArea->setViewMode(viewMode);
    }
}

void
MainWindow::on_actionCascade_triggered()
{
    for (int i = 0, len = _ui->treeWidget->topLevelItemCount(); i < len; ++i)
    {
        SessionArea* sessionArea = static_cast<SessionArea*>(_ui->stackedWidget->widget(i));
        sessionArea->cascadeSubWindows();
    }
}

void
MainWindow::on_actionTile_triggered()
{
    for (int i = 0, len = _ui->treeWidget->topLevelItemCount(); i < len; ++i)
    {
        SessionArea* sessionArea = static_cast<SessionArea*>(_ui->stackedWidget->widget(i));
        sessionArea->tileSubWindows();
    }
}

void
MainWindow::on_actionTree_toggled(bool checked)
{
    _ui->dockWidget->setVisible(checked);
}



void
MainWindow::on_actionTbConnection_triggered()
{
    SessionArea* curArea = static_cast<SessionArea*>(_ui->stackedWidget->currentWidget());
    if (_ui->actionTbConnection->isChecked())
    {
        if (curArea)
        {
            curArea->session().start();
        }
        else
        {
            _ui->actionTbConnection->setChecked(false);
            if (_sessionListDialog->exec() == QDialog::Accepted)
            {
                add_session(_sessionListDialog->sessionParams());
            }
        }
    }
    else
    {
        if (curArea)
        {
            curArea->session().stop();
        }
    }

}

void
MainWindow::on_actionServers_triggered()
{
    if (_sessionListDialog->exec() == QDialog::Accepted)
    {
        add_session(_sessionListDialog->sessionParams());
    }
}
void
MainWindow::on_actionIdentities_triggered()
{
}

void
MainWindow::on_actionOptions_triggered()
{
    _optionDialog->show();
}
