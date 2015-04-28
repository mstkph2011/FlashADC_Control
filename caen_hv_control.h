#ifndef CAEN_HV_CONTROL_H
#define CAEN_HV_CONTROL_H

#include <QObject>



class CAEN_HV_Control : public QObject
{
	Q_OBJECT
public:
	explicit CAEN_HV_Control(QObject *parent = 0);
	
signals:
	
private:
    int handle;
public slots:

    int             Connect(char* ip,  char* Name_ext = "panda", char* Password_ext = "panda", int Systemtype = 1, int Linktype = 0);
	
};

#endif // CAEN_HV_CONTROL_H
