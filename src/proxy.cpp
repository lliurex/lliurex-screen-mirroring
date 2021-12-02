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

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusArgument>
#include <QList>
#include <QMap>
#include <QVariant>

#include <iostream>

#include <QDebug>

using namespace lliurex::scrm;
using namespace std;

Option::Option(QString output,QString id,int width,int height,double refresh) : QObject(nullptr)
{
    m_outputs[0]=output;
    m_ids[0]=id;
    m_width=width;
    m_height=height;
    m_refresh=refresh;
}

void Option::append(Option& option)
{
    m_outputs[1]=option.m_outputs[0];
    m_ids[1]=option.m_ids[0];
}

double Option::getValue()
{
    return (m_width*m_height)+m_refresh;
}

Proxy::Proxy(QObject* parent) : QObject(parent)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    
    QDBusMessage msg = QDBusMessage::createMethodCall("org.kde.KScreen",
                                                    "/backend",
                                                    "org.kde.kscreen.Backend",
                                                    "getConfig");
    QDBusMessage reply = connection.call(msg);
    
    if (reply.type() == QDBusMessage::ErrorMessage) {
        cerr<<"Error getting screen config"<<endl;
    }
    else {
        QList<QVariant> args = reply.arguments();
        clog<<"reply "<<args.count()<<endl;
        
        QDBusArgument arg = reply.arguments().at(0).value<QDBusArgument>();
        
        qDebug()<<arg.currentType();
        
        QMap<QString,QVariant> dict = qdbus_cast<QMap<QString,QVariant> >(arg);
        QList<QVariant>::iterator iter;
        QList<QVariant> outputs = qdbus_cast<QList<QVariant> >(dict["outputs"]);
        
        m_outputs.clear();
        
        for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
            QMap<QString,QVariant> output = qdbus_cast<QMap<QString,QVariant> >(*iter);
            if (output["connected"].value<bool>()) {
                qDebug()<<"connected "<<output["name"].value<QString>();
                m_outputs.push_back(output["name"].value<QString>());
                QList<QVariant> modes = qdbus_cast<QList<QVariant> >(output["modes"]);
                QList<QVariant>::iterator miter;
                
                for(miter = modes.begin(); miter != modes.end(); ++miter) {
                    QMap<QString,QVariant> mode = qdbus_cast<QMap<QString,QVariant> >(*miter);
                    QString id = mode["id"].value<QString>();
                    
                    QMap<QString,QVariant> size = qdbus_cast<QMap<QString,QVariant> >(mode["size"]);
                    //qDebug()<<size["width"].value<int>()<<"x"<<size["height"].value<int>();
                    //qDebug()<<id<<":"<<mode["size"].value<QDBusArgument>().currentType();
                }
            }
            
        }
        
        emit outputsModelChanged();
    }
}
