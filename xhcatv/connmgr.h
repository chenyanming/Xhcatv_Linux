#ifndef CONNMGR_H
#define CONNMGR_H
//#include "protocolhelper.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#define START 1
#define STOP 0
#define ONLINE 1
#define OFFLINE 0

class ConnMgr
{
public:
    ConnMgr();
    int Init(const char *, unsigned short);
    int Connect();
    int Recv(int);
    int SendMac();
    int SendNatversion();
    int SendInformation();
    int SendChecknat();
	int CheckOk();
	int CheckOnline();
	int Close();
    int Print();
private:
	int Encoder(char *from, char *to, int lenTo);
    struct sockaddr_in m_server;
    int m_client;
    char *m_buf;
};

#endif // CONNMGR_H
