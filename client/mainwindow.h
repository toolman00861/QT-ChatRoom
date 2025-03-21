#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginBtn_clicked();

    void on_sendBtn_clicked();

    void on_quitBtn_clicked();

    void connectToServer();

    void messageReceived(const QString &msg);

    void addNewUser(const QString &username);

    void userDisconnect(const QString &username);

private:
    Ui::MainWindow *ui;

    ChatClient *m_chatClient;
};
#endif // MAINWINDOW_H
