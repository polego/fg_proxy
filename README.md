# fg_proxy
Application to control and monitior Flightgear flight simulator

Start FlightGear as:
>fgfs --httpd=8080 --disable-random-objects --fdm=external --timeofday=noon --native-fdm=socket,in,1,,5500,udp


fgfs --httpd=8080 --disable-random-objects --timeofday=noon --native-fdm=socket,out,1,192.168.1.74,5501,udp

# FG receive external pilote comands
fgfs --httpd=8080 --disable-random-objects --timeofday=noon --native-ctrls=socket,in,10,,5500,udp

# FG send pilote comands
fgfs --httpd=8080 --disable-random-objects --timeofday=noon --native-ctrls=socket,out,10,192.168.1.74,5501,udp

To work between two hosts disable firewall (should really only open ports)
>sudo systemctl stop firewalld

Test with netcat:
>nc -u -l 192.168.1.114 5500 (host1)
>nc -u 192.168.1.114 5500 (host2)

# To send into virtual box add port forwarding