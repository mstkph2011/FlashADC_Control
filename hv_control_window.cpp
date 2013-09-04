#include "hv_control_window.h"
#include "ui_hv_control_window.h"

HV_Control_window::HV_Control_window(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::HV_Control_window)
{
	ui->setupUi(this);
}

HV_Control_window::~HV_Control_window()
{
	delete ui;
}
