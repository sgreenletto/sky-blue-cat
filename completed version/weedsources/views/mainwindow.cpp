#include "views/mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    gameFramework = new GameFramework();
    gameResources = new GameResources();

    frameTimer = new QTimer(this);
    clockTimer = new QTimer(this);

    aboutDialog = new AboutDialog(this);
    customDialog = new CustomDialog(this);
    rankDialog = new RankDialog(this);
    successDialog = new SuccessDialog(this);
    startDialog = new StartDialog(this);

    ui->setupUi(this);
    ui->sceneWidget->setGameFramework(gameFramework);
    ui->sceneWidget->setGameResources(gameResources);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    qDebug() << "MainWindow constructed";
    qDebug() << "MainWindow constructed";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameFramework;
    delete gameResources;

    delete frameTimer;
    delete clockTimer;

    delete aboutDialog;
    delete customDialog;
    delete rankDialog;
    delete successDialog;
    delete startDialog;
}

void MainWindow::initialize()
{
    setupTimers();
    setupActions();
    setLevel1();

    frameTimer->setInterval(TimerInterval::FRAME);
    frameTimer->start();

    clockTimer->setInterval(TimerInterval::CLOCK);
    clockTimer->start();
}

void MainWindow::setLevel1()
{
    gameFramework->setLevel1();
    resizeWindow();
    restartGame();
}

void MainWindow::setLevel2()
{
    gameFramework->setLevel2();
    resizeWindow();
    restartGame();
}

void MainWindow::setLevel3()
{
    gameFramework->setLevel3();
    resizeWindow();
    restartGame();
}

void MainWindow::setCustomLevel()
{
    if (customDialog->exec() == 0)
    {
        int rows = customDialog->getInputRows();
        int cols = customDialog->getInputCols();

        gameFramework->setCustomLevel(rows, cols, customDialog->getInputMineCount());
        resizeWindow();
        restartGame();
    }
}

void MainWindow::resizeWindow()
{
    int rows = gameFramework->getRows();
    int cols = gameFramework->getCols();

    fitnessW = cols * UnitProperties::SIZE + WindowProperties::MARGIN_X * 2;
    fitnessH = rows * UnitProperties::SIZE + WindowProperties::MARGIN_Y + WindowProperties::MARGIN_X;

    setFixedSize(fitnessW, fitnessH);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), QGuiApplication::primaryScreen()->geometry()));
}

void MainWindow::setupTimers()
{
    connect(frameTimer, &QTimer::timeout, this, [=]
    {
        updateInfo();

        if (gameFramework->getStatus() == GameStatus::PLAYING)
        {
            judgeSuccess();

            if (isMinimized())
            {
                gameFramework->pause();
            }
            gameFramework->uncoverEmptyUnits();
        }
        ui->sceneWidget->update();
    });

    connect(clockTimer, &QTimer::timeout, this, [=]
    {
        if (gameFramework->getStatus() == GameStatus::PLAYING)
        {
            elapseTime += 1;
        }
    });
}

void MainWindow::setupActions()
{
    connect(ui->actionLevel1, &QAction::triggered, this, [=]
    {
        setLevel1();
    });

    connect(ui->actionLevel2, &QAction::triggered, this, [=]
    {
        setLevel2();
    });

    connect(ui->actionLevel3, &QAction::triggered, this, [=]
    {
        setLevel3();
    });

    connect(ui->actionCustom, &QAction::triggered, this, [=]
    {
        setCustomLevel();
    });

    connect(ui->actionPause, &QAction::triggered, this, [=]
    {
        gameFramework->pause();
    });

    connect(ui->actionRestart, &QAction::triggered, this, [=]
    {
        restartGame();
    });

    connect(ui->actionRecord, &QAction::triggered, this, [=]
    {
        rankDialog->exec();
    });

    connect(ui->actionAbout, &QAction::triggered, this, [=]
    {
        aboutDialog->exec();
    });

    // 设置退出标签可点击
    ui->exitButton->setMouseTracking(true);
    ui->exitButton->installEventFilter(this);
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->exitButton)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QApplication::quit();
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updateInfo()
{
    ui->timeValueLabel->setNum(elapseTime);

    if (gameFramework->getStatus() == GameStatus::PAUSE)
    {
        ui->pauseInfoLabel->show();
    }
    else
    {
        ui->pauseInfoLabel->hide();
    }
    ui->remainValueLabel->setNum(gameFramework->getRemainFlagCount());
}

