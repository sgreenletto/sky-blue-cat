#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "petgifwindow.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ChatDialog : public PetGifWindow {
    Q_OBJECT
public:
    ChatDialog(PetState state, QWidget *parent = nullptr);

private slots:
    void onSend();

private:
    QTextEdit *chatHistory;
    QLineEdit *input;
    QPushButton *sendButton;
    QNetworkAccessManager *networkManager;
    void callDeepseekAPI(const QString &userText);
    void handleDeepseekReply(QNetworkReply *reply);
};

#endif // CHATDIALOG_H

