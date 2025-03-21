#include "chatclient.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

ChatClient::ChatClient(QObject *parent)
    : QObject{parent}
{
    m_clientSocket = new QTcpSocket(this);

    connect(m_clientSocket, &QTcpSocket::connected, this, &ChatClient::connected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &ChatClient::onReadRead);
}

//这里解析json并发送信号到主函数窗口
void ChatClient::showMsg(QByteArray jsonData)
{
    //调试新消息
    // emit messageReceived(QString::fromUtf8(jsonData));
    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(jsonDoc.isObject()){
            QJsonObject jsonObj = jsonDoc.object();
            QString type = jsonObj.value("type").toString();
            //如果是消息类型
            if(type.compare("message")==0){
                QString sender = jsonObj.value("sender").toString();
                QString text = jsonObj.value("text").toString();
                QString msg = sender + ": " + text;
                emit messageReceived(msg);
            }else if(type.compare("newuser")==0){
                QString username = jsonObj.value("username").toString();
                emit messageReceived("新用户登录：" + username);
                emit addNewUser(username);
            }else if(type.compare("userDisconnect")==0){
                QString username = jsonObj.value("username").toString();
                emit messageReceived("用户断开连接：" + username);
                emit userDisconnect(username);
            }
        }
    }
}

void ChatClient::onReadRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    while(true){
        socketStream.startTransaction();
        socketStream >> jsonData;
        if(socketStream.commitTransaction()){
            //处理接受的消息并显示：
            showMsg(jsonData);
        }else{
            break;
        }
    }
}

void ChatClient::sendMessgae(const QString &text, const QString &type)
{
    if(m_clientSocket->state() != QTcpSocket::ConnectedState){
        return;
    }

    if(!text.isEmpty()){
        QDataStream serverStream(m_clientSocket);
        serverStream.setVersion(QDataStream::Qt_5_12);

        QJsonObject message;
        message["type"] = type;
        message["text"] = text;

        serverStream << QJsonDocument(message).toJson();
    }
}

void ChatClient::connectToServer(const QHostAddress &address, qint16 port)
{
    m_clientSocket->connectToHost(address, port);
}

// 断开连接
void ChatClient::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}
