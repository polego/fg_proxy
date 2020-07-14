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
#define R2D (180.0 / 3.14159)


using namespace std;

int mySocket = -1;
struct sockaddr_in myAddr;
struct sockaddr_in fgAddr;

// IP and port where FG is listening
char my_ip[] = "10.0.2.15";
int my_port = 5501;
char fg_ip[] = "192.168.1.114";
int fg_port = 5500;

// update period.  controls how often updates are
// sent to FG.  in useconds.
int update_period = 500000;


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

void pbytes(const char* bytes, const size_t sz) {
  for (size_t i=0; i<sz; i++) {
    printf("%02hhX", *(bytes+i));
  }
  printf("\n");
}

void fg_receive() {
  FGNetFDM fdm;
  struct sockaddr_in fgAddr;
  socklen_t si_len = sizeof(fgAddr);
  int len;

  // Clear reception buffer
  memset((char*)&fdm, '\0', sizeof(fdm));
  
  if ((len = recvfrom(mySocket, &fdm, sizeof(fdm), 0, (struct sockaddr*) &(fgAddr), &si_len)) == -1) {
    printf("No data received\n");
  } else {
    printf("Recieved packet of size %i, from %s:%d\n",
	   len,
	   inet_ntoa(fgAddr.sin_addr), 
	   ntohs(fgAddr.sin_port));
    printf("bytes: ");
    pbytes((char*)&fdm, len);
    printf("\n");
  }
}

void fg_send() {
  static bool flag = true;
  static double latitude = 58.406; // degs
  static double longitude = 15.673; // degs
  static double altitude = 200.0; // meters above sea level
  static float roll = 0.0; // degs
  static float pitch = 0.0; // degs
  static float yaw = 0.0; // degs

  int len;

  latitude+= 0.001;
  longitude += 0.001;
  
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
  
  len = sendto(mySocket, (char *)&fdm, sizeof(fdm), 0, (struct sockaddr *)&fgAddr, sizeof(fgAddr));
  printf("Successfully sent packet of size %i, to %s:%d\n",
	 len,
	 inet_ntoa(fgAddr.sin_addr), 
	 ntohs(fgAddr.sin_port));
  
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

  mySocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
