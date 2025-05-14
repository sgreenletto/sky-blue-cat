#include "petgifwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PetGifWindow w(Idle, nullptr); // Idle 是初始动图
    w.show();
    return a.exec();
}
