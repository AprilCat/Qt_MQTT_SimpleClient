#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qmqtt.h"
#include "qmqtt_client.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMQTT::Client* m_client;
private slots:
    void onCleanMsg();
    void onConnected();
    void onDisconnected();
    void onSubscribe(QString topic,quint8 qos);
    void onReceive(QMQTT::Message msg);
    void onBtnConnect();
    void onBtnSubscribe();
    void onBtnPublish();
};

#endif // MAINWINDOW_H
