#ifndef PETGIFWINDOW_H
#define PETGIFWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QMenu>
#include <QPoint>

enum PetState {
    Idle,
    Sleep,
    Eat,
    Chat
};

class PetGifWindow : public QWidget {
    Q_OBJECT
public:
    explicit PetGifWindow(PetState state, QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void showIdle();
    void showSleep();
    void showEat();
    void showChat();
    void exitApp();

private:
    void setGif(PetState state);

protected:
    QLabel *label;
    QMovie *movie;
    PetState currentState;
    QPoint dragPosition;

};
#endif // PETGIFWINDOW_H
