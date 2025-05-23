//============================================================================
// Name        : D0sag3.cpp
// Author      : Antonius Torode
// Version     : 1.005
// Copyright   : Copywrite Antonius Torode 2014
// Description : D0sag3 Command in C++, Ansi-style
//============================================================================

#include <iostream>
#include "D3CProgramConstants.h"
#include "QApplication"
#include "d3cwindow.h"
#include "wtypes.h"

using namespace std;

D3CProgramConstants::D3CProgramConstants() : programVersion("1.101"){
    // TODO Auto-generated constructor stub
}

D3CProgramConstants::~D3CProgramConstants(){

}

QString D3CProgramConstants::getProgramVersion(){
    return programVersion;
}

QString D3CProgramConstants::getD3CPromptStart(){
    QString Qtext =
            "DDDDDD        000000      SSSSSS     AAAAAA       GGGGGG   333333""\n"
            "DD       DD    00        00  SS             AA         AA  GG                         33""\n"
            "DD         DD  00        00  SS             AA         AA  GG                         33""\n"
            "DD           D  00        00     SSSSS    AAAAAAAA  GG      GG        3333""\n"
            "DD         DD  00        00             SS  AA         AA  GG        GG            33""\n"
            "DD       DD    00        00             SS  AA         AA  GG        GG            33""\n"
            "DDDDDD        000000      SSSSSS   AA         AA    GGGGGG    333333""\n"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""\n"
            "~~~~~~~~~~~   d0sag3 Command Initialized   ~~~~~~~~~~~""\n"
            "~~~~~~~~~~~~~~  cmd Version: " + programVersion + "  ~~~~~~~~~~~~~~""\n"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~""\n"
            "~~~~~~~~~~  Type 'help' for a list of commands  ~~~~~~~~~~""\n"
            "\n"
            "Enter Command: ""\n";
    return Qtext;
}

