#include "caen_hv_control.h"
#include "CAENHVWrapper.h"

CAEN_HV_Control::CAEN_HV_Control(QObject *parent) :
	QObject(parent)
{
    handle = 0;
}

int CAEN_HV_Control::Connect(char* ip,  char* Name_ext , char* Password_ext , int Systemtype , int Linktype )
{

}


/*
//initialize CAENHV
void IP = 192.168.1.3;
QString Username = "admin";
QString Password = "password";
int *handle;
CAENHV_InitSystem( 1, 0, IP, Username.toUtf8().data(), Password.toUtf8().data(), *handle);
*/
