#ifndef DATABITSSELECTOR_H_INCLUDED
#define DATABITSSELECTOR_H_INCLUDED

#include <QComboBox>
#include <QtSerialPort/QSerialPort>

class DataBitsSelector
      : public QComboBox
{
public:
	DataBitsSelector();

	QSerialPort::DataBits
	currentData()
	{
		return (QSerialPort::DataBits)itemData(currentIndex()).toUInt();
	}
};

#endif // DATABITSSELECTOR_H_INCLUDED
