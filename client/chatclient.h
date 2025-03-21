#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>

class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    void showMsg(QByteArray jsonData); // 解析并展示收到的json对象

signals:
    void connected();
    void messageReceived(const QString &msg);
    void addNewUser(const QString &username);
    void userDisconnect(const QString &username);

private:
    QTcpSocket *m_clientSocket;

public slots:
    void onReadRead();
    void sendMessgae(const QString &text, const QString &type = "message");
    void connectToServer(const QHostAddress &address, qint16 port);
    void disconnectFromHost();
};

#endif // CHATCLIENT_H
