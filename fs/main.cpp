#include "main.h"
#include "mainwindow.h"
#include "receiver.h"
#include "sender.h"
#include <QApplication>
#include <thread>

const static char *r_local_ip = "1234";
const static int r_local_port = 1234;
const static char *r_remote_ip = "1234";
const static int r_remote_port = 1234;

const static char *s_local_ip = "1234";
const static int s_local_port = 1234;
const static char *s_remote_ip = "1234";
const static int s_remote_port = 1234;

MainWindow *windowPointer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    windowPointer = &w;
    w.show();

    receiver r;
    std::thread t1(&receiver::run, &r, r_local_ip, r_local_port, r_remote_ip, r_remote_port);

    sender s;
    std::thread t2(&sender::run, &s, s_local_ip, s_local_port, s_remote_ip, s_remote_port);

    return a.exec();
}

MainWindow *getWindowHandle()
{
    return windowPointer;
}
