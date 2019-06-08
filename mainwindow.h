#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QtSerialPort/QSerialPort>

#include "formatselector.h"
#include "baudselector.h"
#include "databitsselector.h"
#include "parityselector.h"
#include "stopbitselector.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	QTextEdit *msgOutput;
	QTextEdit *recDataOutput;
	QPushButton *devPathConnectBtn;
	QPushButton *sndLabelSendBtn;
	QLineEdit *devPathEdit;
	QSerialPort *serialPort;
	QTextEdit *sndDataInput;
	FormatSelector *sndDataInputFmtCBox;
	FormatSelector *recDataOutputFmtCBox;
	BaudSelector *devBaudSelector;
	DataBitsSelector *devDataBitsSelector;
	ParitySelector *devParitySelector;
	StopBitSelector *devStopBitSelector;

	enum {connected,disconnected} serialPortConnectionStatus = disconnected;

	quint8 recDataOutputFmtCBoxOld = FormatSelector::HEX;

private:
	void sendMsg( QString text );

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:
	void signal_serialPortConnectionStatus_changed();

public slots:
	void handle_devPathConnectBtn_clicked();
	void handle_serialPortConnectionStatus_changed();
	void handle_serialPort_readyRead();
	void handle_recLabelResetBtn_clicked();
	void handle_sndLabelSendBtn_clicked();
	void handle_recDataOutputFmtCBox_changed(int index);
};

#endif // MAINWINDOW_H
