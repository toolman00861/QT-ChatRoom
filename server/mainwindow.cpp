#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_chatServer = new ChatServer(this);

    connect(m_chatServer, &ChatServer::log, this, &MainWindow::log);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopServer_clicked()
{
    if(m_chatServer->isListening()){
        m_chatServer->stopServer();
        ui->startStopServer->setText("启动服务器");
        log("服务器暂停");
    }else{
        if(!m_chatServer->listen(QHostAddress::Any, 1967)){
            QMessageBox::critical(this, "错误", "无法启动服务器，请检查");
            return;
        }
        log("服务器启动");
        ui->startStopServer->setText("暂停服务器");
    }
}

void MainWindow::log(const QString &logs){
    ui->serverLog->append(logs);
}

