#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QObject>
#include <tcp_object.h>

class QTcpSocket;

class TCP_client : public TCP_object
{
	Q_OBJECT
public:
	TCP_client();
	TCP_client(QString IP_ext, int Port_ext);

	QByteArray				GetDataArray();





private:
	quint16						blockSize;
	QTcpSocket				*tcpSocket;
	QByteArray				DataArray;
signals:
	//void						DataReceived();
	void							NewParameterAvailable();
	void							ClientConnected();
	void							ClientConnectionError(QString ErrorMessage);
public slots:
	QString						Connect();
	void							SendData(QString Data_ext = "NoDaTa");
	void							SendData(QByteArray Data_ext);
	QString						ReceiveData();
	QByteArray				ReceiveDataArray();
	QString						Disconnect();

	void							Connected();
	void							NetworkError();
};

#endif // TCP_CLIENT_H
