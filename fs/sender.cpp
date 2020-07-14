#include "sender.h"
#include "mainwindow.h"
#include "socket_util.h"
#include "main.h"

sender::sender()
{

}

void sender::run(const char* local_ip, int local_port, const char* remote_ip, int remote_port)
{
    udp_link_t link;
    size_t BUFFER_SIZE = 1500;
    char buffer[BUFFER_SIZE];

    int res;
    res = socket_init(&link, local_ip, local_port, remote_ip, remote_port);


    while(1){
        MainWindow *window = getWindowHandle();
        int yaw = window->getYaw();
        int roll = window->getRoll();
        int pitch = window->getPitch();
        int thrust = window->getThrust();
        //socket_send();
    }
}
