#include <stdio.h>
#include <string.h>

#include <iostream>
#include <ctime>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "connmgr.h"
#define TENSPACE "          "
#define INFO_PREFIX "myinformation:"

extern int status;
extern int action;

using namespace std;

ConnMgr::ConnMgr()
{
}

int ConnMgr::Init(const char *szIP, unsigned short nPort)
{
    m_buf = new char[1024];
    m_server.sin_family = AF_INET;
    m_server.sin_addr.s_addr = inet_addr(szIP);
    m_server.sin_port = htons(nPort);
    return 0;
}

int ConnMgr::Connect()
{
    int tmp;
    m_client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    tmp = connect(m_client, (struct sockaddr *)&m_server, sizeof(m_server));
    return 0;
}

int ConnMgr::Recv(int nLen)
{
    int rev, tmp;

    fd_set rfds;
    struct timeval tv;

    tv.tv_sec = 3;//3 second
    tv.tv_usec = 0;//0 micorsecond

    memset(m_buf, 0, 1024);

    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(m_client, &rfds);
        rev = select(m_client+1, &rfds, NULL, NULL, &tv);
        if (rev == -1)
        {
            perror("select");
            break;
        }
        else if (rev)//读文件集里面监控的设备文件已经准备好了（rev = 1）
        {
            tmp = recv(m_client, m_buf, nLen-1, 0);
            if (tmp == -1)
            {
                Close();
                printf("Try to reconnect...\n");
                status = OFFLINE;
            }
            break;
        }
        else//0, 超时
        {
            printf("overtime.\n");
            break;
        }
    }
    return 0;

}

int ConnMgr::SendMac()
{
    int tmp;
    char m_mac[] = "mymac-address:00-21-85-FA-F6-46";
    tmp = send(m_client, m_mac, strlen(m_mac), 0);
    return 0;
}

int ConnMgr::SendNatversion()
{
    send(m_client, "natverson:1", 11, 0);
    return 0;
}

int ConnMgr::SendInformation()
{
    char user[] = "hc39376";
    char psw[] = "654597";
    static char infobuff[] = INFO_PREFIX TENSPACE TENSPACE TENSPACE TENSPACE "  ";
    char mp[] = "01234";
    char *e_uid = infobuff + strlen(INFO_PREFIX);
    char *e_psw = e_uid + 20;
    char *plan = e_psw + 20;
    Encoder(user, e_uid, 20);
    Encoder(psw, e_psw, 20);
    plan[0]=mp[1%5];//那就是套餐1
    send(m_client, infobuff, strlen(infobuff), 0);
    return 0;
}

int ConnMgr::SendChecknat()
{
    send(m_client, "checknat", 8, 0);
    return 0;
}

int ConnMgr::Close()
{
    close(m_client);
    return 0;
}

int ConnMgr::CheckOk()
{
    int tmp;
    tmp = strncmp("passok", m_buf, 6);
    // printf("m_buf = %s\n", m_buf);
    // printf("tmp = %d\n", tmp);
    if (tmp == 0)
    {
        status = ONLINE;
    }
    else
        status = OFFLINE;
    return 0;

}


int ConnMgr::CheckOnline()
{
    int ret;
    //time_t now_time;
    time_t timep;
    //now_time= time(NULL);
    sleep(20);
    ret = send(m_client, "qr", 2, 0);
    // send(m_client, "showinfo", 8, 0);
    // ret = Recv(1024);
    // printf("ret = %d\n", ret);
    // printf("showinfo = %s\n", m_buf);
    if (ret == -1)
    {
        printf("Offline!\n");
        Close();
        status = OFFLINE;
    }
    else
    {

	time (&timep);
	printf("%s",ctime(&timep));
	//cout << now_time;
    }
    return 0;
}

int ConnMgr::Print()
{
    printf("%s\n", m_buf);
    return 0;
}

int ConnMgr::Encoder(char *from, char *to, int lenTo)
{
    char mp[] = "0123456789ABCDEF";
    int len = strlen(from), i, j;
    unsigned char key = (unsigned char)((len & 0xff) ^ 0xa5);
    if(!to)
        return -1;
    for(i=0,j=0; i<len && j+1 < lenTo; i++)
    {
        key = from[i] ^ key;
        to[j++] = mp[(key&0xf0)>>4];
        to[j++] = mp[key&0xf];
    }
    return 0;
}
