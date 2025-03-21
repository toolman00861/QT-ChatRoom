#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
    ui->serverAddress->setText("127.0.0.1");
    m_chatClient = new ChatClient(this);

    connect(m_chatClient, &ChatClient::messageReceived, this, &MainWindow::messageReceived);
    connect(m_chatClient, &ChatClient::connected, this, &MainWindow::connectToServer);
    connect(m_chatClient, &ChatClient::addNewUser, this, &MainWindow::addNewUser);
    connect(m_chatClient, &ChatClient::userDisconnect, this, &MainWindow::userDisconnect);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    QString HostAddress = ui->serverAddress->text();
    m_chatClient->connectToServer(QHostAddress(HostAddress), 1967);
}


void MainWindow::on_sendBtn_clicked()
{
    QString msg = ui->sendMassage->text();
    if(!msg.isEmpty()){
        m_chatClient->sendMessgae(msg, "message");
    }
}

//断开连接
void MainWindow::on_quitBtn_clicked()
{
    m_chatClient->disconnectFromHost();
    // 清空文本框并转跳
    ui->onlineBox->clear();
    ui->chatBox->clear();
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

//连接上服务器后跳转到聊天页面 127.0.0.1
void MainWindow::connectToServer()
{
    ui->stackedWidget->setCurrentWidget(ui->chatPage);
    QString username = ui->username->text();
    m_chatClient->sendMessgae(username, "login");
}

void MainWindow::messageReceived(const QString &msg)
{
    ui->chatBox->append(msg);
}


// 有新用户加进来：向对话框展示
void MainWindow::addNewUser(const QString &username)
{
    ui->onlineBox->append(username);
}

// 有用户断开连接：
void MainWindow::userDisconnect(const QString &username)
{
    QStringList onlineUsers = ui->onlineBox->toPlainText().split('\n');
    onlineUsers.removeAll(username);
    ui->onlineBox->setPlainText(onlineUsers.join('\n'));
}

