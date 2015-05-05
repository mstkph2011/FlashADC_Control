#ifndef HV_CONTROL_WINDOW_H
#define HV_CONTROL_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

namespace Ui {
class HV_Control_window;
}

class HV_Control_window : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit HV_Control_window(QWidget *parent = 0);
	~HV_Control_window();
	
private:
	Ui::HV_Control_window *ui;
    void closeEvent(QCloseEvent * e);

    int                 handle;
    bool                isConnected;
    int                 fChannelGe;
    int                 fSlotGe;
    int                 fChannelNaI;
    int                 fSlotNaI;

    unsigned            isHVGeEnabled;
    unsigned            isHVNaIEnabled;

    float               VMonGe;
    float               IMonGe;
    float               VMonNaI;
    float               IMonNaI;
    QFile               *LogFile;
    QTextStream         *OutStream;


    QTimer              *MonitorTimer;
    bool                isMonitoring;
    int                 MonitorCounter;


    QwtPlotCurve        *MonitorCurve;
    QVector<double>     VectorX;
    QVector<double>     VectorY;

private slots:

    void                    EnableSetupControls(int state);
    int                     Connect();
    int                     Disconnect();
    int                     SlotGe();
    int                     ChannelGe();
    int                     SlotNaI();
    int                     ChannelNaI();

    int                     SetTargetVoltageGe();
    int                     MeasureValuesGe();
    int                     EnableHVGe();
    int                     DisableHVGe();

    int                     SetTargetVoltageNaI();
    int                     MeasureValuesNaI();
    int                     EnableHVNaI();
    int                     DisableHVNaI();

    void                    ReadSettings();
    void                    ShowNotConnectedError();

    void                    MonitoringLoop();
    void                    StartMonitoring();
    void                    StopMonitoring();

signals:
    void                    connected();

};

#endif // HV_CONTROL_WINDOW_H
