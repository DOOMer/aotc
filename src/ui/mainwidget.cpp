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
#include <QtCore/QTimer>

#include <QtWidgets/QAction>

#include <QtGui/QIcon>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget), _mainMenu(nullptr)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

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

void MainWidget::showTime()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString timeStr = dt.time().toString("hh:mm:ss");
    QString dateStr = dt.date().toString();

    ui->labTime->setText(timeStr);
    ui->labDate->setText(dateStr);

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
