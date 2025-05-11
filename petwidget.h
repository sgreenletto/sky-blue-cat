#ifndef PETWIDGET_H
#define PETWIDGET_H

#pragma once
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QMenu>
#include <QPoint>
#include "chatdialog.h"


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
    void exitApp();

private:
    int eatCount=0;
    int sleepCount=0;
    Ui::PetWidget *ui;
    void playAnimation(const QString &gifPath, bool restoreIdle = true);
// PetWidget.h
    ChatDialog *chatDialog = nullptr;
    
    QLabel *label;
    QMovie *movie;
    QPoint dragPosition;
    QString idleGif;
    QString eatGif;
    QString sleepGif;
};
#endif

