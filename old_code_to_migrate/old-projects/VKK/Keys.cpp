#define WINVER 0x0500
#include <windows.h>
#include "Keys.h"
#include <QString>
#include <iostream>

using namespace std;

INPUT ip;

Keys::Keys(){

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
}

void Keys::space(){
    // Press the "space" key
    ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "space" key
    ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::one(){
    // Press the "1" key
    ip.ki.wVk = 0x31; // virtual-key code for the "1" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "1" key
    ip.ki.wVk = 0x31; // virtual-key code for the "1" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::two(){
    // Press the "2" key
    ip.ki.wVk = 0x32; // virtual-key code for the "2" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "2" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::three(){
    // Press the "3" key
    ip.ki.wVk = 0x33; // virtual-key code for the "3" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "3" key
    ip.ki.wVk = 0x33; // virtual-key code for the "3" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::four(){
    // Press the "4" key
    ip.ki.wVk = 0x34; // virtual-key code for the "4" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "4" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::five(){
    // Press the "5" key
    ip.ki.wVk = 0x35; // virtual-key code for the "5" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "5" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::six(){
    // Press the "6" key
    ip.ki.wVk = 0x36; // virtual-key code for the "6" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "6" key
    ip.ki.wVk = 0x36; // virtual-key code for the "6" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::seven(){
    // Press the "7" key
    ip.ki.wVk = 0x37; // virtual-key code for the "7" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "7" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::eight(){
    // Press the "8" key
    ip.ki.wVk = 0x38; // virtual-key code for the "8" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "8" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::nine(){
    // Press the "9" key
    ip.ki.wVk = 0x39; // virtual-key code for the "9" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "9" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::zero(){
    // Press the "0" key
    ip.ki.wVk = 0x30; // virtual-key code for the "0" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "0" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::dash(){
    // Press the "-" key
    ip.ki.wVk = VK_OEM_MINUS; // virtual-key code for the "-" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "-" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::equal(){
    // Press the "=" key
    ip.ki.wVk = VK_OEM_PLUS; // virtual-key code for the "=" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "=" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::tab(){
    // Press the "tab" key
    ip.ki.wVk = VK_TAB; // virtual-key code for the "tab" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "tab" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::a(){
    // Press the "a" key
    ip.ki.wVk = 0x41; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "a" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::b(){
    // Press the "b" key
    ip.ki.wVk = 0x42; // virtual-key code for the "b" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "b" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::c(){
    // Press the "c" key
    ip.ki.wVk = 0x43; // virtual-key code for the "c" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "c" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::d(){
    // Press the "d" key
    ip.ki.wVk = 0x44; // virtual-key code for the "d" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "d" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::e(){
    // Press the "e" key
    ip.ki.wVk = 0x45; // virtual-key code for the "e" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "e" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::f(){
    // Press the "f" key
    ip.ki.wVk = 0x46; // virtual-key code for the "f" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "f" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::g(){
    // Press the "g" key
    ip.ki.wVk = 0x47; // virtual-key code for the "g" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "g" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::h(){
    // Press the "h" key
    ip.ki.wVk = 0x48; // virtual-key code for the "h" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "h" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::i(){
    // Press the "i" key
    ip.ki.wVk = 0x49; // virtual-key code for the "i" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "i" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::j(){
    // Press the "j" key
    ip.ki.wVk = 0x4A; // virtual-key code for the "j" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "j" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::k(){
    // Press the "k" key
    ip.ki.wVk = 0x4B; // virtual-key code for the "k" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "k" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::l(){
    // Press the "l" key
    ip.ki.wVk = 0x4C; // virtual-key code for the "l" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "l" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::m(){
    // Press the "m" key
    ip.ki.wVk = 0x4D; // virtual-key code for the "m" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "m" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::n(){
    // Press the "n" key
    ip.ki.wVk = 0x4E; // virtual-key code for the "n" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "n" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::o(){
    // Press the "o" key
    ip.ki.wVk = 0x4F; // virtual-key code for the "o" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "o" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::p(){
    // Press the "p" key
    ip.ki.wVk = 0x50; // virtual-key code for the "p" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "p" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::q(){
    // Press the "q" key
    ip.ki.wVk = 0x51; // virtual-key code for the "q" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "q" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::r(){
    // Press the "r" key
    ip.ki.wVk = 0x52; // virtual-key code for the "r" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "r" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::s(){
    // Press the "s" key
    ip.ki.wVk = 0x53; // virtual-key code for the "s" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "s" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::t(){
    // Press the "t" key
    ip.ki.wVk = 0x54; // virtual-key code for the "t" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "t" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::u(){
    // Press the "u" key
    ip.ki.wVk = 0x55; // virtual-key code for the "u" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "u" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::v(){
    // Press the "v" key
    ip.ki.wVk = 0x56; // virtual-key code for the "v" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "v" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::w(){
    // Press the "w" key
    ip.ki.wVk = 0x57; // virtual-key code for the "w" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "w" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::x(){
    // Press the "x" key
    ip.ki.wVk = 0x58; // virtual-key code for the "x" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "x" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::y(){
    // Press the "y" key
    ip.ki.wVk = 0x59; // virtual-key code for the "y" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "y" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::z(){
    // Press the "z" key
    ip.ki.wVk = 0x5A; // virtual-key code for the "z" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "z" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::alt0248(){
    // Press the "alt" key
    ip.ki.wVk = VK_LMENU; // virtual-key code for the "alt" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    zero();
    two();
    four();
    eight();

    // Release the "alt" key
    ip.ki.wVk = VK_MENU; // virtual-key code for the "alt" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::alt136(){
    // Press the "alt" key
    ip.ki.wVk = VK_MENU; // virtual-key code for the "alt" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    one();
    three();
    six();


    // Release the "alt" key
    ip.ki.wVk = VK_MENU;
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);
}

void Keys::press(QString character){
    if(character == "1"){
        one();
    } else if(character == "2"){
        two();
    } else if(character == "3"){
        three();
    } else if(character == "4"){
        four();
    } else if(character == "5"){
        five();
    } else if(character == "6"){
        six();
    } else if(character == "7"){
        seven();
    } else if(character == "8"){
        eight();
    } else if(character == "9"){
        nine();
    } else if(character == "0"){
        zero();
    } else if(character == "a"){
        a();
    } else if(character == "b"){
        b();
    } else if(character == "c"){
        c();
    } else if(character == "d"){
        d();
    } else if(character == "e"){
        e();
    } else if(character == "f"){
        f();
    } else if(character == "g"){
        g();
    } else if(character == "h"){
        h();
    } else if(character == "i"){
        i();
    } else if(character == "j"){
        j();
    } else if(character == "k"){
        k();
    } else if(character == "l"){
        l();
    } else if(character == "m"){
        m();
    } else if(character == "n"){
        n();
    } else if(character == "o"){
        o();
    } else if(character == "p"){
        p();
    } else if(character == "q"){
        q();
    } else if(character == "r"){
        r();
    } else if(character == "s"){
        s();
    } else if(character == "t"){
        t();
    } else if(character == "u"){
        u();
    } else if(character == "v"){
        v();
    } else if(character == "w"){
        w();
    } else if(character == "x"){
        x();
    } else if(character == "y"){
        y();
    } else if(character == "z"){
        z();
    } else if(character == " "){
        space();
    }else if(character == "-"){
        dash();
    }else if(character == "LC"){
        leftclick();
    }else if(character == "/"){
        slash();
    }else if(character == "."){
        slash();
    }
}

void Keys::type(QString word){
    int size = word.size();
    for(int i=0;i<size;i++){
        QString chara = word.at(i);
        press(chara);
        Sleep(25);
    }
}

void Keys::paste(){
    // Press the "ctrl" key
    ip.ki.wVk = VK_CONTROL; // virtual-key code for the "ctrl" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    v();

    // Release the "ctrl" key
    ip.ki.wVk = VK_CONTROL; // virtual-key code for the "ctrl" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(50);

}

void Keys::leftclick(){
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
  Sleep(100);
}

void Keys::slash(){
    // Press the "/" key
    ip.ki.wVk = 0x6F; // virtual-key code for the "/" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "/" key
    ip.ki.wVk = 0x6F; // virtual-key code for the "/" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::period(){
    // Press the "." key
    ip.ki.wVk = 0xBE; // virtual-key code for the "." key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "." key
    ip.ki.wVk = 0xBE; // virtual-key code for the "." key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}

void Keys::enter(){
    // Press the "enter" key
    ip.ki.wVk = 0x0D; // virtual-key code for the "enter" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "enter" key
    ip.ki.wVk = 0x0D; // virtual-key code for the "enter" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    Sleep(50);
}
