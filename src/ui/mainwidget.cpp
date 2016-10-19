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

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget), _mainMenu(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    ui->setupUi(this);

    QSettings settings(QS_APP_NAME, QS_APP_NAME);
    settings.beginGroup(QS_BLOCK_WINDOW);
    restoreGeometry(settings.value(QS_ITEM_GEOMETRY).toByteArray());
    settings.endGroup();

    createMenu();
    showTime();

    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &MainWidget::showTime);
    timer->start();
}

MainWidget::~MainWidget()
{
    delete ui;
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

    QColor colorBackground(DEF_BKG_R, DEF_BKG_B, DEF_BKG_G, DEF_BKG_A);
    painter.setBrush(QBrush(colorBackground));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void MainWidget::showTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString timeStr = dt.time().toString("hh:mm:ss");
    QString dateStr = dt.date().toString();

    ui->labTime->setText(timeStr);
    ui->labDate->setText(dateStr);

    ui->labTime->setStyleSheet("QLabel { font-weight: bold; font-size: 32px }");
    ui->labDate->setStyleSheet("QLabel { font-size: 16px }");

    setWindowTitle(timeStr + " - " + dateStr);
}

void MainWidget::createMenu()
{
    QAction* actInfo =  new QAction(tr("Info"), this);
    QAction* actQuit = new QAction(tr("Quit"), this);

    QIcon iconInfo = QIcon::fromTheme("dialog-information", QIcon(":/dialog-information.png"));
    QIcon iconQuit = QIcon::fromTheme("application-exit", QIcon(":/application-exit.png"));

    actInfo->setIcon(iconInfo);
    actQuit->setIcon(iconQuit);

    connect(actQuit, &QAction::triggered, this, &MainWidget::close);
    connect(actInfo, &QAction::triggered, this, &MainWidget::showAbout);

    _mainMenu = new QMenu(this);
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
