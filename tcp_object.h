#ifndef TCP_OBJECT_H
#define TCP_OBJECT_H

#include <QObject>
#include <QHostAddress>
class QHostAddress;
class QString;
class QTcpSocket;

class TCP_object : public QObject
{
	Q_OBJECT
public:
	explicit TCP_object(QObject *parent = 0);
//	~TCP_object();

	QString								GetData();
	QTcpSocket						*Socket;

	
protected:
	QHostAddress					IP;
	int										Port;
	QString								Data;
private:
signals:
	void									NewDataAvailable();
public slots:
	void									SetIP(QString IP_ext);
	void									SetPort(int Port_ext);


	QString								GetIPString();
	QHostAddress					GetIP();
	int										GetPort();


};

#endif // TCP_OBJECT_H
