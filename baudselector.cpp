#include "baudselector.h"

BaudSelector::BaudSelector()
{
	addItem("  1.200",QSerialPort::Baud1200);
	addItem("  2.400",QSerialPort::Baud2400);
	addItem("  4.800",QSerialPort::Baud4800);
	addItem("  9.600",QSerialPort::Baud9600);
	addItem(" 19.200",QSerialPort::Baud19200);
	addItem(" 38.400",QSerialPort::Baud38400);
	addItem(" 57.600",QSerialPort::Baud57600);
	addItem("115.200",QSerialPort::Baud115200);
}
