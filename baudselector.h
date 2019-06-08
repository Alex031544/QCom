#ifndef BAUDSELECTOR_H_INCLUDED
#define BAUDSELECTOR_H_INCLUDED

#include <QComboBox>
#include <QtSerialPort/QSerialPort>

class BaudSelector
      : public QComboBox
{
public:
	BaudSelector();

	QSerialPort::BaudRate
	currentData()
	{
		return (QSerialPort::BaudRate)itemData(currentIndex()).toUInt();
	}
};

#endif // BAUDSELECTOR_H_INCLUDED
