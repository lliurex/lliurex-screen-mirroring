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
    
    Connections {
        target: backend
        
        function onError(what)
        {
            console.log("error:",what);
        }
    }
    
    QQC2.Popup {
        id: n4dPopup
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
                n4dPopup.close();
                main.locked=false;
            }
            
            onCanceled: {
                n4dPopup.close();
            }
            
            onAuthenticated: {
                //console.log("passwd:",passwd)
            }
        }
    }
    
    QQC2.Popup {
        id: confirmPopup
        x: 0
        y: 0
        width: 430
        height: 130
        modal: true
        focus: true
        anchors.centerIn: parent
        closePolicy: QQC2.Popup.CloseOnEscape | QQC2.Popup.CloseOnPressOutsideParent
        parent: QQC2.Overlay.overlay
        property int timeout: 15
        
        onOpened: {
            timeout=15;
            timer.start();
        }
        
        onClosed: {
            timer.stop();
        }
        
        Timer {
            id: timer
            interval: 1000
            repeat: true
            
            
            onTriggered: {
                confirmPopup.timeout = confirmPopup.timeout - 1;
                if (confirmPopup.timeout==0) {
                    timer.stop();
                    confirmPopup.close();
                    backend.revert();
                }
            }
        }
        
        QQC2.Overlay.modal: Rectangle {
            color: "#AA000000"
        }
        
        ColumnLayout {
            anchors.fill:parent
            QQC2.Label {
                Layout.alignment: Qt.AlignCenter
                text:i18nd("lliurex-screen-mirroring","Did it work?");
            }
            
            QQC2.Label {
                Layout.alignment: Qt.AlignCenter
                text:i18nd("lliurex-screen-mirroring","Reverting in ")+confirmPopup.timeout;
            }
            
            
            RowLayout {
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                QQC2.Button {
                    text:i18nd("lliurex-screen-mirroring","Revert");
                    
                    onClicked: {
                        confirmPopup.close();
                        backend.revert();
                    }
                }
                
                QQC2.Button {
                    text:i18nd("lliurex-screen-mirroring","Apply");
                    
                    onClicked: {
                        confirmPopup.close();
                        //TODO:N4D call here
                        Qt.exit(0);
                    }
                }
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
                n4dPopup.open();
            }
        }
            
        QQC2.Label {
            text:i18nd("lliurex-screen-mirroring","Select resolution to mirror screens");
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

                switch (model.length) {
                    case 0:
                        msg.type=Kirigami.MessageType.Error;
                        msg.text=i18nd("lliurex-screen-mirroring","Failed to retrieve screen settings");
                        msg.visible=true;
                    break;
                    
                    case 1:
                        msg.type=Kirigami.MessageType.Warning;
                        msg.text=i18nd("lliurex-screen-mirroring","Expected two outputs");
                        msg.visible=true;
                    break;
                    
                    default:
                        msg.visible=false;
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
            //enabled: !main.locked
            enabled: false
            text: i18nd("lliurex-screen-mirroring","Apply to all users");
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
                text: i18nd("lliurex-screen-mirroring","Close");
                onClicked: {
                    //confirmPopup.open();
                    Qt.exit(0);
                }
            }
            
            QQC2.Button {
                id: btnApply
                text: i18nd("lliurex-screen-mirroring","Apply");
                enabled: !msg.visible
                
                onClicked: {
                    //console.log(cmbOptions.currentValue.name)
                    backend.setMode(cmbOptions.currentValue);
                    confirmPopup.open();
                }
            }
        }
    }
}
