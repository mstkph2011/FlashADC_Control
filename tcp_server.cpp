#include "tcp_server.h"
#include <QtNetwork>

#include <QByteArray>
#include <QLabel>
#include <QString>
#include <QHostAddress>

#include <QDebug>

TCP_server::TCP_server()
{
	tcpServer = new QTcpServer(this);
	int PortNumber = 12341;
			if (!tcpServer->listen(QHostAddress::Any, PortNumber))
			{
				qDebug("Failed to open server!");
					return;
			}

			connect(tcpServer, SIGNAL(newConnection()), this, SLOT(AcceptConnection()));
			connect(Socket, SIGNAL(readyRead()), this, SLOT(startRead()));
}

TCP_server::TCP_server(QString IP_ext, int Port_ext)
{
	tcpServer = new QTcpServer(this);
	SetIP(IP_ext);
	SetPort(Port_ext);
	ClientConnected = false;
	//StartServer();

	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(AcceptConnection()));

}

void TCP_server::AcceptConnection()
{
	Socket = tcpServer->nextPendingConnection();
	connect(Socket, SIGNAL(disconnected()), Socket, SLOT(deleteLater()));
	connect(Socket, SIGNAL(readyRead()), this, SLOT(StartRead()));

	Socket->write("Connection established!");
	qDebug("Connected");
	ClientConnected = true;
}


QString TCP_server::StartRead()
{
	char *buffer;
	QByteArray BuffArray =Socket->read( Socket->bytesAvailable());
	buffer= BuffArray.data();
	qDebug(buffer);
	//Data = QString(buffer);
	Socket->write(BuffArray);
	emit NewDataAvailable();
	return QString(buffer);

}

QString TCP_server::StartServer()
{
	if (tcpServer->isListening())
	{
			QString Message = QString("Server already opened on Adress %1 and Port %2. Close to open new Server!").arg(tcpServer->serverAddress().toString()).arg(tcpServer->serverPort());
			qDebug(Message.toUtf8().data());
			return Message;

	}
	else
	{
		if (!tcpServer->listen(GetIP(), GetPort()))
		{
			qDebug("Failed to open server!");
			return QString("Failed to Open Server");
		}
		else
		{
			QString SuccessMessage = QString("Connection established. Listening on Adress %1 and Port %2").arg(tcpServer->serverAddress().toString()).arg(tcpServer->serverPort());
			qDebug(SuccessMessage.toUtf8().data());
			return SuccessMessage;
		}
	}

}

QString TCP_server::ShutdownServer()
{

	//Socket->disconnectFromHost();
	if (ClientConnected)
		//if (Socket->state() == 3)
			Socket->write("Server closed!");
	tcpServer->close();
	ClientConnected=false;
	qDebug("Server closed");
	emit ServerClosed();
	return QString("Server closed");
}
