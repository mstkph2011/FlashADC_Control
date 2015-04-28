#include "tcp_object.h"

#include <QtNetwork>


TCP_object::TCP_object(QObject *parent) :
	QObject(parent)
{
}


void TCP_object::SetIP(QString IP_ext)
{
	if (IP_ext.contains("local",Qt::CaseInsensitive))
		IP= QHostAddress::LocalHost;
	else
		if(!IP.setAddress(IP_ext))
			qDebug("IP Setting failed!");
}

void TCP_object::SetPort(int Port_ext)
{
	Port = Port_ext;
}


QString TCP_object::GetIPString()
{
	return IP.toString();
}

QHostAddress TCP_object::GetIP()
{
	return IP;
}

int TCP_object::GetPort()
{
	return Port;
}

QString	TCP_object::GetData()
{
	return Data;
}

