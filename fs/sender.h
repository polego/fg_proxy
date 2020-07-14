#ifndef SENDER_H
#define SENDER_H


class sender
{
public:
    sender();
    [[noreturn]] void run(const char* local_ip, int local_port, const char* remote_ip, int remote_port);
};

#endif // SENDER_H
