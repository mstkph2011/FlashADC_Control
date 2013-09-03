#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TCP_client;
class TCP_server;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
	void								SetParameter();
	void								ShowParameter();
	QByteArray 					ComposeSendData();
	void								ExtractReceivedData();
private:
	Ui::MainWindow *ui;

	TCP_client					*Client;
	TCP_server					*Server;
	QByteArray					MessageOut;
	QByteArray					MessageIn;
	// parameters of fadc_crtl
	qint32 par_ctrl;
	qint32 no_of_modules;
	qint32 post_trigger;
	qint32 coinc_level;
	qint32 custom_size;
	quint8 ov_thr_on;
	quint8 ext_trig_in;
	quint8 ext_trig_out;
	quint8 trace_length;
	qint32 sample_dec;
	quint16 risetime;
	quint16 average_win;
	quint16 decay_time;
	quint16 peaking_time;
	quint16 gap_time;
	quint16 peak_shift;
	quint16 thresholdctrl;
	quint16 flattop_del;
	qint32 decimation;
	qint32 peak_average;

	//parametes of fadc_par
	quint8 ena_fadc[8];
	quint16 offset[8];
	quint16 threshold[8];
	quint16 ov_un_thresh[8];
	quint8 ena_trig[8];

public slots:
	void								Send();
	void								Connect();
	void								Connected();									//is called when successfully connected
	void								SetErrorStatus(QString StatusString);				// is called when not connected
	void								SetIPAndPort();
	void								SetIPAndPortServer();
	void								StartServer();
	void								ShutdownServer();
	void								ShowClientData();
	void								ShowServerData();
	void								ServerClosed();

	void								ShowOutput();



	void								Makeoutput();
};

#endif // MAINWINDOW_H
