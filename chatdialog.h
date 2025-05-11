#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#endif // CHATDIALOG_H
#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QMovie>



class ChatDialog : public QDialog {
    Q_OBJECT
public:
    ChatDialog(QWidget *parent = nullptr);

private slots:
    void onSend();

private:
    QTextEdit *chatHistory;
    QLineEdit *input;
    QPushButton *sendButton;
    QLabel *petGifLabel;
    QMovie *petGifMovie;

    QNetworkAccessManager *networkManager;
    void callDeepseekAPI(const QString &userText);
    void handleDeepseekReply(QNetworkReply *reply);
};

