#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatserver.h"

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
    void on_startStopServer_clicked();

public slots:
    void log(const QString &logs);

private:
    Ui::MainWindow *ui;

    ChatServer *m_chatServer;
};
#endif // MAINWINDOW_H
