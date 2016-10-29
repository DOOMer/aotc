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
#include <QtGui/QPalette>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSlider>

#include "globals.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::Settings)
{
    _ui->setupUi(this);

    int frameStyle = QFrame::Sunken | QFrame::Panel;
    _ui->labBkgColorData->setFrameStyle(frameStyle);

    connect(_ui->buttonBox, &QDialogButtonBox::clicked, this, &Settings::clickMapper);
    connect(_ui->sliderTransparency, &QSlider::valueChanged, this, &Settings::selectTransparency);
    connect(_ui->btnChangeBkgColor, &QPushButton::clicked, this, &Settings::selectBackgroundColor);

    load();
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
    _ui->sliderTransparency->setValue(settings.value(QS_ITEM_TRABSPARENCY, DEF_BKG_A).toInt());

    QColor bkgColor = settings.value(QS_ITEM_BKG_CLORO, QColor(DEF_BKG_R, DEF_BKG_G, DEF_BKG_B)).value<QColor>();
    setupBkgColorLabel(bkgColor);

    settings.endGroup();
}

void Settings::save()
{
    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_MAIN);
    settings.setValue(QS_ITEM_DISPLAY_DATE, _ui->cbxDisplayDate->isChecked());
    settings.setValue(QS_ITEM_DISPLAY_SECS, _ui->cbxDisplaySeconds->isChecked());
    settings.setValue(QS_ITEM_TRABSPARENCY, _ui->sliderTransparency->value());

    QPalette pal = _ui->labBkgColorData->palette();
    QColor bkgColor = pal.background().color();
    settings.setValue(QS_ITEM_BKG_CLORO, bkgColor);

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
        settings.setValue(QS_ITEM_TRABSPARENCY, DEF_BKG_A);

        QColor bkgColor(DEF_BKG_R, DEF_BKG_G, DEF_BKG_B);
        settings.setValue(QS_ITEM_BKG_CLORO, bkgColor);
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

void Settings::selectTransparency(quint8 value)
{
    _ui->labTransparencySelected->setText(QString::number(value));
}

void Settings::selectBackgroundColor()
{
    QColor defColor = _ui->labBkgColorData->palette().background().color();
    QColor color = QColorDialog::getColor(defColor, this, tr("Select background color"));

    if (color.isValid()) {
        setupBkgColorLabel(color);
    }
}

void Settings::setupBkgColorLabel(const QColor &color)
{
    _ui->labBkgColorData->setText(color.name());
    _ui->labBkgColorData->setPalette(QPalette(color));
    _ui->labBkgColorData->setAutoFillBackground(true);
}
