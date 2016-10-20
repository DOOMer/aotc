/***************************************************************************
* Copyright (C) 2016 by Artem 'DOOMer' Galichkin *
* doomer3d@gmail.com *
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with this program; if not, write to the *
* Free Software Foundation, Inc., *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
***************************************************************************/

#include "settings.h"
#include "ui_settings.h"

#include <QtCore/QSettings>
#include <QtWidgets/QMessageBox>

#include "globals.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Settings)
{
    _ui->setupUi(this);
    load();

    connect(_ui->buttonBox, &QDialogButtonBox::clicked, this, &Settings::clickMapper);
}

Settings::~Settings()
{
    delete _ui;
}

void Settings::load()
{
    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_MAIN);
    _ui->cbxDisplayDate->setChecked(settings.value(QS_ITEM_DISPLAY_DATE, DEF_DISPLAY_DATE).toBool());
    _ui->cbxDisplaySeconds->setChecked(settings.value(QS_ITEM_DISPLAY_SECS, DEF_DISPLAY_SECS).toBool());
    settings.endGroup();
}

void Settings::save()
{
    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_MAIN);
    settings.setValue(QS_ITEM_DISPLAY_DATE, _ui->cbxDisplayDate->isChecked());
    settings.setValue(QS_ITEM_DISPLAY_SECS, _ui->cbxDisplaySeconds->isChecked());
    settings.endGroup();
}

void Settings::defaults()
{
    QMessageBox msg;
    msg.setWindowTitle(tr("Warning"));
    msg.setText(tr("Do you want reset settings to default values?"));
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    int result = msg.exec();

    if (result == QMessageBox::Yes) {
        QSettings settings(QS_APP_NAME, QS_APP_NAME);
        settings.beginGroup(QS_BLOCK_MAIN);
        settings.setValue(QS_ITEM_DISPLAY_DATE, DEF_DISPLAY_DATE);
        settings.setValue(QS_ITEM_DISPLAY_SECS, DEF_DISPLAY_SECS);
        settings.endGroup();

        accept();
    }
}

void Settings::clickMapper(QAbstractButton *button)
{
    if (_ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults) {
        defaults();
    }
    if (_ui->buttonBox->standardButton(button) == QDialogButtonBox::Save) {
        save();
    }
}
