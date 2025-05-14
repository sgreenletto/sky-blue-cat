#include "views/dialogs/startdialog.h"
#include "ui_startdialog.h"
#include <QApplication>

StartDialog::StartDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 连接按钮信号
    connect(ui->startButton, &QPushButton::clicked, this, &StartDialog::on_startButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &StartDialog::on_exitButton_clicked);

    qDebug() << "StartDialog constructed";
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void StartDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void StartDialog::on_startButton_clicked()
{
    qDebug() << "Start button clicked";
    accept();
}

void StartDialog::on_exitButton_clicked()
{
    QApplication::quit();
}
