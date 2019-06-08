#include "parityselector.h"

ParitySelector::ParitySelector()
{
	addItem("No",QSerialPort::NoParity);
	addItem("Even",QSerialPort::EvenParity);
	addItem("Odd",QSerialPort::OddParity);
	addItem("Space",QSerialPort::SpaceParity);
	addItem("Mark",QSerialPort::MarkParity);
}
