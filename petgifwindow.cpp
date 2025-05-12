#include "petgifwindow.h"
#include <QContextMenuEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QApplication>
#include "chatdialog.h"

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
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);

    label = new QLabel(this);
    movie = nullptr;
    setGif(state);

    if (state != Chat) {
        const int petSize = 128;
        setFixedSize(petSize, petSize);
        label->setFixedSize(petSize, petSize);
        move(200, 200); // 显示在屏幕中央偏左上
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
    currentState = state;

    qDebug() << "gif valid:" << movie->isValid();
}

void PetGifWindow::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);

    // back 按钮（初始界面没有，其他界面有）
    if (currentState != Idle) {
        menu.addAction("Back", this, &PetGifWindow::showIdle);
    }
    // sleep 按钮（sleep界面没有）
    if (currentState != Sleep) {
        menu.addAction("Sleep", this, &PetGifWindow::showSleep);
    }
    // eat 按钮（eat界面没有）
    if (currentState != Eat) {
        menu.addAction("Eat", this, &PetGifWindow::showEat);
    }
    // chat 按钮（chat界面没有）
    if (currentState != Chat) {
        menu.addAction("Chat", this, &PetGifWindow::showChat);
    }
    menu.addSeparator();
    menu.addAction("Exit", this, &PetGifWindow::exitApp);

    menu.exec(event->globalPos());
}

void PetGifWindow::showIdle() {
    // 关闭所有顶层 ChatDialog
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    setGif(Idle);
    this->show();
}

void PetGifWindow::showSleep() {
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    setGif(Sleep);
    this->show();
}

void PetGifWindow::showEat() {
    const auto topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget *w : topLevelWidgets) {
        ChatDialog *chat = qobject_cast<ChatDialog*>(w);
        if (chat) chat->close();
    }
    setGif(Eat);
    this->show();
}
void PetGifWindow::showChat() {
    // 隐藏当前窗口
    this->hide();
    // 弹出聊天窗口，parent 设为 this
    ChatDialog *w = new ChatDialog(Chat, this);
    // 监听 ChatDialog 关闭事件，自动恢复主窗口
    connect(w, &ChatDialog::destroyed, this, [this]() { this->show(); });
    w->show();
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
