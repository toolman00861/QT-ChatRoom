#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include "serverworker.h"

class ChatServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescripter) override;
    QVector<ServerWorker*> m_clients;
    void broadcast(const QJsonObject &msg, ServerWorker *exclude = nullptr);
    void sendOnlineUser(ServerWorker *receiver);

signals:
    void log(const QString &logs);

public slots:
    void stopServer();
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void removeClient(ServerWorker *sender);
};

#endif // CHATSERVER_H
