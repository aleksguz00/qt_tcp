#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void on_connect_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QByteArray data;
    quint16 next_block_size;

    void SendToServer(QString str);

public slots:
    void slotReadyRead();
};
#endif // MAINWINDOW_H
