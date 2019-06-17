#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new QMQTT::Client();
    connect(ui->btn_connect,SIGNAL(clicked()),this,SLOT(onBtnConnect()));
    connect(ui->btn_sub,SIGNAL(clicked()),this,SLOT(onBtnSubscribe()));
    connect(ui->btn_pub,SIGNAL(clicked()),this,SLOT(onBtnPublish()));
    connect(ui->btn_clean,SIGNAL(clicked()),this,SLOT(onCleanMsg()));
    connect(m_client,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(m_client,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    connect(m_client,SIGNAL(subscribed(QString,quint8)),this,SLOT(onSubscribe(QString,quint8)));
    connect(m_client,SIGNAL(received(QMQTT::Message)),this,SLOT(onReceive(QMQTT::Message)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBtnConnect()
{
    m_client->setHost(QHostAddress(ui->lineEdit_IP->text()));
    m_client->setPort(ui->lineEdit_Port->text().toUInt());
    m_client->setUsername(ui->lineEdit_username->text());
    m_client->setPassword(ui->lineEdit_password->text().toLatin1());
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QString clientID = QString("client_%1").arg(qrand()%1000);
    m_client->setClientId(clientID);
    m_client->connectToHost();
}

void MainWindow::onBtnSubscribe()
{
    m_client->subscribe(ui->lineEdit_subTopic->text(),1);
}

void MainWindow::onSubscribe(QString topic, quint8 qos)
{
    QString msg;
    msg = "subscribe topic:";
    msg += topic;
    msg += QString(" success.QOS:%1\r\n").arg(qos);
    ui->textEdit_msg->append(msg);
}

void MainWindow::onBtnPublish()
{
    QMQTT::Message msg;
    msg.setTopic(ui->lineEdit_pubTopic->text());
    msg.setPayload(ui->lineEdit_pubContent->text().toLatin1());
    m_client->publish(msg);
}

void MainWindow::onReceive(QMQTT::Message msg)
{
    QString qmsg;
    qmsg = QString("receive topic:%1 payload:%2").arg(msg.topic()).arg(QString(msg.payload()));
    ui->textEdit_msg->append(qmsg);
}

void MainWindow::onCleanMsg()
{
    ui->textEdit_msg->clear();
}

void MainWindow::onConnected()
{
    QString msg;
    msg  = "client connect to [";
    msg += ui->lineEdit_IP->text();
    msg += ":";
    msg += ui->lineEdit_Port->text();
    msg += "] success\r\n";
    ui->textEdit_msg->append(msg);
}

void MainWindow::onDisconnected()
{
    QString msg;
    msg = "client disconnect.\r\n";
    ui->textEdit_msg->append(msg);
}
