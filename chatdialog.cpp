#include "chatdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
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
    this->setMinimumSize(600, 350);
    this->resize(600, 350);

    // 聊天气泡区
    chatList = new QListWidget(this);
    chatList->setStyleSheet(
        "QListWidget {"
        "  background: #f8fbff;"
        "  border: none;"
        "  border-radius: 16px;"
        "  padding: 8px;"
        "}"
        "QListWidget::item { border: none; }"
    );
    chatList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    chatList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chatList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    chatList->setMaximumHeight(220); // 控制输出区高度

    // 输入框
    input = new QLineEdit(this);
    input->setStyleSheet(
        "QLineEdit {"
        "  background: white;"
        "  color: #1E90FF;"
        "  font-size: 15px;"
        "  border: 2px solid #1E90FF;"
        "  border-radius: 12px;"
        "  padding: 6px 12px;"
        "}"
    );

    // 发送按钮
    sendButton = new QPushButton("Send", this);
    sendButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #1E90FF;"
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 5px 15px;"
        "}"
        "QPushButton:hover { background-color: #63b3ed; }"
    );

    // 聊天输入区
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(input);
    inputLayout->addWidget(sendButton);

    // 聊天区（聊天记录+输入）
    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(chatList, 1);
    chatLayout->addLayout(inputLayout);

    // 动图label设置合适的大小策略
    label->setFixedSize(128, 128);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 主布局：左侧聊天区，右侧动图
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    mainLayout->addWidget(label, 0, Qt::AlignTop | Qt::AlignLeft);//左侧宠物
    mainLayout->addLayout(chatLayout, 1);//右侧聊天区

    setLayout(mainLayout);

    connect(sendButton, &QPushButton::clicked, this, &ChatDialog::onSend);

    networkManager = new QNetworkAccessManager(this);
}

// 气泡样式的HTML生成函数
static QString bubbleHtml(const QString& text, bool isUser)
{
    // 气泡样式
    QString bubbleStyle =
        "display:inline-block;"
        "padding:8px 14px;"
        "margin:6px 0;"
        "border-radius:16px;"
        "max-width:70%;"
        "font-size:15px;"
        "border:2px solid #1E90FF;"
        "background:white;"
        "color:#1E90FF;"
        "word-break:break-all;"
        "box-sizing:border-box;";
    // 用户消息靠右，宠物消息靠左
    QString align = isUser ? "right" : "left";
    QString margin = isUser ? "margin-left:30%;" : "margin-right:30%;";
    return QString("<div style='text-align:%1;'><span style='%2 %3'>%4</span></div>")
        .arg(align)
        .arg(bubbleStyle)
        .arg(margin)
        .arg(text.toHtmlEscaped());
}

void ChatDialog::addBubble(const QString &text, bool isUser)
{
    QWidget *bubbleWidget = new QWidget;
    QLabel *bubbleLabel = new QLabel(text);
    bubbleLabel->setWordWrap(true);
    bubbleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    bubbleLabel->setStyleSheet(
        QString("QLabel {"
                "background: white;"
                "color: #1E90FF;"
                "border: 2px solid #1E90FF;"
                "border-radius: 16px;"
                "padding: 8px 14px;"
                "font-size: 15px;"
                "max-width: 320px;"
                "}")
    );
    QHBoxLayout *layout = new QHBoxLayout(bubbleWidget);
    if (isUser) {
        layout->addStretch();
        layout->addWidget(bubbleLabel, 0, Qt::AlignRight);
    } else {
        layout->addWidget(bubbleLabel, 0, Qt::AlignLeft);
        layout->addStretch();
    }
    layout->setContentsMargins(0, 0, 0, 0);
    bubbleWidget->setLayout(layout);

    QListWidgetItem *item = new QListWidgetItem(chatList);
    item->setSizeHint(bubbleWidget->sizeHint());
    chatList->setItemWidget(item, bubbleWidget);
    chatList->scrollToBottom();
}

void ChatDialog::onSend() {
    QString userText = input->text();
    if (userText.isEmpty()) return;
    addBubble(userText, true); // 用户气泡
    callDeepseekAPI(userText);
    input->clear();
}

void ChatDialog::handleDeepseekReply(QNetworkReply *reply) {
    QString replyText;
    if (reply->error() != QNetworkReply::NoError) {
        replyText = "网络错误！";
    } else {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (!doc.isObject()) {
            replyText = "回复解析失败！";
        } else {
            QJsonObject obj = doc.object();
            if (obj.contains("choices")) {
                QJsonArray choices = obj["choices"].toArray();
                if (!choices.isEmpty()) {
                    QJsonObject choice = choices[0].toObject();
                    QJsonObject message = choice["message"].toObject();
                    replyText = message["content"].toString();
                }
            }
        }
    }
    if (replyText.isEmpty()) replyText = "没有收到有效回复。";
    addBubble(replyText, false); // 宠物气泡
    reply->deleteLater();
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
