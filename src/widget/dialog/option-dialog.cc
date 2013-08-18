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
 * \file option-dialog.cc
 * \author shaoner
 * \brief
 */

#include "option-dialog.hh"

#include "ui_option-dialog.h"
#include "color-label.hh"
#include "event-display.hh"
#include "common/parameters.hh"

OptionDialog* OptionDialog::_instance = 0;

OptionDialog*
OptionDialog::get()
{
    if (!_instance)
        _instance = new OptionDialog();
    return _instance;
}

void
OptionDialog::reset()
{
    delete _instance;
    _instance = 0;
}

OptionDialog::OptionDialog() :
    QDialog(0),
    _ui(new Ui::OptionDialog),
    _params(Parameters::get())
{
    _ui->setupUi(this);

//    QColor col = QColorDialog::getColor(Qt::white, this);
//    if(col.isValid()) {
//        QString qss = QString("background-color: %1").arg(col.name());
//        _ui->pushButton->setStyleSheet(qss);
//    }
    _ui->chkRejoinOnKick->setChecked(_params->get<bool>("rejoin_onkick"));
    _ui->chkReconnectOnKill->setChecked(_params->get<bool>("reconnect_onkill"));
    _ui->chkCloseOnDisconnect->setChecked(_params->get<bool>("closewins_ondisconnect"));
    _ui->chkRejoinOnConnect->setChecked(_params->get<bool>("rejoin_onconnect"));
    _ui->chkJoinOnInvite->setChecked(_params->get<bool>("join_oninvite"));
    _ui->chkCloseOnPart->setChecked(_params->get<bool>("closewin_onpart"));
    _ui->spnReconDelay->setValue(_params->get<int>("reconnect_delay"));
    _ui->spnReconRetry->setValue(_params->get<int>("reconnect_retry"));
    _ui->chkTimestamp->setChecked(_params->get<bool>("Display/timestamp"));
    _ui->edtTimestamp->setText(_params->get<QString>("Display/timestamp_format"));

    for (int i = 0; i < _ui->comboEvent->count(); ++i)
    {
        _ui->stackEventFormat->addWidget(new EventDisplay((DisplayEventType)i));
    }

    for (int i = 1; i < 16; ++i)
    {
        _ui->grpColors->layout()->addWidget(new ColorLabel(i, _ui->grpColors));
    }
}

OptionDialog::~OptionDialog()
{
    delete _ui;
}

void
OptionDialog::on_lstOptions_itemSelectionChanged()
{
    int idx = _ui->lstOptions->currentRow();
    if (idx > -1)
    {
        _ui->stackedWidget->setCurrentIndex(idx);
    }
}

void OptionDialog::on_btnSave_clicked()
{
    _params->set("rejoin_onkick", _ui->chkRejoinOnKick->isChecked());
    _params->set("reconnect_onkill", _ui->chkReconnectOnKill->isChecked());
    _params->set("closewins_ondisconnect", _ui->chkCloseOnDisconnect->isChecked());
    _params->set("rejoin_onconnect", _ui->chkRejoinOnConnect->isChecked());
    _params->set("join_oninvite", _ui->chkRejoinOnKick->isChecked());
    _params->set("closewin_onpart", _ui->chkCloseOnPart->isChecked());
    _params->set("reconnect_delay", _ui->spnReconDelay->value());
    _params->set("reconnect_retry", _ui->spnReconRetry->value());
    _params->set("Display/timestamp", _ui->chkTimestamp->isChecked());
    _params->set("Display/timestamp_format", _ui->edtTimestamp->text());

    for (int i = 0; i < _ui->stackEventFormat->count(); ++i)
    {
        EventDisplay* evtFmt = static_cast<EventDisplay*>(_ui->stackEventFormat->widget(i));
        evtFmt->save();
    }
    accept();
}

void
OptionDialog::on_comboEvent_currentIndexChanged(int index)
{
    _ui->stackEventFormat->setCurrentIndex(index);
}
