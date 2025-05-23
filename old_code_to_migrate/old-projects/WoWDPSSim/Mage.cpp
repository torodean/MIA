#define WINVER 0x0500
#include <windows.h>
#include "Mage.h"

INPUT ip;

Mage::Mage() : GCD(1.5), frostBoltCast(2.0), frostFireBoltCast(0.0), globalTimer(0.0),
    iceNovaCD(25.0), iceNovaTimer(0.0), frozenOrbCD(60.0), frozenOrbTimer(0.0),
    mirrorImageTimer(0.0), icyVeinsTimer(0.0), mirrorImageCD(120.0), icyVeinsCD(180.0),
    frozenOrbOnCD (false), iceNovaOnCD(false), mirrorImageOnCD(false), icyVeinsOnCD(false), openingRotationRun(true), normalRotationRun(true){

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
}

void Mage::setLastSpellCast(QString spell){
    lastSpellCast = spell;
}

QString Mage::getLastSpellCast(){
    return lastSpellCast;
}

void Mage::addTime(double time){
    if (time > 0){
        if(iceNovaOnCD){
            iceNovaTimer += time;
        }
        if(frozenOrbOnCD){
            frozenOrbTimer += time;
        }
        if(icyVeinsOnCD){
            icyVeinsTimer += time;
        }
        if(mirrorImageOnCD){
            mirrorImageTimer += time;
        }
        int timeInt = time*1000;
        globalTimer += time;
        Sleep(timeInt);
    }
}

void Mage::setNormalRotationRun(bool x){
    normalRotationRun = x;
}

void Mage::setOpeningRotationRun(bool x){
    openingRotationRun = x;
}

//======================================================================//
//============================ ROTATIONS ===============================//
//======================================================================//

void Mage::openingRotation(){
    openingRotationRun = true;
    while(openingRotationRun){
        castMirrorImage();
        castIcyVeins();
        castFrostBolt();
        castFrostBolt();
        castIceLance();
        castIceLance();
        castIceNova();
        castIceNova();
        castFrostfireBolt();
        castFrozenOrb();
        castFrostBolt();
        castIceLance();
        openingRotationRun = false;
    }
}

void Mage::normalRotation(){
    normalRotationRun = true;
    while(normalRotationRun){
        for(int i=1;i<101;i++){
            if(!icyVeinsOnCD){
                castIcyVeins();
            }
            if(!frozenOrbOnCD){
                castFrozenOrb();
                castFrostBolt();
                castIceLance();
            }
            if(!iceNovaOnCD){
                castIceNova();
            }
            if(!mirrorImageOnCD){
                castMirrorImage();
            }
            if(i%4 == 0){
                castFrostfireBolt();
            }
            if(i%5 == 0){
                castIceLance();
            }
            castFrostBolt();
        }
        normalRotationRun = false;
    }
}

//======================================================================//
//============================ ROTATIONS ===============================//
//======================================================================//


//======================================================================//
//=========================== MAGE SPELLS ==============================//
//======================================================================//

void Mage::castFrostBolt(){ //represented by the 3 key.
    // Press the "3" key
    ip.ki.wVk = 0x33; // virtual-key code for the "3" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "3" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(240);
    // Press the "3" key
    ip.ki.wVk = 0x33; // virtual-key code for the "3" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "3" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(240);
    // Press the "3" key
    ip.ki.wVk = 0x33; // virtual-key code for the "3" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "3" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("FrostBolt");
    waitGCD();
}

void Mage::castFrostfireBolt(){
    // Press the "1" key
    ip.ki.wVk = 0x31; // virtual-key code for the "1" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "1" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    ip.ki.wVk = 0x31; // virtual-key code for the "1" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "1" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    ip.ki.wVk = 0x31; // virtual-key code for the "1" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "1" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("FrostFireBolt");
    waitGCD();
}

