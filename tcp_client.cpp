#include "tcp_client.h"
#include <QtNetwork>

#include <QByteArray>
#include <QLabel>
#include <QString>
#include <QHostAddress>

#include <stdlib.h>
#include <stdio.h>


TCP_client::TCP_client()
{
	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveData()));
}
TCP_client::TCP_client(QString IP_ext, int Port_ext)
{
	tcpSocket = new QTcpSocket(this);
	SetIP(IP_ext);
	SetPort(Port_ext);
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveDataArray()));
	connect(tcpSocket, SIGNAL(connected()),this, SLOT(Connected()));
	connect(tcpSocket, SIGNAL(disconnected()),this, SLOT(Disconnected()));
	connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(NetworkError()));
}
QString TCP_client::Connect()
{
	if (!tcpSocket->state())
	{

		tcpSocket->connectToHost(IP, Port);
		return QString("Client -- Connecting");
	}
	else
	{
		qDebug("Client -- Connection failed (already connected)!");
		return QString("Client -- Connection failed (already connected)!");
	}
}

void TCP_client::Connected()
{
	emit ClientConnected();
	qDebug("Client -- Connected!");
	//qDebug("%i",tcpSocket->state());
}
void TCP_client::NetworkError()
{
	qDebug(tcpSocket->errorString().toUtf8().data());
	emit ClientConnectionError(tcpSocket->errorString());
}

void TCP_client::SendData(QString Data_ext )
{
	QByteArray Data;
	if (Data_ext.compare(QString("NoDaTa"),Qt::CaseSensitive))
		Data.append(Data_ext);
	else
		Data.append("Hello World!");
	tcpSocket->write(Data,Data.length());
}

void TCP_client::SendData(QByteArray Data_ext )
{
	tcpSocket->write(Data_ext,Data_ext.length());
}

QString TCP_client::ReceiveData()
{
		char buffer[1024] = {0};
		tcpSocket->read(buffer, tcpSocket->bytesAvailable());
		qDebug(buffer);
		Data = QString(buffer);
		emit NewDataAvailable();
		return QString(buffer);
}
QByteArray TCP_client::ReceiveDataArray()
{
	char buffer[1024] = {0};
	bool EmitParameterSignal = false;
	qDebug("BA: %i", (int)tcpSocket->bytesAvailable());
	if ((int)tcpSocket->bytesAvailable() >100)
	{
		EmitParameterSignal = true;
		DataArray = tcpSocket->read( tcpSocket->bytesAvailable());
	}
	else
	{
		tcpSocket->read(buffer, tcpSocket->bytesAvailable());
		qDebug(buffer);
	}


	if(EmitParameterSignal)
		emit NewParameterAvailable();
	else
		emit NewDataAvailable();
	return DataArray;
}

QString	TCP_client::Disconnect()
{
	if (tcpSocket->state())
	{
		tcpSocket->disconnectFromHost();
		qDebug("Client -- Disconnected!");
		return QString("Disconnected from Server");
	}
	else
	{
		qDebug("Client -- Disconnect failed (not connected)!");
		return QString("Disconnected (Connection lost)");
	}
}

QByteArray TCP_client::GetDataArray()
{
	return DataArray;
}

void TCP_client::Disconnected()
{
	emit ClientDisconnected();
}
