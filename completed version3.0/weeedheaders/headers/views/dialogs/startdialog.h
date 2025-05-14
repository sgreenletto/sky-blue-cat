#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StartDialog(QWidget *parent = nullptr);
    ~StartDialog();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_startButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::StartDialog *ui;
    QPoint m_dragPosition;
};

#endif // STARTDIALOG_H
