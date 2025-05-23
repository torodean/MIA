#ifndef D3CProgramConstants_H
#define D3CProgramConstants_H

#include "QApplication"
#include "d3cwindow.h"

namespace std {

class D3CProgramConstants {
    private:
        QString programVersion;
    public:
        D3CProgramConstants();
        ~D3CProgramConstants();
        QString getD3CPromptStart();
        QString getProgramVersion();
};

} /* namespace std */

#endif // D3CProgramConstants_H
