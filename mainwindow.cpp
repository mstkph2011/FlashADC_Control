#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tcp_client.h"
#include "tcp_server.h"
#include <QThread>

#include <stdio.h>
#include <stdlib.h>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->ledCoincLevelInput->setInputMask("9");
	SetParameter();
	Client = new TCP_client(ui->ledIP->text(),ui->ledPort->text().toInt());
	Server = new TCP_server(ui->ledIPServer->text(),ui->ledPortServer->text().toInt());


	//client stuff
		// connect/disconnect buttons
	connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(Connect()));								// connect client
	connect(ui->btnDisconnect,SIGNAL(clicked()), this, SLOT(Disconnect()));						// disconnect client
		// connect/disconnect/error status messages
	connect(Client, SIGNAL(ClientConnected()), this, SLOT(Connected()));
	connect(Client, SIGNAL(ClientDisconnected()), this, SLOT(Disconnect()));
	connect(Client, SIGNAL(ClientConnectionError(QString)),this, SLOT(SetErrorStatus(QString)));
		//update ip/port
	connect(ui->ledIP,SIGNAL(editingFinished()),this, SLOT(SetIP()));
	connect(ui->ledPort,SIGNAL(editingFinished()),this, SLOT(SetPort()));
		//send new parameters to server
	connect(ui->btnUpdateParameter,SIGNAL(clicked()),this, SLOT(ComposeAndSendParameters()));
		// print new parameters in labels, after received from server
	connect(Client, SIGNAL(NewParameterAvailable()),this, SLOT(ShowOutput()));


	//Server stuff

		// enable server controls
	connect(ui->cbEnableServer,SIGNAL(stateChanged(int)), this, SLOT(EnableServerControls(int)));
		// start/shutdown server
	connect(ui->btnStartServer, SIGNAL(clicked()), this, SLOT(StartServer()));
	connect(ui->btnShutdownServer, SIGNAL(clicked()), this, SLOT(ShutdownServer()));
		//update server ip/port
	connect(ui->ledIPServer,SIGNAL(editingFinished()),this, SLOT(SetIPServer()));
	connect(ui->ledPortServer,SIGNAL(editingFinished()),this, SLOT(SetPortServer()));


}

MainWindow::~MainWindow()
{
	delete ui;
}

//client stuff

void MainWindow::Connect()				//connect client
{

	Client->Connect();
	Client->SetIP(ui->ledIP->text());

	ui->btnConnect->setEnabled(false);
	ui->btnDisconnect->setEnabled(true);
	statusBar()->showMessage("Connecting...");
}

void MainWindow::Disconnect()			//disconnect client, also shows disconnected status
{

	statusBar()->showMessage(Client->Disconnect());
	ui->btnConnect->setEnabled(true);
	ui->btnDisconnect->setEnabled(false);
}

void MainWindow::SetIP()					//set ip to connect to
{
	Client->SetIP(ui->ledIP->text());

}

void MainWindow::SetPort()				//set port to connect to
{
	Client->SetPort(ui->ledPort->text().toInt());
}

void MainWindow::Connected()			//show successfully connected status
{
		QString status =  QString("Succesfully connected to Server on ") + Client->GetIPString() + QString(":") + QString("%1").arg(Client->GetPort());
	statusBar()->showMessage(status);

}

void MainWindow::SetErrorStatus(QString StatusString)			//show error status
{
	ui->btnConnect->setEnabled(true);
	ui->btnDisconnect->setEnabled(false);
	statusBar()->showMessage(StatusString);
}


