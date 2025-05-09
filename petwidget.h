#ifndef PETWIDGET_H
#define PETWIDGET_H

#pragma once
#include <QWidget>
#include "chatdialog.h"
#include <QLabel>
#include <QMovie>
#include <QMenu>
#include <QPoint>

#include "ui_petwidget.h"
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QTimer>
#include <QGraphicsOpacityEffect> // 新增
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class PetWidget;
}
QT_END_NAMESPACE

class PetWidget : public QWidget {
    Q_OBJECT
public:
    PetWidget(QWidget *parent = nullptr);
    ~PetWidget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void playEat();
    void playSleep();
    void openChat();

private:
    Ui::PetWidget *ui;
    void playAnimation(const QString &gifPath, bool restoreIdle = true);

    QLabel *label;
    QMovie *movie;


    QPoint dragPosition;
    QString idleGif;
    QString eatGif;
    QString sleepGif;

    int loopCount = 0;
    ChatDialog *chatDialog = nullptr;
};
#endif