void MainWindow::restartGame()
{
    elapseTime = 0;

    gameResources->getClickSound()->stop();
    gameResources->getClickSound()->play();

    gameFramework->restart();
    gameFramework->addMines();
    gameFramework->addNumbers();
}

void MainWindow::judgeSuccess()
{
    if (!gameFramework->isSuccess())
    {
        return;
    }
    gameResources->getSuccessSound()->stop();
    gameResources->getSuccessSound()->play();

    if (gameFramework->getLevel() == GameLevel::CUSTOM || gameFramework->isCracked())
    {
        return;
    }
    if (successDialog->exec() == 0)
    {
        rankDialog->addRecord(successDialog->getInputName(), QString::number(elapseTime), gameFramework->getLevel());
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        // 检查是否点击在游戏区域外（用于窗口拖动）
        QPoint mouse = event->pos();
        if (mouse.x() < WindowProperties::MARGIN_X || mouse.x() > fitnessW - WindowProperties::MARGIN_X ||
            mouse.y() < WindowProperties::MARGIN_Y || mouse.y() > fitnessH - WindowProperties::MARGIN_X) {
            m_dragPosition = event->globalPos() - frameGeometry().topLeft();
            event->accept();
            return;
        }

        // 游戏区域内的点击处理
        if (!gameFramework->isCracking() && gameFramework->getStatus() == GameStatus::PLAYING) {
            int col = (mouse.x() - WindowProperties::MARGIN_X) / UnitProperties::SIZE;
            int row = (mouse.y() - WindowProperties::MARGIN_Y) / UnitProperties::SIZE;

            if (gameFramework->leftButtonClick(row, col)) {
                if (gameFramework->isFailure(row, col)) {
                    gameResources->getFailureSound()->stop();
                    gameResources->getFailureSound()->play();
                } else {
                    gameResources->getClickSound()->stop();
                    gameResources->getClickSound()->play();
                }
            }
        } else if (gameFramework->getStatus() == GameStatus::PAUSE) {
            gameResources->getClickSound()->stop();
            gameResources->getClickSound()->play();
            gameFramework->resume();
        } else {
            restartGame();
        }
    } else if (event->button() == Qt::RightButton && !gameFramework->isCracking() && 
               gameFramework->getStatus() == GameStatus::PLAYING) {
        QPoint mouse = event->pos();
        if (mouse.x() >= WindowProperties::MARGIN_X && mouse.x() <= fitnessW - WindowProperties::MARGIN_X &&
            mouse.y() >= WindowProperties::MARGIN_Y && mouse.y() <= fitnessH - WindowProperties::MARGIN_X) {
            int col = (mouse.x() - WindowProperties::MARGIN_X) / UnitProperties::SIZE;
            int row = (mouse.y() - WindowProperties::MARGIN_Y) / UnitProperties::SIZE;

            if (gameFramework->rightButtonClick(row, col)) {
                gameResources->getClickSound()->stop();
                gameResources->getClickSound()->play();
            }
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    // 只有在按下左键且点击在窗口边缘区域时才允许拖动
    if ((event->buttons() & Qt::LeftButton) && 
        (event->pos().x() < WindowProperties::MARGIN_X || 
         event->pos().x() > fitnessW - WindowProperties::MARGIN_X ||
         event->pos().y() < WindowProperties::MARGIN_Y || 
         event->pos().y() > fitnessH - WindowProperties::MARGIN_X)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_P)
    {
        gameFramework->pause();
    }
    else if (event->key() == Qt::Key_R)
    {
        restartGame();
    }
    else if (event->key() == Qt::Key_Z && gameFramework->getStatus() == GameStatus::PLAYING)
    {
        gameFramework->setCrackStart();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Z && gameFramework->getStatus() == GameStatus::PLAYING)
    {
        gameFramework->setCrackEnd();
    }
}
