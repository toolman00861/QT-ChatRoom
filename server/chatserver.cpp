#include "chatserver.h"
#include "serverworker.h"

#include <QMessageBox>
#include <QJsonValue>
#include <QJsonObject>

ChatServer::ChatServer(QObject *parent): QTcpServer(parent){

}

//建立连接时处理事件
void ChatServer::incomingConnection(qintptr socketDescripter){
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescripter(socketDescripter)){
        worker->deleteLater();
        return;
    }
    // 连接当前消息通道到chatServer
    connect(worker, &ServerWorker::log, this, &ChatServer::log);
    connect(worker, &ServerWorker::jsonReceived, this, &ChatServer::jsonReceived);
    connect(worker, &ServerWorker::removeClient, this, &ChatServer::removeClient);
    m_clients.append(worker);

}

void ChatServer::broadcast(const QJsonObject &msg, ServerWorker *exclude)
{
    for(ServerWorker *worker : m_clients){
        // if(worker->getUser().compare(exclude->getUser())) continue;
        worker->sendMessgae(msg);
    }
}


// 把所有信息在线用户发送过去
void ChatServer::sendOnlineUser(ServerWorker *receiver)
{
    for(ServerWorker *worker : m_clients){
        // 除开将要登录的当前用户
        if(worker->getUser().compare(receiver->getUser())==0) continue;
        QString username = worker->getUser();
        QJsonObject jsonObj;
        jsonObj["type"] = "newuser";
        jsonObj["username"] = username;
        receiver->sendMessgae(jsonObj);
    }
}

void ChatServer::stopServer(){
    close();
}

// 收到新消息后广播
void ChatServer::jsonReceived(ServerWorker *sender, const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value("type");
    const QJsonValue textVal = docObj.value("text");
    if(typeVal.isNull() || !typeVal.isString()){
        return;
    }
    if(textVal.isNull() || !textVal.isString()){
        return;
    }
    QString msgType = typeVal.toString();
    QString text = textVal.toString();

    if(msgType.compare("message")==0){
        QJsonObject message;
        message["type"] = "message";
        message["text"] = text;
        message["sender"] = sender->getUser();

        broadcast(message, sender);
        emit log(sender->getUser() + ": " + text);
    }else if(msgType.compare("login")==0){
        // 将在线信息（排除新用户）发送给新用户：
        sendOnlineUser(sender);

        //广播新用户登录
        QJsonObject message;
        sender->setUser(text);
        message["type"] = "newuser";
        message["username"] = sender->getUser();
        broadcast(message, sender);
        emit log(sender->getUser() + "：用户登录");
    }
}


// 用户退出
void ChatServer::removeClient(ServerWorker *sender)
{
    m_clients.removeAll(sender);  // 从列表中移除
    if(sender != nullptr){
        // 广播通知所有用户：
        QJsonObject jsonObj;
        jsonObj["type"] = "userDisconnect";
        jsonObj["username"] = sender->getUser();
        broadcast(jsonObj);

        emit log("user disconnect: " +  sender->getUser());

        // 移除用户
        sender->deleteLater();
    }
}
