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

#ifndef LLIUREX_SCREEN_MIRRORING_PROXY
#define LLIUREX_SCREEN_MIRRORING_PROXY

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMap>
#include <QPoint>

namespace lliurex
{
    namespace scrm
    {
        class Option: public QObject
        {
            Q_OBJECT
            
            public:
                
            Q_PROPERTY(QString name READ getName CONSTANT)
            
            Option(QString output,QString id,int width,int height,double refresh);
            
            QString getName();
            void append(Option& option);
            
            double getValue();
            
            bool match() const
            {
                return m_count==2;
            }
            
            QString outputId(int index)
            {
                return m_ids[index];
            }

            QString outputName(int index)
            {
                return m_outputs[index];
            }

            protected:
            
            int m_width;
            int m_height;
            double m_refresh;
            int m_count;
            
            QString m_outputs[2];
            QString m_ids[2];
            
        };
        
        class Proxy: public QObject
        {
            Q_OBJECT
            
            public:
                
            Q_PROPERTY(QStringList outputsModel MEMBER m_outputs NOTIFY outputsModelChanged)
            Q_PROPERTY(QList<QObject*> optionsModel MEMBER m_options NOTIFY optionsModelChanged)
            
            Proxy(QObject* parent = nullptr);
            ~Proxy();
            
            Q_INVOKABLE void setMode(Option* option,bool revert = false);
            Q_INVOKABLE void revert();
            Q_INVOKABLE void confirm(Option* option);
            Q_INVOKABLE void applyToAll(QString ticket);
            
            protected:
            
            QStringList m_outputs;
            QList<QObject*> m_options;
            QMap<QString,QVariant> configuration;
            QMap<QString,QString> currentId;
            QMap<QString,QPoint> currentPos;
            
            signals:
                
            void outputsModelChanged();
            void optionsModelChanged();
            void error(int what);
            
        };
    }
}

#endif
