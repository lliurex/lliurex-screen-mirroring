/*
 * Copyright (C) 2021 Lliurex project
 *
 * Author:
 *  Enrique Medina Gremaldos <quiqueiii@gmail.com>
 *
 * Source:
 *  https://github.com/lliurex/lliurex-screen-mirroring
 *
 * This file is a part of lliurex screen mirroring
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.16 as Kirigami

import QtQuick 2.6
import QtQuick.Controls 2.6 as QQC2
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.0

QQC2.Pane {
    id: main
    
    visible:true
    width: 400
    height: 360
    anchors.centerIn: parent
    
    ColumnLayout {
        anchors.fill:parent
        
        QQC2.Label {
            text:"Select resolution to mirror screens"
        }
        
        QQC2.ComboBox {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            model: backend.outputsModel
        }
        
        QQC2.CheckBox {
            text: "Apply to all users"
        }
        
        RowLayout {
            QQC2.Button {
                text: "Close"
            }
            
            QQC2.Button {
                text: "Apply"
            }
        }
    }
}
