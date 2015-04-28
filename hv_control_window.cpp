#include "hv_control_window.h"
#include "ui_hv_control_window.h"
#include "CAENHVWrapper.h"
#include <iostream>
#include <cstring>
#include "QMessageBox"
#include <QDateTime>

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

#include"Sleeper.h"

/* error codes
 CAENHV_OK                   0
 CAENHV_SYSERR               1
 CAENHV_WRITEERR             2
 CAENHV_READERR              3
 CAENHV_TIMEERR              4
 CAENHV_DOWN                 5
 CAENHV_NOTPRES              6
 CAENHV_SLOTNOTPRES          7
 CAENHV_NOSERIAL             8
 CAENHV_MEMORYFAULT          9
 CAENHV_OUTOFRANGE           10
 CAENHV_EXECCOMNOTIMPL       11
 CAENHV_GETPROPNOTIMPL       12
 CAENHV_SETPROPNOTIMPL       13
 CAENHV_PROPNOTFOUND         14
 CAENHV_EXECNOTFOUND         15
 CAENHV_NOTSYSPROP		        16
 CAENHV_NOTGETPROP		        17
 CAENHV_NOTSETPROP           18
 CAENHV_NOTEXECOMM           19
 CAENHV_SYSCONFCHANGE	      20
 CAENHV_PARAMPROPNOTFOUND    21
 CAENHV_PARAMNOTFOUND        22
 CAENHV_CONNECTED	         (0x1000 + 1)
 CAENHV_NOTCONNECTED	       (0x1000 + 2)
 CAENHV_OS    	             (0x1000 + 3)
 CAENHV_LOGINFAILED         (0x1000 + 4)
 CAENHV_LOGOUTFAILED        (0x1000 + 5)
 CAENHV_LINKNOTSUPPORTED    (0x1000 + 6)  // Rel. 1.2
*/

using namespace std;
HV_Control_window::HV_Control_window(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::HV_Control_window)
{

    ui->setupUi(this);
    MonitorTimer = new QTimer(this);
    connect(ui->cbEnableSetup,SIGNAL(stateChanged(int)), this, SLOT(EnableSetupControls(int)));
    connect(ui->butConnectHV,SIGNAL(clicked()), this, SLOT(Connect()));
    connect(ui->butDisconnectHV,SIGNAL(clicked()), this, SLOT(Disconnect()));
    connect(ui->butSetNewHVGe,SIGNAL(clicked()), this, SLOT(SetTargetVoltageGe()));
    connect(ui->butSetHVGeOn,SIGNAL(clicked()), this, SLOT(EnableHVGe()));
    connect(ui->butSetHVGeOff,SIGNAL(clicked()), this, SLOT(DisableHVGe()));
    connect(ui->butSetNewHVNaI,SIGNAL(clicked()), this, SLOT(SetTargetVoltageNaI()));
    connect(ui->butSetHVNaIOff,SIGNAL(clicked()), this, SLOT(DisableHVNaI()));
    connect(ui->butSetHVNaIOn,SIGNAL(clicked()), this, SLOT(EnableHVNaI()));
    connect(ui->butStartLog,SIGNAL(clicked()), this, SLOT(StartMonitoring()));
    connect(ui->butStopLog,SIGNAL(clicked()), this, SLOT(StopMonitoring()));
    //connect(ui->HV_Control_window,SIGNAL(closed()), this, SLOT(StopMonitoring()));
    connect(this, SIGNAL(connected()),this,SLOT(ReadSettings()));
    connect(MonitorTimer, SIGNAL(timeout()),this, SLOT(MonitoringLoop()));

    handle = 0;
    isConnected = 0;
    isHVGeEnabled = -1;
    isHVNaIEnabled = -1;
    isMonitoring =0;


    ui->qwtPlotHV->setTitle("Ge HV");
}

HV_Control_window::~HV_Control_window()
{
    MonitorTimer->stop();
    LogFile->close();
    delete LogFile;
	delete ui;
}

void HV_Control_window::EnableSetupControls(int state)			// enable/disable server controls in ui
{
        ui->gbSetup->setEnabled(state);

}

