#ifndef HV_CONTROL_WINDOW_H
#define HV_CONTROL_WINDOW_H

#include <QDialog>

namespace Ui {
class HV_Control_window;
}

class HV_Control_window : public QDialog
{
	Q_OBJECT
	
public:
	explicit HV_Control_window(QWidget *parent = 0);
	~HV_Control_window();
	
private:
	Ui::HV_Control_window *ui;
};

#endif // HV_CONTROL_WINDOW_H
