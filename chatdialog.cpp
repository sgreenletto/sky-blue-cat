#include "chatdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

ChatDialog::ChatDialog(PetState state, QWidget *parent)
    : PetGifWindow(state, parent)
{
    // 让窗口更大
    this->setMinimumSize(300, 120);
    this->resize(300, 120);

    // 聊天控件
    chatHistory = new QTextEdit(this);
    chatHistory->setReadOnly(true);
    chatHistory->setMinimumHeight(180);

    input = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    // 聊天输入区
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(sendButton);

    // 聊天区（聊天记录+输入）
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(chatHistory, 1);
    chatLayout->addLayout(inputLayout);

    // 动图label设置合适的大小策略
    label->setFixedSize(128, 128);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 主布局：左侧动图，右侧聊天区
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addLayout(chatLayout, 1); // 左侧聊天区
    mainLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignRight); // 右侧动图

    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatDialog::onSend);

    networkManager = new QNetworkAccessManager(this);
}

void ChatDialog::onSend() {
    QString userText = input->text();
    if (userText.isEmpty()) return;
    chatHistory->append("You: " + userText);
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


