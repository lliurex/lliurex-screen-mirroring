/*
    Lliurex screen mirroring

    Copyright (C) 2021  Enrique Medina Gremaldos <quiqueiii@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "proxy.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QIcon>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QScreen>

#include <iostream>

using namespace lliurex::scrm;
using namespace std;

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);
    QApplication::setApplicationName("LliureX Screen Mirroring");
    QApplication::setApplicationVersion("1.0");
    
    QQuickView *view = new QQuickView;
    view->setIcon(QIcon::fromTheme("monitor"));
    view->setMinimumSize(QSize(400,360));
    view->setMaximumSize(QSize(400,360));
    
    Proxy* proxy = new Proxy();
    
    QQmlContext* ctxt = view->rootContext();
    QObject::connect(ctxt->engine(),&QQmlEngine::exit,&app,&QCoreApplication::exit);
    ctxt->setContextProperty("backend",proxy);
    view->setSource(QUrl(QStringLiteral("qrc:/ui.qml")));
    
    view->show();
    
    // center on screen
    QScreen* screen = view->screen();
    uint32_t x = screen->geometry().width()/2 - view->frameGeometry().width()/2;
    uint32_t y = screen->geometry().height()/2 - view->frameGeometry().height()/2;
    view->setPosition(x,y);
    
    return app.exec();
}
