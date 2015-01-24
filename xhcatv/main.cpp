// #include "mainwindow.h"
// #include <QApplication>
#include "connmgr.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>



int status = OFFLINE;
int action = START;

int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    ConnMgr ConnectionMgr;
    while(1)
    {
	    if (action == START && status == OFFLINE)
	    {
		    ConnectionMgr.Init("10.192.4.29", 5018);
		    ConnectionMgr.Connect();
		    // sleep(1);
		    ConnectionMgr.Recv(1024);
		    ConnectionMgr.Print();

		    ConnectionMgr.SendMac();
		    // sleep(1);
		    ConnectionMgr.Recv(1024);
		    ConnectionMgr.Print();

		    ConnectionMgr.SendNatversion();
		    // sleep(1);
		    // ConnectionMgr.Recv(1024);
		    // ConnectionMgr.Print();

		    ConnectionMgr.SendInformation();
		    // sleep(1);
		    ConnectionMgr.Recv(1024);
		    ConnectionMgr.Print();
		    ConnectionMgr.CheckOk();

		    // ConnectionMgr.SendChecknat();
		    // sleep(1);
		    // ConnectionMgr.Recv(1024);
		    // ConnectionMgr.Print();

	    }
	    else if (action == START && status == ONLINE)
	    {
	    	ConnectionMgr.CheckOnline();
	    }
    }

    ConnectionMgr.Close();

	// why in main.c cant use public value in class.h?
    // how to use function in class?
    // how to encode the information?

    // w.show();

    // return a.exec();
    return 0;
}
