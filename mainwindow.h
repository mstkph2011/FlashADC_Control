#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hv_control_window.h"
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

	HV_Control_window			*HVControlWindow;

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

	//open HV control window
	void								OpenHVControlWindow();
	//client stuff
	void								Connect();										//connect to server
	void								Disconnect();									//disconnect from server	
	void								SetIP();											// set IP for client to connect to
	void								SetPort();										// set Port for client to connect to
	void								Connected();									//is called when successfully connected
	void								SetErrorStatus(QString StatusString);				// is called when connection error occures
	void								ComposeAndSendParameters();
	void								ShowOutput();



	//server stuff
	void								EnableServerControls(int state);
	void								StartServer();
	void								ShutdownServer();
	void								SetIPServer();											// set IP for server to listen
	void								SetPortServer();										// set Port for server to listen


};

#endif // MAINWINDOW_H