int HV_Control_window::Connect()
{
    int status = -1;
    string strbuf = ui->ledHVIP->text().toStdString();
    char *buf = new char[strbuf.length()+1];
    strcpy(buf,strbuf.c_str());
    if (!isConnected)
    {
        status =  CAENHV_InitSystem(CAENHV_SYSTEM_TYPE_t (1),0,buf,"panda","panda",&handle);
        if (status == 0)
        {
             isConnected = 1;
             cout << "connected" << endl;
             ui->butConnectHV->setEnabled(false);
             ui->butDisconnectHV->setEnabled(true);
             connected();
        }
    }

    delete[] buf;
    return status;
}

int HV_Control_window::Disconnect()
{
    int status = -1;
    //CAENHV_DeinitSystem(handle);
    if (isConnected)
    {
     status = CAENHV_DeinitSystem(handle);
        if (status == 0)
        {
            isConnected = 0;
            cout << "disconnected"<< endl;
            ui->butConnectHV->setEnabled(true);
            ui->butDisconnectHV->setEnabled(false);
        }
    }
    return status;
}


int HV_Control_window::SlotGe()
{
    fSlotGe = ui->ledHVGeSlot->text().toInt();
    return fSlotGe;
}

int HV_Control_window::ChannelGe()
{
    fChannelGe = ui->ledHVGeChannel->text().toInt();
    return fChannelGe;
}
int HV_Control_window::SlotNaI()
{
    fSlotNaI = ui->ledHVNaISlot->text().toInt();
    return fSlotNaI;
}

int HV_Control_window::ChannelNaI()
{
    fChannelNaI = ui->ledHVNaIChannel->text().toInt();
    return fChannelNaI;
}
int HV_Control_window::SetTargetVoltageGe()
{
    if (!isConnected)
    {
        ShowNotConnectedError();
        return -1;
    }
    const short unsigned int chlist[1]={ChannelGe()};
    float V0 = ui->ledHVGeTarget->text().toInt();
    int status = CAENHV_SetChParam(handle, SlotGe(),"V0Set",1,chlist,&V0);
    if (status == 0)
    {
        Sleeper::msleep(100);
        float V0measured = -1;
        status = CAENHV_GetChParam(handle, SlotGe(),"V0Set",1,chlist,&V0measured);
        if (status == 0)
            ui->ledHVGeTargetMeasured->setText(QString("%1").arg(V0measured));
    }
    return status;
}

int HV_Control_window::MeasureValuesGe()
{
    int status = -1;
    const short unsigned int chlist[1]={ChannelGe()};
    VMonGe = -1;
    status = CAENHV_GetChParam(handle, SlotGe(),"VMon",1,chlist,&VMonGe);
    if (status == 0)
    {
        ui->ledHVGeMeasured->setText(QString("%1").arg(VMonGe));
        IMonGe = -1;
        status = CAENHV_GetChParam(handle, SlotGe(),"IMon",1,chlist,&IMonGe);
        if (status == 0)
            ui->ledHVGeMeasuredCurrent->setText(QString("%1").arg(IMonGe));
        //cout << "bla" << endl;

    }
    return status;
}

int HV_Control_window::EnableHVGe()
{

    if (!isConnected)
    {
         ShowNotConnectedError();
         return -1;
    }
    const short unsigned int chlist[1]={ChannelGe()};

    unsigned EnableHV = 1;
    int status = CAENHV_SetChParam(handle, SlotGe(),"Pw",1,chlist,&EnableHV);
    Sleeper::msleep(100);
    CAENHV_GetChParam(handle, SlotGe(),"Pw",1,chlist,&isHVGeEnabled);
    //cout << isHVGeEnabled<<endl;
    if (isHVGeEnabled == 1)
    {
        ui->butSetHVGeOn->setEnabled(0);
        ui->butSetHVGeOff->setEnabled(1);
        ui->labHVGeStatus->setText("Enabled");
        ui->labHVGeStatus->setStyleSheet("QLabel{ color : green; font: bold 9pt \"Sans Serif\"; }");
    }
    return status;
}

