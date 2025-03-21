#include "serverworker.h"
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

ServerWorker::ServerWorker(QObject *parent)
    : QObject{parent}
{
    m_serverSocket = new QTcpSocket(this);
    connect(m_serverSocket, &QTcpSocket::readyRead, this, &ServerWorker::onReadRead);
    connect(m_serverSocket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectFromClient);
}

bool ServerWorker::setSocketDescripter(qintptr socketDescripter)
{
    return m_serverSocket->setSocketDescriptor(socketDescripter);
}

void ServerWorker::sendJsonDocument(QByteArray jsonData)
{
    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if(parseError.error == QJsonParseError::NoError){
        if(jsonDoc.isObject()){
            emit jsonReceived(this, jsonDoc.object());
        }
    }
}

QString ServerWorker::getUser()
{
    return username;
}

void ServerWorker::setUser(QString username)
{
    this->username = username;
}

void ServerWorker::onReadRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_12);
    while(true){
        socketStream.startTransaction();
        socketStream >> jsonData;
        if(socketStream.commitTransaction()){
            // emit log(QString::fromUtf8(jsonData));
            // sendMessgae("i receved message");

            sendJsonDocument(jsonData);

        }else{
            break;
        }
    }
}


void ServerWorker::sendMessgae(const QJsonObject &jsonObj)
{
    const QByteArray jsonData = QJsonDocument(jsonObj).toJson();
    QDataStream serverStream(m_serverSocket);
    serverStream.setVersion(QDataStream::Qt_5_12);
    serverStream << jsonData;
}

void ServerWorker::disconnectFromClient()
{
    // 传入this上下文
    emit removeClient(this);
}
