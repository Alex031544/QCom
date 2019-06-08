#include "databitsselector.h"

DataBitsSelector::DataBitsSelector()
{
	addItem("5",QSerialPort::Data5);
	addItem("6",QSerialPort::Data6);
	addItem("7",QSerialPort::Data7);
	addItem("8",QSerialPort::Data8);
}

