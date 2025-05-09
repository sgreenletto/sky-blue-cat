#include "chatdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsOpacityEffect>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(300, 200);

    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);
    input = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(sendButton);

    // 聊天动图
    petGifLabel = new QLabel(this);
    petGifMovie = new QMovie("C:\\Users\\23788\\Desktop\\++大作业\\素材\\读信抠图.gif");
    petGifLabel->setMovie(petGifMovie);
    petGifMovie->start();
    petGifLabel->setScaledContents(false); // 不拉伸
    QSize chatGifSize(80, 80); // 你可以根据需要调整
    petGifLabel->setFixedSize(chatGifSize);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(petGifLabel);
    topLayout->addWidget(chatHistory);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(inputLayout);
    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatDialog::onSend);

    networkManager = new QNetworkAccessManager(this);
}

void ChatDialog::onSend() {
    QString userText = input->text();
    if (userText.isEmpty()) return;
    chatHistory->append("You: " + userText);

    // 这里接入deepseek API，获取回复
    // QString reply = callDeepseekAPI(userText);
    // chatHistory->append("Pet: " + reply);
    callDeepseekAPI(userText);
    input->clear();
}

void ChatDialog::callDeepseekAPI(const QString &userText) {
    QUrl url("https://api.deepseek.com"); // 替换为你的 DeepSeek API 地址
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer sk-a0a2447838cf49a8bc68f1ded9114a24"); // 替换为你的 API KEY

    // 构造 JSON 请求体
    QJsonObject message;
    message["role"] = "user";
    message["content"] = userText;

    QJsonArray messages;
    messages.append(message);

    QJsonObject body;
    body["model"] = "小八"; // 替换为你的模型名
    body["messages"] = messages;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, [this, reply]() { handleDeepseekReply(reply); });
}

void ChatDialog::handleDeepseekReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        chatHistory->append("Pet: 网络错误！");
        reply->deleteLater();
        return;
    }
    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (!doc.isObject()) {
        chatHistory->append("Pet: 回复解析失败！");
        reply->deleteLater();
        return;
    }
    QJsonObject obj = doc.object();
    // 解析 DeepSeek 返回的内容（根据实际返回结构调整）
    QString replyText;
    if (obj.contains("choices")) {
        QJsonArray choices = obj["choices"].toArray();
        if (!choices.isEmpty()) {
            QJsonObject choice = choices[0].toObject();
            QJsonObject message = choice["message"].toObject();
            replyText = message["content"].toString();
        }
    }
    if (replyText.isEmpty()) replyText = "Pet: 没有收到有效回复。";
    chatHistory->append("Pet: " + replyText);
    reply->deleteLater();
}


void ChatDialog::setChatGifSize(int w, int h) {
    if (petGifLabel) {
        petGifLabel->setFixedSize(w, h);
    }
}

