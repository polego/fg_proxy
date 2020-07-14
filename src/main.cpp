#include <time.h>
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <net_fdm.hxx>


#define D2R (3.14159 / 180.0)


using namespace std;

double htond (double x)	
{
    int * p = (int*)&x;
    int tmp = p[0];
    p[0] = htonl(p[1]);
    p[1] = htonl(tmp);

    return x;
}

float htonf (float x)	
{
    int * p = (int *)&x;
    *p = htonl(*p);
    return x;
}

int mySocket = -1;
struct sockaddr_in myAddr;
struct sockaddr_in fgAddr;

// IP and port where FG is listening
char my_ip[] = "127.0.0.1";
int my_port = 5501;
char fg_ip[] = "192.168.1.114";
int fg_port = 5500;

// update period.  controls how often updates are
// sent to FG.  in useconds.
int update_period = 100000;

void fg_receive() {
}

void fg_send() {
  double latitude = 45.59823; // degs
  double longitude = -120.69202; // degs
  double altitude = 150.0; // meters above sea level
  
  float roll = 0.0; // degs
  float pitch = 0.0; // degs
  float yaw = 0.0; // degs
  
  float visibility = 5000.0; // meters
  
  FGNetFDM fdm;
  memset(&fdm,0,sizeof(fdm));
  fdm.version = htonl(FG_NET_FDM_VERSION);
	
  fdm.latitude = htond(latitude * D2R);
  fdm.longitude = htond(longitude * D2R);
  fdm.altitude = htond(altitude);
  
  fdm.phi = htonf(roll * D2R);
  fdm.theta = htonf(pitch * D2R);
  fdm.psi = htonf(yaw * D2R);
  
  fdm.num_engines = htonl(1);
  
  fdm.num_tanks = htonl(1);
  fdm.fuel_quantity[0] = htonf(100.0);
  
  fdm.num_wheels = htonl(3);
  
  fdm.cur_time = htonl(time(0));
  fdm.warp = htonl(1);
  
  fdm.visibility = htonf(visibility);
  
  sendto(mySocket, (char *)&fdm, sizeof(fdm), 0, (struct sockaddr *)&fgAddr, sizeof(fgAddr));
  
  static bool flag = true;
  if (flag){
      roll += 5.0;
  } else {            
    roll -= 5.0;
  }
  flag = !flag;
}


void run() {

  while (true) {
    usleep(update_period);
    fg_send();
    fg_receive();
  }
}

int main(int argc, char ** argv)
{
  memset(&myAddr, 0, sizeof(myAddr));
  myAddr.sin_family = AF_INET;
  myAddr.sin_port = htons(my_port);
  myAddr.sin_addr.s_addr = inet_addr(my_ip);
	
  memset(&fgAddr, 0, sizeof(fgAddr));
  fgAddr.sin_family = AF_INET;
  fgAddr.sin_port = htons(fg_port);
  fgAddr.sin_addr.s_addr = inet_addr(fg_ip);

  mySocket = socket(AF_INET, SOCK_DGRAM, 0);
  if (mySocket < 0) {
    perror("socket() failed");
  }
  
  if (bind(mySocket, (const struct sockaddr *) &myAddr, sizeof(myAddr)) == -1) {
    perror("bind() failed");
    return -2;
  }
  
  // Make non-blocking
  fcntl(mySocket, F_SETFL, fcntl(mySocket, F_GETFL, 0) | O_NONBLOCK);
  
  run();
}
