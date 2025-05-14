#ifndef __VIEWS_MAINWINDOW_H__
#define __VIEWS_MAINWINDOW_H__

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>
#include <QStyle>
#include <QScreen>

#include "cores/framework.h"
#include "utils/resources.h"

#include "views/dialogs/aboutdialog.h"
#include "views/dialogs/customdialog.h"
#include "views/dialogs/rankdialog.h"
#include "views/dialogs/successdialog.h"
#include "views/dialogs/startdialog.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        Ui::MainWindow* ui;

    private:
        GameFramework* gameFramework;
        GameResources* gameResources;

    private:
        QTimer* frameTimer;
        QTimer* clockTimer;

    private:
        AboutDialog* aboutDialog;
        CustomDialog* customDialog;
        RankDialog* rankDialog;
        SuccessDialog* successDialog;
        StartDialog* startDialog;

    private:
        int fitnessW;
        int fitnessH;
        int elapseTime;
        QPoint m_dragPosition;

    private:
        void setLevel1();
        void setLevel2();
        void setLevel3();
        void setCustomLevel();

    private:
        void setupTimers();
        void setupActions();

    private:
        void resizeWindow();
        void restartGame();
        void judgeSuccess();
        void updateInfo();

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        bool eventFilter(QObject* watched, QEvent* event) override;

    private:
        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

    public:
        MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    public:
        void initialize();
};
#endif
