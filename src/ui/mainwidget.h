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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QMenu>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void showTime();

private:
    Ui::MainWidget *_ui;
    QMenu* _mainMenu;

    bool _displayDate;
    bool _displaySeconds;

    void loadSettings();
    void createMenu();
    void showContextMenu(const QPoint &pos);
    void showSettings();
    void showAbout();
};

#endif // MAINWIDGET_H
