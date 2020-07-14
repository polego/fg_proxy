#include "receiver.h"
#include "socket_util.h"
#include "mainwindow.h"
#include "main.h"

receiver::receiver()
{

}

void receiver::run(const char* local_ip, int local_port, const char* remote_ip, int remote_port)
{
    udp_link_t link;
    size_t BUFFER_SIZE = 1500;
    char buffer[BUFFER_SIZE];

    int res;
    res = socket_init(&link, local_ip, local_port, remote_ip, remote_port);

    while(1){
        socket_receive(&link, buffer, BUFFER_SIZE);
        double x;
        double y;
        MainWindow *window = getWindowHandle();
        window->addPointCurve(x, y);
    }
}
