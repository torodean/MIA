#include "d3cwindow.h"
#include <QApplication>
#include "D3CProgramConstants.h"



int main(int argc, char *argv[]){
    std::D3CProgramConstants program;
    QApplication a(argc, argv);
    D3CWindow window;
    window.show();
    window.setWindowTitle("D3C: D0sag3 Command version " + program.getProgramVersion());
    window.hideWindowItems();
    window.showHome();


    return a.exec();
}
