#include <QApplication>
#include "views/dialogs/startdialog.h"
#include "views/mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    // 先创建并显示开始对话框
    StartDialog startDialog;
    startDialog.setModal(true);
    
    if (startDialog.exec() == QDialog::Accepted) {
        // 如果用户点击了开始按钮，才创建并显示主窗口
        MainWindow window;
        window.initialize();
        window.show();
        return app.exec();
    }
    
    // 如果用户点击了退出按钮，直接退出程序
    return 0;
}
