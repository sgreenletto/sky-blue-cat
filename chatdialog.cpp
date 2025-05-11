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
    const int petSize = 128; // 与PetWidget保持一致
    setFixedSize(petSize, petSize);
    petGifLabel->setFixedSize(petSize, petSize);

    // 透明背景
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(0.85);
    setGraphicsEffect(opacity);

    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);
    input = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(sendButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chatHistory);
    mainLayout->addLayout(inputLayout);

    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatDialog::onSend);

     networkManager = new QNetworkAccessManager(this);

    petGifLabel = new QLabel(this);
    petGifMovie = new QMovie("C:\\Users\\23788\\Desktop\\++大作业\\素材\\读信抠图.gif"); // 你的动图路径
    petGifLabel->setMovie(petGifMovie);
    petGifMovie->start();
    petGifLabel->setFixedSize(64, 64); // 根据你的动图大小调整

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(petGifLabel);
    topLayout->addWidget(chatHistory);

    QVBoxLayout *mainLayout2 = new QVBoxLayout(this);
    mainLayout2->addLayout(topLayout);
    mainLayout2->addLayout(inputLayout);
    setLayout(mainLayout2);
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


