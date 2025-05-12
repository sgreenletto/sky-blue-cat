#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "petgifwindow.h"
#include <QTextEdit>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>


class ChatDialog : public PetGifWindow {
    Q_OBJECT
public:
    ChatDialog(PetState state, QWidget *parent = nullptr);

private slots:
    void onSend();

private:
    QListWidget *chatList; // 用于显示气泡
    QLineEdit *input;
    QPushButton *sendButton;
    QNetworkAccessManager *networkManager;
    void callDeepseekAPI(const QString &userText);
    void handleDeepseekReply(QNetworkReply *reply);
    void addBubble(const QString &text, bool isUser);
};

#endif // CHATDIALOG_H

