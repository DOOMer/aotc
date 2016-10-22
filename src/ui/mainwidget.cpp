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


#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtCore/QDateTime>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>

#include <QtGui/QIcon>

#include "globals.h"
#include "ui/settings.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::MainWidget), _mainMenu(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    _ui->setupUi(this);

    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_WINDOW);
    restoreGeometry(settings.value(QS_ITEM_GEOMETRY).toByteArray());
    settings.endGroup();

    loadSettings();

    createMenu();
    showTime();

    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWidget::showTime);
    timer->start();
}

MainWidget::~MainWidget()
{
    delete _ui;
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_WINDOW);
    settings.setValue(QS_ITEM_GEOMETRY, saveGeometry());
    settings.endGroup();

    QWidget::closeEvent(event);
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    QColor colorBackground(DEF_BKG_R, DEF_BKG_B, DEF_BKG_G, _transparency);
    painter.setBrush(QBrush(colorBackground));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void MainWidget::showTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString timeStr;

    if (_displaySeconds) {
        timeStr = dt.time().toString("hh:mm:ss");
    } else {
        timeStr = dt.time().toString("hh:mm");
    }

    QString dateStr = dt.date().toString();

    _ui->labTime->setText(timeStr);
    _ui->labDate->setText(dateStr);

    _ui->labTime->setStyleSheet("QLabel { font-weight: bold; font-size: 32px }");
    _ui->labDate->setStyleSheet("QLabel { font-size: 16px }");

    setWindowTitle(timeStr + " - " + dateStr);
}

void MainWidget::loadSettings()
{
    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_MAIN);
    _displayDate = settings.value(QS_ITEM_DISPLAY_DATE, DEF_DISPLAY_DATE).toBool();
    _displaySeconds = settings.value(QS_ITEM_DISPLAY_SECS, DEF_DISPLAY_SECS).toBool();
    _transparency = settings.value(QS_ITEM_TRABSPARENCY, DEF_BKG_A).toInt();
    settings.endGroup();

    _ui->labDate->setVisible(_displayDate);
}

void MainWidget::createMenu()
{
    QAction* actSettings =  new QAction(tr("Settings"), this);
    QAction* actInfo =  new QAction(tr("Info"), this);
    QAction* actQuit = new QAction(tr("Quit"), this);

    QIcon iconSettings = QIcon::fromTheme("preferences-desktop", QIcon(":/preferences-desktop.png"));
    QIcon iconInfo = QIcon::fromTheme("dialog-information", QIcon(":/dialog-information.png"));
    QIcon iconQuit = QIcon::fromTheme("application-exit", QIcon(":/application-exit.png"));

    actSettings->setIcon(iconSettings);
    actInfo->setIcon(iconInfo);
    actQuit->setIcon(iconQuit);

    connect(actSettings, &QAction::triggered, this, &MainWidget::showSettings);
    connect(actQuit, &QAction::triggered, this, &MainWidget::close);
    connect(actInfo, &QAction::triggered, this, &MainWidget::showAbout);

    _mainMenu = new QMenu(this);
    _mainMenu->addAction(actSettings);
    _mainMenu->addSeparator();
    _mainMenu->addAction(actInfo);
    _mainMenu->addSeparator();
    _mainMenu->addAction(actQuit);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &MainWidget::customContextMenuRequested, this, &MainWidget::showContextMenu);
}

void MainWidget::showContextMenu(const QPoint &pos)
{
    _mainMenu->exec(mapToGlobal(pos));
}

void MainWidget::showSettings()
{
    Settings* dlg = new Settings;

    int result = dlg->exec();

    if (result == QDialog::Accepted) {
        loadSettings();
    }

    delete dlg;
}

void MainWidget::showAbout()
{
    QString title = tr("About");
    QString version = QStringLiteral(VERSION_MAJOR) + "." + QStringLiteral(VERSION_MINOR);
    QString info = QStringLiteral("<b>Always on top clock</b> - v") + version + QStringLiteral("<hr>") + QStringLiteral("Simple clock utility") + QStringLiteral("<br /><br />") + QStringLiteral("Copyright (c) 20016, by DOOMer");

    QMessageBox msg;

    msg.setWindowTitle(title);
    msg.setText(info);
    msg.setIcon(QMessageBox::Information);
    msg.setWindowIcon(QIcon::fromTheme("dialog-information", QIcon(":/dialog-information.png")) );

    msg.exec();
}
