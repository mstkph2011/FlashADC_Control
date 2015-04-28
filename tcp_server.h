#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QObject>
#include "tcp_object.h"

class QTcpServer;
class QString;
class QTcpSocket;

class TCP_server : public TCP_object
{
	Q_OBJECT
public:
	TCP_server();
	TCP_server(QString IP_ext, int Port_ext);
private :
	QTcpServer				*tcpServer;
	bool							ClientConnected;



signals:
	void							ServerClosed();
public slots:
	void							AcceptConnection();
	QString						StartRead();
	QString						StartServer();
	QString						ShutdownServer();

};

#endif // TCP_SERVER_H