void MainWindow::SetParameter()			// get parameters from ui
{
	// parameters of fadc_crtl
	par_ctrl = (qint32) ui->ledParControlInput->text().toInt();//2;
	no_of_modules = (qint32) ui->ledNoOfModulesInput->text().toInt();//1
	post_trigger = (qint32) ui->ledPostTriggerInput->text().toInt();
	coinc_level = (qint32) ui->ledCoincLevelInput->text().toInt();
	custom_size = (qint32) 0;											// not used
	if(ui->cbPositiveTriggerInput->checkState())
		ov_thr_on = (quint8) 1;
	else
		ov_thr_on = (quint8) 0;
	if (ui->cbExtTriggerInInput->checkState())
		ext_trig_in = 1;
	else
		ext_trig_in = 0;
	if (ui->cbExtTriggerOutInput->checkState())
		ext_trig_out = 1;
	else
		ext_trig_out = 0;
	trace_length = (quint8) ui->ledTraceLengthInput->text().toInt();
	sample_dec = (qint32) 0;											// not used
	risetime = (quint16) 0;											// not used;
	average_win = (quint16) 0;											// not used;;
	decay_time = (quint16) 0;											// not used;;
	peaking_time = (quint16) 0;											// not used;;
	gap_time = (quint16) 0;											// not used;
	peak_shift= (quint16) 0;											// not used;
	thresholdctrl = (quint16) 0;											// not used;
	flattop_del = (quint16) 0;											// not used;
	decimation  = (qint32) 0;											// not used
	peak_average  = (qint32) 0;											// not used

	//parametes of fadc_par

	if(ui->cbEnableChannelCh0Input->checkState())
		ena_fadc[0] = 1;
	else
		ena_fadc[0] = 0;
	if(ui->cbEnableChannelCh1Input->checkState())
		ena_fadc[1] = 1;
	else
		ena_fadc[1] = 0;
	if(ui->cbEnableChannelCh2Input->checkState())
		ena_fadc[2] = 1;
	else
		ena_fadc[2] = 0;
	if(ui->cbEnableChannelCh3Input->checkState())
		ena_fadc[3] = 1;
	else
		ena_fadc[3] = 0;
	if(ui->cbEnableChannelCh4Input->checkState())
		ena_fadc[4] = 1;
	else
		ena_fadc[4] = 0;
	if(ui->cbEnableChannelCh5Input->checkState())
		ena_fadc[5] = 1;
	else
		ena_fadc[5] = 0;
	if(ui->cbEnableChannelCh6Input->checkState())
		ena_fadc[6] = 1;
	else
		ena_fadc[6] = 0;
	if(ui->cbEnableChannelCh5Input->checkState())
		ena_fadc[7] = 1;
	else
		ena_fadc[7] = 0;

	offset[0] = ui->ledOffsetCh0Input->text().toInt();
	offset[1] = ui->ledOffsetCh1Input->text().toInt();
	offset[2] = ui->ledOffsetCh2Input->text().toInt();
	offset[3] = ui->ledOffsetCh3Input->text().toInt();
	offset[4] = ui->ledOffsetCh4Input->text().toInt();
	offset[5] = ui->ledOffsetCh5Input->text().toInt();
	offset[6] = ui->ledOffsetCh6Input->text().toInt();
	offset[7] = ui->ledOffsetCh7Input->text().toInt();

	threshold[0] = ui->ledThresholdCh0Input->text().toInt();
	threshold[1] = ui->ledThresholdCh1Input->text().toInt();
	threshold[2] = ui->ledThresholdCh2Input->text().toInt();
	threshold[3] = ui->ledThresholdCh3Input->text().toInt();
	threshold[4] = ui->ledThresholdCh4Input->text().toInt();
	threshold[5] = ui->ledThresholdCh5Input->text().toInt();
	threshold[6] = ui->ledThresholdCh6Input->text().toInt();
	threshold[7] = ui->ledThresholdCh7Input->text().toInt();

	ov_un_thresh[0] = ui->ledSamplesToTriggerCh0Input->text().toInt();
	ov_un_thresh[1] = ui->ledSamplesToTriggerCh1Input->text().toInt();
	ov_un_thresh[2] = ui->ledSamplesToTriggerCh2Input->text().toInt();
	ov_un_thresh[3] = ui->ledSamplesToTriggerCh3Input->text().toInt();
	ov_un_thresh[4] = ui->ledSamplesToTriggerCh4Input->text().toInt();
	ov_un_thresh[5] = ui->ledSamplesToTriggerCh5Input->text().toInt();
	ov_un_thresh[6] = ui->ledSamplesToTriggerCh6Input->text().toInt();
	ov_un_thresh[7] = ui->ledSamplesToTriggerCh7Input->text().toInt();

	if(ui->cbEnableTriggerCh0Input->checkState())
		ena_trig[0] = 1;
	else
		ena_trig[0] = 0;
	if(ui->cbEnableTriggerCh1Input->checkState())
		ena_trig[1] = 1;
	else
		ena_trig[1] = 0;
	if(ui->cbEnableTriggerCh2Input->checkState())
		ena_trig[2] = 1;
	else
		ena_trig[2] = 0;
	if(ui->cbEnableTriggerCh3Input->checkState())
		ena_trig[3] = 1;
	else
		ena_trig[3] = 0;
	if(ui->cbEnableTriggerCh4Input->checkState())
		ena_trig[4] = 1;
	else
		ena_trig[4] = 0;
	if(ui->cbEnableTriggerCh5Input->checkState())
		ena_trig[5] = 1;
	else
		ena_trig[5] = 0;
	if(ui->cbEnableTriggerCh6Input->checkState())
		ena_trig[6] = 1;
	else
		ena_trig[6] = 0;
	if(ui->cbEnableTriggerCh5Input->checkState())
		ena_trig[7] = 1;
	else
		ena_trig[7] = 0;

}

