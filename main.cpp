/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "src/MainWindow/mainwindow.h"
#include <QtSingleApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QScreen>
#include <QTranslator>
#include <QLocale>
//#include "ukuimenuinterface.h"

int main(int argc, char *argv[])
{
    QtSingleApplication app("ukui-start-menu",argc,argv);
    app.setQuitOnLastWindowClosed(false);

    if(app.isRunning())
    {
        app.sendMessage("raise_window_noop");
        return EXIT_SUCCESS;
    }

    QFile qss(":/data/qss/ukui-menu.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());

//    QGSettings* setting=new QGSettings(QString("org.mate.interface").toLocal8Bit());
//    QString value=setting->get("font-name").toString();
//    QFont font;
//    font.setFamily(value);
//    qApp->setFont(font);

    QString locale = QLocale::system().name();
    QTranslator translator;
    if (locale == "zh_CN"){
        if (translator.load(":/ukui_menu_ch.qm"))
            app.installTranslator(&translator);
        else
            qDebug() << "Load translations file" << locale << "failed!";
    }

    char style[400];
    sprintf(style,"QToolTip{min-height:30px;border-style: none;border:1px solid rgba(255, 255, 255, 0.2);"
                  "padding-left:4px;padding-top:0px;padding-right:4px;padding-bottom:0px;border-radius:3px;"
                  "font-size:14px;color:#ffffff;background-color:%s;}",
            ToolTipBackground);
    qApp->setStyleSheet(style);

    MainWindow w;
    QFileInfo fileInfo(QString("/usr/share/glib-2.0/schemas/org.ukui.panel.settings.gschema.xml"));
    int position=0;
    int panelSize=0;
    if(fileInfo.exists())
    {
        QGSettings* gsetting=new QGSettings(QString("org.ukui.panel.settings").toLocal8Bit());
        position=gsetting->get("panelposition").toInt();
        panelSize=gsetting->get("panelsize").toInt();
    }
    else
    {
        position=0;
        panelSize=46;
    }

    int x=QApplication::primaryScreen()->geometry().x();
    int y=QApplication::primaryScreen()->geometry().y();

    if(position==0)
        w.setGeometry(QRect(x,Style::heightavailable-590,376,590));
    else if(position==1)
        w.setGeometry(QRect(x,panelSize,376,590));
    else if(position==2)
        w.setGeometry(QRect(panelSize,y,376,590));
    else
        w.setGeometry(QRect(Style::widthavailable-376,y,376,590));
    app.setActivationWindow(&w);

    //注释掉，以保证自启动时不显示界面
    w.show();
    w.raise();
    w.activateWindow();
    w.hide();

    return app.exec();
}
