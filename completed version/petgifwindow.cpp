#include "petgifwindow.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QApplication>
#include "chatdialog.h"
#include <QApplication>
#include <QWidgetList>
#include <QTimer>
#include "chatdialog.h"
#include "weeedheaders/headers/views/dialogs/startdialog.h"
#include "weeedheaders/headers/views/mainwindow.h"

PetGifWindow* PetGifWindow::mainInstance = nullptr;

static QString gifPathForState(PetState state) {
    switch (state) {
    case Idle:  return ":/gifs/i-dle.gif";
    case Sleep: return ":/gifs/sleep.gif";
    case Eat:   return ":/gifs/eat.gif";
    case Chat:  return ":/gifs/read.gif";
    }
    return ":/gifs/i-dle.gif";
}

PetGifWindow::PetGifWindow(PetState state, QWidget *parent)
    : QWidget(parent), currentState(state)
{
    if (!mainInstance && state == Idle) mainInstance = this;
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAutoFillBackground(false);

    label = new QLabel(this);
    movie = nullptr;
    setGif(state);

    if (state != Chat) {
        const int petSize = 128;
        setFixedSize(petSize, petSize);
        label->setFixedSize(petSize, petSize);
        move(300, 300); // 显示在屏幕中央偏左上
    }

} 
void PetGifWindow::setGif(PetState state) {
    if (movie) {
        movie->stop();
        delete movie;
    }
    movie = new QMovie(gifPathForState(state));
    label->setMovie(movie);
    label->setScaledContents(true);
    movie->start();

    if (state == Idle) {
        label->setFixedSize(128, 128);
        setFixedSize(128, 128);
    } else if (state == Sleep) {
        label->setFixedSize(140, 100); // 例如睡觉时更扁
        setFixedSize(140, 100);
    } else if (state == Eat) {
        label->setFixedSize(100, 100); // 例如吃饭时更小
        setFixedSize(100, 100);
    } else if (state == Chat) {
        // Chat 状态由 ChatDialog 控制
    }

    qDebug() << "gif valid:" << movie->isValid();
}

// ... 省略前面代码 ...

void PetGifWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);

    if (currentState != Idle) {
        menu.addAction("Back", this, &PetGifWindow::showIdle);
    }
    if (currentState != Sleep) {
        menu.addAction("Sleep", this, &PetGifWindow::showSleep);
    }
    if (currentState != Eat) {
        menu.addAction("Eat", this, &PetGifWindow::showEat);
    }
    if (currentState != Chat) {
        menu.addAction("Chat", this, &PetGifWindow::showChat);
    }

    menu.addAction("Weed", this, [this]() {
        StartDialog *startDialog = new StartDialog(nullptr);
        startDialog->setModal(true);
        int result = startDialog->exec();
        qDebug() << "StartDialog exec result:" << result;
        if (result == QDialog::Accepted) {
            qDebug() << "StartDialog accepted from context menu";
            MainWindow *weedWindow = new MainWindow();
            weedWindow->initialize();
            weedWindow->show();
            qDebug() << "MainWindow shown from context menu";
        }
        startDialog->deleteLater();
    });

    menu.addSeparator();
    menu.addAction("Exit", this, &PetGifWindow::exitApp);

    menu.exec(event->globalPos());
}

// 下面是 showIdle、showSleep 等其他成员函数

void PetGifWindow::showIdle() {
    if (this != mainInstance && mainInstance) { mainInstance->showIdle(); return; }
    // 关闭并销毁当前chatDialog
    if (chatDialog) {
        chatDialog->close();
        chatDialog->deleteLater();
        chatDialog = nullptr;
    }
    // 关闭所有顶层 ChatDialog（保险）
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    currentState = Idle;
    setGif(Idle);
    this->show();
    this->raise();
    this->activateWindow();
    this->update();
}

void PetGifWindow::showSleep() {
    if (this != mainInstance && mainInstance) { mainInstance->showSleep(); return; }
    if (chatDialog) {
        chatDialog->close();
        chatDialog->deleteLater();
        chatDialog = nullptr;
    }
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    currentState = Sleep;
    setGif(Sleep);
    this->show();
    this->raise();
    this->activateWindow();
    this->update();
}

void PetGifWindow::showEat() {
    if (this != mainInstance && mainInstance) { mainInstance->showEat(); return; }
    if (chatDialog) {
        chatDialog->close();
        chatDialog->deleteLater();
        chatDialog = nullptr;
    }
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    currentState = Eat;
    setGif(Eat);
    this->show();
    this->raise();
    this->activateWindow();
    this->update();
}
void PetGifWindow::showChat() {
    if (this != mainInstance && mainInstance) { mainInstance->showChat(); return; }
    // 如果已有chatDialog且未关闭，先销毁
    if (chatDialog) {
        chatDialog->close();
        chatDialog->deleteLater();
        chatDialog = nullptr;
    }
    // 隐藏当前窗口
    this->hide();
    // 新建聊天窗口
    chatDialog = new ChatDialog(Chat, nullptr);
    // 监听 ChatDialog 关闭事件，延迟主窗口 show 并置空指针
    connect(chatDialog, &ChatDialog::destroyed, this, [this]() {
        QTimer::singleShot(0, this, [this]() {
            this->show();
            this->raise();
            this->activateWindow();
            this->update();
            chatDialog = nullptr;
        });
    });
    currentState = Chat;
    chatDialog->show();
}
void PetGifWindow::exitApp() {
    qApp->quit();
}

void PetGifWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void PetGifWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
