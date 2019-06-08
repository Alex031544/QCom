#include "stopbitselector.h"

StopBitSelector::StopBitSelector()
{
	addItem("1",QSerialPort::OneStop);
	addItem("1.5",QSerialPort::OneAndHalfStop);
	addItem("2",QSerialPort::TwoStop);
}
