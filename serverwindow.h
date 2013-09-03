#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QDialog>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QDialog
{
	Q_OBJECT
	
public:
	explicit ServerWindow(QWidget *parent = 0);
	~ServerWindow();
	
private:
	Ui::ServerWindow *ui;
};

#endif // SERVERWINDOW_H
