#include "petwidget.h"
#include "ui_petwidget.h"
#include "chatdialog.h"
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QApplication>


PetWidget::PetWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PetWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, true);

    idleGif = "C:\\Users\\23788\\Desktop\\c++大作业\\素材\\i-dle.gif";
    eatGif = "C:\\Users\\23788\\Desktop\\c++大作业\\素材\\eat.gif";
    sleepGif = "C:\\Users\\23788\\Desktop\\c++大作业\\素材\\sleep.gif";

    label = new QLabel(this);
    movie = new QMovie(idleGif);
    label->setMovie(movie);
    label->setScaledContents(true);

    movie->start();
    resize(movie->currentPixmap().size());

    const int petSize = 128; // 你可以根据需要调整
    setFixedSize(petSize, petSize);
    label->setFixedSize(petSize, petSize);
}

PetWidget::~PetWidget()
{
    delete ui;
}

void PetWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void PetWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void PetWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    QAction *eatAction = menu.addAction("Eat");
    QAction *sleepAction = menu.addAction("Sleep");
    QAction *chatAction = menu.addAction("Chat");
    menu.addSeparator();
    menu.addAction("exit", this, &QWidget::close); // 新增

    connect(eatAction, &QAction::triggered, this, &PetWidget::playEat);
    connect(sleepAction, &QAction::triggered, this, &PetWidget::playSleep);
    connect(chatAction, &QAction::triggered, this, &PetWidget::openChat);
    
    menu.exec(event->globalPos());
}

void PetWidget::exitApp() {
    qApp->quit();
}


void PetWidget::playAnimation(const QString &gifPath, bool restoreIdle) {
    movie->stop();
    delete movie;
    movie = new QMovie(gifPath);
    label->setMovie(movie);
    movie->start();

    // 动画播放一次后恢复idle
    if (restoreIdle) {
        connect(movie, &QMovie::finished, [this]() {
            movie->stop();
            delete movie;
            movie = new QMovie(idleGif);
            label->setMovie(movie);
            movie->start();
        });
    }
}


// PetWidget.cpp
void PetWidget::openChat() {
    if (!chatDialog) chatDialog = new ChatDialog(this);
    chatDialog->show();
}

void PetWidget::playEat() {
    if (chatDialog) chatDialog->close();
    // ...播放eat动画...
    eatCount++;
    playAnimation(eatGif, false);
    if (eatCount >= 5) {
        eatCount = 0;
        playAnimation(idleGif, false);
    }
}

void PetWidget::playSleep() {
    if (chatDialog) chatDialog->close();
    // ...播放sleep动画...
    sleepCount++;
    playAnimation(sleepGif, false);
    if (sleepCount >= 5) {
        sleepCount = 0;
        playAnimation(idleGif, false);
    }
}