int HV_Control_window::DisableHVGe()
{
    if (!isConnected)
    {
         ShowNotConnectedError();
         return -1;
    }
    const short unsigned int chlist[1]={ChannelGe()};

    unsigned EnableHV = 0;
    int status = CAENHV_SetChParam(handle, SlotGe(),"Pw",1,chlist,&EnableHV);
    Sleeper::msleep(100);
    CAENHV_GetChParam(handle, SlotGe(),"Pw",1,chlist,&isHVGeEnabled);
    //cout << isHVGeEnabled<<endl;
    if (isHVGeEnabled == 0)
    {
        ui->butSetHVGeOn->setEnabled(1);
        ui->butSetHVGeOff->setEnabled(0);
        ui->labHVGeStatus->setText("Disabled");
        ui->labHVGeStatus->setStyleSheet("QLabel { color : red; font: bold 9pt \"Sans Serif\"; }");
    }
    return status;
}

int HV_Control_window::SetTargetVoltageNaI()
{
    if (!isConnected)
    {
        ShowNotConnectedError();
        return -1;
    }
    const short unsigned int chlist[1]={ChannelNaI()};
    float V0 = ui->ledHVNaITarget->text().toInt();
    int status = CAENHV_SetChParam(handle, SlotNaI(),"V0Set",1,chlist,&V0);
    if (status == 0)
    {
        Sleeper::msleep(100);
        float V0measured = -1;
        status = CAENHV_GetChParam(handle, SlotNaI(),"V0Set",1,chlist,&V0measured);
        if (status == 0)
            ui->ledHVNaITargetMeasured->setText(QString("%1").arg(V0measured));
    }
    return status;
}

int HV_Control_window::MeasureValuesNaI()
{
    int status = -1;
    const short unsigned int chlist[1]={ChannelNaI()};
    VMonNaI = -1;
    status = CAENHV_GetChParam(handle, SlotNaI(),"VMon",1,chlist,&VMonNaI);
    if (status == 0)
    {
        ui->ledHVNaIMeasured->setText(QString("%1").arg(VMonNaI));
        IMonNaI = -1;
        status = CAENHV_GetChParam(handle, SlotNaI(),"IMon",1,chlist,&IMonNaI);
        if (status == 0)
            ui->ledHVNaIMeasuredCurrent->setText(QString("%1").arg(IMonNaI));
        //cout << "bla" << endl;

    }
    return status;
}

int HV_Control_window::EnableHVNaI()
{
    if (!isConnected)
    {
         ShowNotConnectedError();
         return -1;
    }
    const short unsigned int chlist[1]={ChannelNaI()};

    unsigned EnableHV = 1;
    int status = CAENHV_SetChParam(handle, SlotNaI(),"Pw",1,chlist,&EnableHV);
    Sleeper::msleep(100);
    CAENHV_GetChParam(handle, SlotNaI(),"Pw",1,chlist,&isHVNaIEnabled);
    //cout << isHVNaIEnabled<<endl;
    if (isHVNaIEnabled == 1)
    {
        ui->butSetHVNaIOn->setEnabled(0);
        ui->butSetHVNaIOff->setEnabled(1);
        ui->labHVNaIStatus->setText("Enabled");
        ui->labHVNaIStatus->setStyleSheet("QLabel{ color : green; font: bold 9pt \"Sans Serif\"; }");
    }
    return status;
}

int HV_Control_window::DisableHVNaI()
{
    if (!isConnected)
    {
         ShowNotConnectedError();
         return -1;
    }
    const short unsigned int chlist[1]={ChannelNaI()};

    unsigned EnableHV = 0;
    int status = CAENHV_SetChParam(handle, SlotNaI(),"Pw",1,chlist,&EnableHV);
    Sleeper::msleep(100);
    CAENHV_GetChParam(handle, SlotNaI(),"Pw",1,chlist,&isHVNaIEnabled);
    //cout << isHVNaIEnabled<<endl;
    if (isHVNaIEnabled == 0)
    {
        ui->butSetHVNaIOn->setEnabled(1);
        ui->butSetHVNaIOff->setEnabled(0);
        ui->labHVNaIStatus->setText("Disabled");
        ui->labHVNaIStatus->setStyleSheet("QLabel { color : red; font: bold 9pt \"Sans Serif\"; }");
    }
    return status;
}

