# fg_proxy
Application to control and monitior Flightgear flight simulator

Start FlightGear as:
>fgfs --aircraft=c172 --disable-random-objects --fdm=external --timeofday=noon --native-fdm=socket,in,1,,5500,udp


To work between two hosts disable firewall (should really only open ports)
>sudo systemctl stop firewalld

Test with netcat:
>nc -u -l 192.168.1.114 5500 (host1)
>nc -u 192.168.1.114 5500 (host2)