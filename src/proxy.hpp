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

namespace lliurex
{
    namespace scrm
    {
        class Option: public QObject
        {
            Q_OBJECT
            
            public:
                
            Q_PROPERTY(QString name MEMBER m_name CONSTANT)
            
            Option(QString output,QString id,int width,int height,double refresh);
            
            void append(Option& option);
            
            double getValue();
            
            protected:
            
            int m_width;
            int m_height;
            double m_refresh;
            
            QString m_name;
            
            QString m_outputs[2];
            QString m_ids[2];
            
        };
        
        class Proxy: public QObject
        {
            Q_OBJECT
            
            public:
                
            Q_PROPERTY(QStringList outputsModel MEMBER m_outputs NOTIFY outputsModelChanged)
            
            Proxy(QObject* parent = nullptr);
            
            protected:
            
            QStringList m_outputs;
            
            signals:
                
            void outputsModelChanged();
            
        };
    }
}

#endif