QByteArray MainWindow::ComposeSendData()	// compose data stream to send to the server
{
	quint32 DataLength = 116;

	QDataStream OutData(&MessageOut, QIODevice::WriteOnly);
	OutData.setVersion(QDataStream::Qt_4_7);

	OutData << DataLength;
	OutData << par_ctrl << no_of_modules <<  post_trigger<< coinc_level << custom_size << ov_thr_on << ext_trig_in << ext_trig_out << trace_length << sample_dec << risetime << average_win << decay_time << peaking_time << gap_time << peak_shift << thresholdctrl	<< flattop_del << decimation << peak_average;
	for(int i = 0;i<8;i++)
		OutData << ena_fadc[i];
	for(int i = 0;i<8;i++)
		OutData << offset[i];
	for(int i = 0;i<8;i++)
		 OutData << threshold[i];
	for(int i = 0;i<8;i++)
		OutData << ov_un_thresh[i];
	for(int i = 0;i<8;i++)
		OutData << ena_trig[i];
	return MessageOut;
}

void MainWindow::ComposeAndSendParameters()		//get parameters from ui and send composed data stream to server
{
	SetParameter();
	Client->SendData(ComposeSendData());
}

void MainWindow::ExtractReceivedData()		// decompose data values from server answer stream
{
	quint32 DataLength;
	QDataStream InData(&MessageIn,QIODevice::ReadOnly);
	InData.setVersion(QDataStream::Qt_4_7);
		qDebug("%i",MessageIn.size());
		if(MessageIn.size() == 120)
				InData >> DataLength;
	InData >> par_ctrl >> no_of_modules >>  post_trigger>> coinc_level >> custom_size >> ov_thr_on >> ext_trig_in >> ext_trig_out >> trace_length >> sample_dec >> risetime >> average_win >> decay_time >> peaking_time >> gap_time >> peak_shift >> thresholdctrl	>> flattop_del >> decimation >> peak_average;
	for(int i = 0;i<8;i++)
		InData >> ena_fadc[i];
	for(int i = 0;i<8;i++)
		InData >> offset[i];
	for(int i = 0;i<8;i++)
		 InData >> threshold[i];
	for(int i = 0;i<8;i++)
		InData >> ov_un_thresh[i];
	for(int i = 0;i<8;i++)
		InData >> ena_trig[i];

}

