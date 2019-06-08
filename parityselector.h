#ifndef PARITYSELECTOR_H_INCLUDED
#define PARITYSELECTOR_H_INCLUDED

#include <QComboBox>
#include <QtSerialPort/QSerialPort>

class ParitySelector
      : public QComboBox
{
public:
	ParitySelector();

	QSerialPort::Parity
	currentData()
	{
		return (QSerialPort::Parity)itemData(currentIndex()).toUInt();
	}

};

#endif // PARITYSELECTOR_H_INCLUDED