void Mage::castIceLance(){
    // Press the "2" key
    ip.ki.wVk = 0x32; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "2" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "2" key
    ip.ki.wVk = 0x32; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "2" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "2" key
    ip.ki.wVk = 0x32; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "2" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("IceLance");
    waitGCD();
}
void Mage::right(){
    // Press the "right arrow" key
    ip.ki.wVk = 0x27; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Release the "right arrow" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void Mage::left(){
    // Press the "left arrow" key
    ip.ki.wVk = 0x25; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Release the "left arrow" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void Mage::tab(){
    // Press the "tab" key
    ip.ki.wVk = 0x09; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "tab" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);

    // Press the "tab" key
    ip.ki.wVk = 0x09; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "tab" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void Mage::castFrozenOrb(){
    frozenOrbOnCD = true;
    // Press the "0" key
    ip.ki.wVk = 0x30; // virtual-key code for the "0" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "0" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "0" key
    ip.ki.wVk = 0x30; // virtual-key code for the "0" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "0" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "0" key
    ip.ki.wVk = 0x30; // virtual-key code for the "0" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "0" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("FrozenOrb");
    waitGCD();
}

void Mage::castIceNova(){
    iceNovaOnCD = true;
    // Press the "4" key
    ip.ki.wVk = 0x34; // virtual-key code for the "4" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "4" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "4" key
    ip.ki.wVk = 0x34; // virtual-key code for the "4" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "4" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "4" key
    ip.ki.wVk = 0x34; // virtual-key code for the "4" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "4" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("IceNova");
    waitGCD();
}

void Mage::castIcyVeins(){
    icyVeinsOnCD = true;
    // Press the "numpad 6" key
    ip.ki.wVk = VK_NUMPAD6; // virtual-key code for the "numpad 6" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "numpad 6" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("icyVeins");
}

void Mage::castMirrorImage(){
    mirrorImageOnCD = true;
    // Press the "numpad 5" key
    ip.ki.wVk = VK_NUMPAD5; // virtual-key code for the "numpad 5" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "numpad 5" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(250);

    // Press the "numpad 5" key
    ip.ki.wVk = VK_NUMPAD5; // virtual-key code for the "numpad 5" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "numpad 5" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    setLastSpellCast("mirrorImage");
    waitGCD();
}

//======================================================================//
//=========================== MAGE SPELLS ==============================//
//======================================================================//

void Mage::waitGCD(){ //Global cooldown for all spells
    Sleep(1000); //1.0 seconds
    globalTimer += 15.0;
    if(iceNovaOnCD){
        iceNovaTimer += 1.5;
    }
    if(frozenOrbOnCD){
        frozenOrbTimer += 1.5;
    }
    if(icyVeinsOnCD){
        icyVeinsTimer += 1.5;
    }
    if(mirrorImageOnCD){
        mirrorImageTimer += 1.5;
    }
    if(frozenOrbTimer >= frozenOrbCD){
        frozenOrbOnCD = false;
    }
    if(iceNovaTimer >= iceNovaCD){
        iceNovaOnCD = false;
    }
    if(icyVeinsTimer >= icyVeinsCD){
        icyVeinsOnCD = false;
    }
    if(mirrorImageTimer >= mirrorImageCD){
        mirrorImageOnCD = false;
    }
}

void LeftClick ( )
{
  INPUT    Input={0};
  // left down
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
  ::SendInput(1,&Input,sizeof(INPUT));

  // left up
  ::ZeroMemory(&Input,sizeof(INPUT));
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
  ::SendInput(1,&Input,sizeof(INPUT));
}

void Mage::petguru(){
    Sleep(1000); //5.0 seconds
    // Press the "p" key
    ip.ki.wVk = 0x50; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "p" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "e" key
    ip.ki.wVk = 0x45; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "e" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "t" key
    ip.ki.wVk = 0x54; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "t" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "s" key
    ip.ki.wVk = 0x53; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "s" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "g" key
    ip.ki.wVk = 0x47; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "g" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "u" key
    ip.ki.wVk = 0x55; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "u" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "r" key
    ip.ki.wVk = 0x52; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "r" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "u" key
    ip.ki.wVk = 0x55; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "u" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "tab" key
    ip.ki.wVk = 0x09; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "tab" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "u" key
    ip.ki.wVk = 0x55; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "u" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "tab" key
    ip.ki.wVk = 0x09; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "tab" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    // Press the "ctrl" key
    ip.ki.wVk = 0x11; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(100);
    // Press the "v" key
    ip.ki.wVk = 0x56; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "v" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);

    // Press the "ctrl" key
    ip.ki.wVk = 0x11; // virtual-key code for the "left shift" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "ctrl" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(100);
    LeftClick();
}

void Mage::leftclick(){
    LeftClick();
}

