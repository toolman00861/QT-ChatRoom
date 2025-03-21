#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescripter(qintptr socketDescripter);
    void sendJsonDocument(QByteArray jsonData);
    QString getUser();
    void setUser(QString username);

signals:
    void log(const QString &logs);
    void jsonReceived(ServerWorker *sender, const QJsonObject &docObj);
    void removeClient(ServerWorker *sender);  // 用来告诉服务器用户断开连接。

private:
    QTcpSocket *m_serverSocket;
    QString username;

public slots:
    void onReadRead();
    // void sendMessgae(const QString &text, const QString &type = "message");
    void sendMessgae(const QJsonObject &jsonObj);
    void disconnectFromClient();  // 接收用户断开连接的消息
};

#endif // SERVERWORKER_H
