#ifndef STOPBITSELECTOR_H
#define STOPBITSELECTOR_H

#include <QComboBox>
#include <QtSerialPort/QSerialPort>

class StopBitSelector
      : public QComboBox
{
public:
	StopBitSelector();

	QSerialPort::StopBits
	currentData()
	{
		return (QSerialPort::StopBits)itemData(currentIndex()).toUInt();
	}
};

#endif // STOPBITSELECTOR_H
