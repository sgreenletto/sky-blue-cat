#include "petwidget.h"
#include "ui_petwidget.h"
#include "chatdialog.h"
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QInputDialog>

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
    movie->start();
    resize(movie->currentPixmap().size());

    label = new QLabel(this);
    label->setScaledContents(false); // 不拉伸
    movie = new QMovie(idleGif);
    label->setMovie(movie);
    movie->start();
    label->setFixedSize(movie->currentPixmap().size());
    resize(label->size());
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
    QAction *exitAction = menu.addAction("Exit");

    connect(eatAction, &QAction::triggered, this, &PetWidget::playEat);
    connect(sleepAction, &QAction::triggered, this, &PetWidget::playSleep);
    connect(chatAction, &QAction::triggered, this, &PetWidget::openChat);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    menu.exec(event->globalPos());

    QAction *resizeAction = menu.addAction("Resize");
    connect(resizeAction, &QAction::triggered, this, [this]() {
        QStringList sizes = {"Small", "Medium", "Large"};
        bool ok;
        QString size = QInputDialog::getItem(this, "Resize", "Select size:", sizes, 1, false, &ok);
        if (ok) {
            int w = 200, h = 200;
            if (size == "Small") w = h = 120;
            else if (size == "Large") w = h = 320;
            label->setFixedSize(w, h);
            resize(w, h);
            if (chatDialog) chatDialog->setChatGifSize(w/2, h/2);
        }
    });
}

void PetWidget::playEat() {
    if (chatDialog) {
        chatDialog->close();
        chatDialog = nullptr;
    }
    label->show(); // 确保主界面动图显示
    playAnimation(eatGif);
}

void PetWidget::playSleep() {
    if (chatDialog) {
        chatDialog->close();
        chatDialog = nullptr;
    }
    label->show();
    playAnimation(sleepGif);
}

void PetWidget::openChat() {
    if (chatDialog) {
        chatDialog->raise();
        return;
    }
    label->hide(); // 隐藏主界面动图
    chatDialog = new ChatDialog(this);
    chatDialog->setAttribute(Qt::WA_DeleteOnClose);
    QPoint pos = this->pos() + QPoint(this->width(), 0);
    chatDialog->move(pos);
    chatDialog->show();

    connect(chatDialog, &QDialog::finished, this, [this]() {
        label->show(); // 聊天框关闭时恢复主界面动图
        chatDialog = nullptr;
    });
}
void PetWidget::playAnimation(const QString &gifPath, bool restoreIdle) {
    movie->stop();
    delete movie;
    movie = new QMovie(gifPath);

    // eat/sleep 动图为主界面 3/4 大小
    QSize idleSize = label->size();
    QSize animSize(idleSize.width() * 3 / 4, idleSize.height() * 3 / 4);
    movie->setScaledSize(animSize);

    label->setMovie(movie);
    label->setFixedSize(animSize);

    if (restoreIdle) {
        loopCount = 0;
        disconnect(movie, nullptr, nullptr, nullptr);
        connect(movie, &QMovie::frameChanged, this, [this, idleSize](int frameNumber) {
            if (frameNumber == movie->frameCount() - 1) {
                loopCount++;
                if (loopCount >= 5) {
                    movie->stop();
                    delete movie;
                    movie = new QMovie(idleGif);
                    label->setMovie(movie);
                    label->setFixedSize(idleSize);
                    movie->start();
                }
            }
        });
    }
    movie->start();
}
