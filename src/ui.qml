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
import Edupals.N4D 1.0 as N4D
import Edupals.N4D.Agent 1.0 as N4DAgent

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kirigami 2.16 as Kirigami

import QtQuick 2.6
import QtQuick.Controls 2.6 as QQC2
import QtQuick.Window 2.6 
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.0

QQC2.Pane {
    id: main
    
    property bool locked:true
        
    width: 460
    height: 460
    anchors.centerIn: parent
    
    QQC2.Popup {
        id: popup
        x: 0
        y: 0
        width: 430
        height: 430
        modal: true
        focus: true
        anchors.centerIn: parent
        closePolicy: QQC2.Popup.CloseOnEscape | QQC2.Popup.CloseOnPressOutsideParent
        parent: QQC2.Overlay.overlay
        
        QQC2.Overlay.modal: Rectangle {
            color: "#AA000000"
        }
        
        N4DAgent.Login {
            id: loginWidget
            
            showAddress: false
            //address: "https://localhost:9779"
            //user: "netadmin"
            showCancel: true
            trustLocal: true
            //inGroups: ["patata"]
            
            anchors.centerIn: parent
            
            onLogged: {
                //console.log(ticket);
                popup.close();
                main.locked=false;
            }
            
            onCanceled: {
                popup.close();
            }
            
            onAuthenticated: {
                //console.log("passwd:",passwd)
            }
        }
    }
    
    ColumnLayout {
        anchors.fill:parent
        
        QQC2.Button {
            Layout.alignment: Qt.AlignRight
            Layout.minimumWidth: Kirigami.Units.iconSizes.medium
            
            Kirigami.Icon {
                Layout.minimumWidth: Kirigami.Units.iconSizes.medium
                source: main.locked ? "object-locked" : "object-unlocked"
            }
            
            onClicked: {
                popup.open();
            }
        }
            
        QQC2.Label {
            text:"Select resolution to mirror screens"
        }
        
        ListView {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            model: backend.outputsModel
            height: Kirigami.Units.largeSpacing*4

            delegate:QQC2.Label {
                    text: modelData
                }
            
            
            onModelChanged: {
                console.log(model);

                if (model.length<2) {
                    msg.type=Kirigami.MessageType.Warning;
                    msg.text="Expected two outputs"
                    msg.visible=true;
                }
            }
        }
        
        QQC2.ComboBox {
            id: cmbOptions
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            model: backend.optionsModel
            textRole:"name"
            
        }
        
        QQC2.CheckBox {
            enabled: !main.locked
            text: "Apply to all users"
        }
        
        Kirigami.InlineMessage {
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth:true
            Layout.minimumHeight:32
            
            id: msg
            type: Kirigami.MessageType.Error
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignRight
            QQC2.Button {
                text: "Close"
            }
            
            QQC2.Button {
                text: "Apply"
                
                onClicked: {
                    console.log(cmbOptions.currentValue.name)
                    backend.setMode(cmbOptions.currentValue);
                }
            }
        }
    }
}
