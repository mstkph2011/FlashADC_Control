#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ServerWindow)
{
	ui->setupUi(this);
}

ServerWindow::~ServerWindow()
{
	delete ui;
}
