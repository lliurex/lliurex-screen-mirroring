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

struct optnode_t {
    Option* me;
    optnode_t* left;
    optnode_t* right;
} ;

static void add_node(optnode_t* node,Option* opt)
{
    double delta = opt->getValue() - node->me->getValue();

    if (std::abs(delta)<0.1) {

        node->me->append(*opt);
        return;
    }

    if (delta > 0.0) {
        if (node->right==nullptr) {
            optnode_t* q = new optnode_t;
            q->left=nullptr;
            q->right=nullptr;
            q->me=opt;
            node->right = q;
        }
        else {
            add_node(node->right,opt);
        }
    }
    else {
        
        if (node->left==nullptr) {
            optnode_t* q = new optnode_t;
            q->left=nullptr;
            q->right=nullptr;
            q->me=opt;
            node->left = q;
        }
        else {
            add_node(node->left,opt);
        }
        
    }
}

static void delete_nodes(optnode_t* node)
{
    if (node->left!=nullptr) {
        delete_nodes(node->left);
    }
    
    if (node->right!=nullptr) {
        delete_nodes(node->right);
    }
    
    delete node;
}

static void traverse_nodes(optnode_t* node,QList<QObject*>& list)
{
    if (node->right!=nullptr) {
        traverse_nodes(node->right,list);
    }
    if (node->me->match()) {
        list.push_back(node->me);
        qDebug()<<node->me->getName();
    }

    if (node->left!=nullptr) {
        traverse_nodes(node->left,list);
    }
}

Option::Option(QString output,QString id,int width,int height,double refresh) : QObject(nullptr)
{
    m_outputs[0]=output;
    m_ids[0]=id;
    m_width=width;
    m_height=height;
    m_refresh=refresh;
    m_count=1;
}

QString Option::getName()
{
    QString tmp=QString("%1x%2 %3 Hz").arg(m_width).arg(m_height).arg(m_refresh);
    //qDebug()<<tmp;
    return tmp;
}

void Option::append(Option& option)
{
    if (option.m_outputs[0]!=m_outputs[0]) {

        m_outputs[1]=option.m_outputs[0];
        m_ids[1]=option.m_ids[0];
        m_count=2;

        qDebug()<<"match "<<m_outputs[0]<<":"<<m_ids[0]<<" and "<<m_outputs[1]<<":"<<m_ids[1];
    }
}

double Option::getValue()
{
    return (m_width*4.0)+(m_height*2.0)+(m_refresh);
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
        
        this->configuration = reply.arguments().at(0).value<QDBusArgument>();
        
        qDebug()<<configuration.currentType();
        
        QMap<QString,QVariant> dict = qdbus_cast<QMap<QString,QVariant> >(configuration);
        QList<QVariant>::iterator iter;
        QList<QVariant> outputs = qdbus_cast<QList<QVariant> >(dict["outputs"]);
        
        m_outputs.clear();
        m_options.clear();
        
        optnode_t* node = nullptr;
        
        for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
            QMap<QString,QVariant> output = qdbus_cast<QMap<QString,QVariant> >(*iter);
            if (output["connected"].value<bool>()) {
                QString outputName = output["name"].value<QString>();
                qDebug()<<"connected "<< outputName;
                m_outputs.push_back(outputName);
                QList<QVariant> modes = qdbus_cast<QList<QVariant> >(output["modes"]);
                QList<QVariant>::iterator miter;
                
                for(miter = modes.begin(); miter != modes.end(); ++miter) {
                    QMap<QString,QVariant> mode = qdbus_cast<QMap<QString,QVariant> >(*miter);
                    QString id = mode["id"].value<QString>();
                    
                    
                    QMap<QString,QVariant> size = qdbus_cast<QMap<QString,QVariant> >(mode["size"]);
                    //qDebug()<<"id:"<<id;
                    //qDebug()<<mode["refreshRate"];
                    //qDebug()<<size;
                    
                    Option* opt = new Option(outputName,id,
                                                   size["width"].value<long>(),
                                                   size["height"].value<long>(),
                                                   mode["refreshRate"].value<double>());
                    
                    if (node == nullptr) {
                        node = new optnode_t;
                        node->me = opt;
                        node->left=nullptr;
                        node->right=nullptr;
                    }
                    else {
                        add_node(node,opt);
                    }
                    
                    //qDebug()<<size["width"].value<int>()<<"x"<<size["height"].value<int>();
                    //qDebug()<<id<<":"<<mode["size"].value<QDBusArgument>().currentType();
                }
            }
            
        }
        
        traverse_nodes(node,m_options);
        
        emit outputsModelChanged();
        emit optionsModelChanged();
    }
}

void Proxy::setMode(Option* option)
{
    qDebug()<<"setting mode "<<option->outputName(0)<<":"<<option->outputId(0)<<" and "<<option->outputName(1)<<":"<<option->outputId(1);
    
    QMap<QString,QVariant> dict = qdbus_cast<QMap<QString,QVariant> >(configuration);
    QList<QVariant>::iterator iter;
    QList<QVariant> outputs = qdbus_cast<QList<QVariant> >(dict["outputs"]);
    
    m_outputs.clear();
    m_options.clear();
    
    optnode_t* node = nullptr;
    
    for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
        QMap<QString,QVariant> output = qdbus_cast<QMap<QString,QVariant> >(*iter);
        QString outputName = output["name"].value<QString>();
        
        if (outputName==option->outputName(0)) {
            output["currentModeId"]=option->outputId(0);
        }
        
        if (outputName==option->outputName(1)) {
            output["currentModeId"]=option->outputId(1);
        }
    }

}