void HV_Control_window::ShowNotConnectedError()
{
    QMessageBox::warning(0, QString("Not Connected!"), QString("Not Connected!"));
}

void HV_Control_window::ReadSettings()
{
    int status = -1;
    //germanium stuff
    const short unsigned int chlist[1]={ChannelGe()};
    float V0measured = -1;
    //read target HV value of Ge
    status = CAENHV_GetChParam(handle, SlotGe(),"V0Set",1,chlist,&V0measured);
    if (status == 0)
        ui->ledHVGeTargetMeasured->setText(QString("%1").arg(V0measured));
   //read measurement values of Ge
    MeasureValuesGe();
    //check status of Ge HV on or off
    CAENHV_GetChParam(handle, SlotGe(),"Pw",1,chlist,&isHVGeEnabled);
    if (isHVGeEnabled)
    {
        ui->labHVGeStatus->setText("Enabled");
        ui->labHVGeStatus->setStyleSheet("QLabel { color : green; font: bold 9pt \"Sans Serif\"; }");
    }
    else
    {
        ui->labHVGeStatus->setText("Disabled");
        ui->labHVGeStatus->setStyleSheet("QLabel { color : red; font: bold 9pt \"Sans Serif\"; }");
    }

    //NaI stuff
    const short unsigned int chlistNaI[1]={ChannelNaI()};
    V0measured = -1;
    //read target value of NaI
    status = CAENHV_GetChParam(handle, SlotNaI(),"V0Set",1,chlistNaI,&V0measured);
    if (status == 0)
        ui->ledHVNaITargetMeasured->setText(QString("%1").arg(V0measured));
    //read measurement values of NaI
    MeasureValuesNaI();
    //check status of NaI HV on or off
    CAENHV_GetChParam(handle, SlotNaI(),"Pw",1,chlistNaI,&isHVNaIEnabled);
    if (isHVNaIEnabled)
    {
        ui->labHVNaIStatus->setText("Enabled");
        ui->labHVNaIStatus->setStyleSheet("QLabel { color : green; font: bold 9pt \"Sans Serif\"; }");
    }
    else
    {
        ui->labHVNaIStatus->setText("Disabled");
        ui->labHVNaIStatus->setStyleSheet("QLabel { color : red; font: bold 9pt \"Sans Serif\"; }");
    }

}

void HV_Control_window::MonitoringLoop()
{
    MeasureValuesGe();
    MeasureValuesNaI();
    QDateTime TimeStamp(QDateTime::currentDateTime());
    cout << TimeStamp.toTime_t()<<endl;

    *OutStream << TimeStamp.toTime_t() << "\t"<< VMonGe << "\t" << IMonGe << "\t"<< VMonNaI << "\t" << IMonNaI << "\n";
}
void HV_Control_window::StartMonitoring()
{
    if (isConnected)
    {
        LogFile = new QFile("text.txt");
        OutStream = new QTextStream(LogFile);
        LogFile->open(QIODevice::WriteOnly | QIODevice::Append);
        //cout <<ui->spBLoggingTime->text().toInt()<<endl;
        MonitorTimer->start(ui->spBLoggingTime->text().toInt()*1000);
        ui->butStartLog->setEnabled(false);
        ui->butStopLog->setEnabled(true);
        isMonitoring=1;
    }
    else
        ShowNotConnectedError();
}

void HV_Control_window::StopMonitoring()
{
    if(isMonitoring)
    {
        MonitorTimer->stop();
        LogFile->close();
        delete OutStream;
        delete LogFile;
        isMonitoring=0;
    }
    ui->butStartLog->setEnabled(true);
    ui->butStopLog->setEnabled(false);
}
void HV_Control_window::closeEvent(QCloseEvent * e)
{
   StopMonitoring();
}