void MainWindow::ShowParameter()					// put parameters to ui
{
	ui->ledParControlOutput->setText(QString("%1").arg(par_ctrl));
	ui->ledNoOfModulesoutput->setText(QString("%1").arg(no_of_modules));
	ui->ledPostTriggerOutput->setText(QString("%1").arg(post_trigger));
	ui->ledCoincLevelOutput->setText(QString("%1").arg(coinc_level));
	ui->ledPositiveTriggerOutput->setText(QString("%1").arg(ov_thr_on));
	ui->ledExtTriggerInOutput->setText(QString("%1").arg(ext_trig_in));
	ui->ledExtTriggerOutOutput->setText(QString("%1").arg(ext_trig_out));
	ui->ledTracLengthOutput->setText(QString("%1").arg(trace_length));

	ui->ledEnableChannelCh0Output->setText(QString("%1").arg(ena_fadc[0]));
	ui->ledEnableChannelCh1Output->setText(QString("%1").arg(ena_fadc[1]));
	ui->ledEnableChannelCh2Output->setText(QString("%1").arg(ena_fadc[2]));
	ui->ledEnableChannelCh3Output->setText(QString("%1").arg(ena_fadc[3]));
	ui->ledEnableChannelCh4Output->setText(QString("%1").arg(ena_fadc[4]));
	ui->ledEnableChannelCh5Output->setText(QString("%1").arg(ena_fadc[5]));
	ui->ledEnableChannelCh6Output->setText(QString("%1").arg(ena_fadc[6]));
	ui->ledEnableChannelCh7Output->setText(QString("%1").arg(ena_fadc[7]));

	ui->ledOffsetCh0Output->setText(QString("%1").arg(offset[0]));
	ui->ledOffsetCh1Output->setText(QString("%1").arg(offset[1]));
	ui->ledOffsetCh2Output->setText(QString("%1").arg(offset[2]));
	ui->ledOffsetCh3Output->setText(QString("%1").arg(offset[3]));
	ui->ledOffsetCh4Output->setText(QString("%1").arg(offset[4]));
	ui->ledOffsetCh5Output->setText(QString("%1").arg(offset[5]));
	ui->ledOffsetCh6Output->setText(QString("%1").arg(offset[6]));
	ui->ledOffsetCh7Output->setText(QString("%1").arg(offset[7]));

	ui->ledThresholdCh0Output->setText(QString("%1").arg(threshold[0]));
	ui->ledThresholdCh1Output->setText(QString("%1").arg(threshold[1]));
	ui->ledThresholdCh2Output->setText(QString("%1").arg(threshold[2]));
	ui->ledThresholdCh3Output->setText(QString("%1").arg(threshold[3]));
	ui->ledThresholdCh4Output->setText(QString("%1").arg(threshold[4]));
	ui->ledThresholdCh5Output->setText(QString("%1").arg(threshold[5]));
	ui->ledThresholdCh6Output->setText(QString("%1").arg(threshold[6]));
	ui->ledThresholdCh7Output->setText(QString("%1").arg(threshold[7]));

	ui->ledSamplesToTriggerCh0Output->setText(QString("%1").arg(ov_un_thresh[0]));
	ui->ledSamplesToTriggerCh1Output->setText(QString("%1").arg(ov_un_thresh[1]));
	ui->ledSamplesToTriggerCh2Output->setText(QString("%1").arg(ov_un_thresh[2]));
	ui->ledSamplesToTriggerCh3Output->setText(QString("%1").arg(ov_un_thresh[3]));
	ui->ledSamplesToTriggerCh4Output->setText(QString("%1").arg(ov_un_thresh[4]));
	ui->ledSamplesToTriggerCh5Output->setText(QString("%1").arg(ov_un_thresh[5]));
	ui->ledSamplesToTriggerCh6Output->setText(QString("%1").arg(ov_un_thresh[6]));
	ui->ledSamplesToTriggerCh7Output->setText(QString("%1").arg(ov_un_thresh[7]));

	ui->ledEnableTriggerCh0Output->setText(QString("%1").arg(ena_trig[0]));
	ui->ledEnableTriggerCh1Output->setText(QString("%1").arg(ena_trig[1]));
	ui->ledEnableTriggerCh2Output->setText(QString("%1").arg(ena_trig[2]));
	ui->ledEnableTriggerCh3Output->setText(QString("%1").arg(ena_trig[3]));
	ui->ledEnableTriggerCh4Output->setText(QString("%1").arg(ena_trig[4]));
	ui->ledEnableTriggerCh5Output->setText(QString("%1").arg(ena_trig[5]));
	ui->ledEnableTriggerCh6Output->setText(QString("%1").arg(ena_trig[6]));
	ui->ledEnableTriggerCh7Output->setText(QString("%1").arg(ena_trig[7]));


}

void MainWindow::ShowOutput()							// get parameters from server answer and put them to ui
{
	MessageIn = Client->GetDataArray();
	ExtractReceivedData();
	ShowParameter();
}


//server stuff
void MainWindow::EnableServerControls(int state)			// enable/disable server controls in ui
{
	if (state)
	{
		ui->gbServer->setEnabled(true);
		ui->ledIPServer->setEnabled(true);
		ui->ledPortServer->setEnabled(true);
	}
	else
	{
		ui->gbServer->setEnabled(false);
		ui->ledIPServer->setEnabled(false);
		ui->ledPortServer->setEnabled(false);
	}
}

void MainWindow::StartServer()
{
	Server->StartServer();
	ui->btnStartServer->setEnabled(false);
	ui->btnShutdownServer->setEnabled(true);
}

void MainWindow::ShutdownServer()
{
	if(Server)
		Server->ShutdownServer();
	ui->btnStartServer->setEnabled(true);
	ui->btnShutdownServer->setEnabled(false);
}

void MainWindow::SetIPServer()
{
	Server->SetIP(ui->ledIPServer->text());
}

void MainWindow::SetPortServer()
{
	Server->SetPort(ui->ledPortServer->text().toInt());
}
