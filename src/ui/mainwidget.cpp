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

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);

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
