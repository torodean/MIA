#include "dpssim.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DPSSim w;
    w.show();


    return a.exec();
}
