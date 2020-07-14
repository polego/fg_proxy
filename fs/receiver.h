#ifndef RECEIVER_H
#define RECEIVER_H

class receiver
{
public:
    receiver();
    [[noreturn]] void run(const char* local_ip, int local_port, const char* remote_ip, int remote_port);
private:

};

#endif // RECEIVER_H
