#include "mainwindow.h"

#include <QFileInfo>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
{
	setWindowTitle("Q-Com");
	auto cwidget = new QWidget();
	setCentralWidget(cwidget);
	auto clayout = new QVBoxLayout(cwidget);

	setStyleSheet(" QPushButton {"
	              "    min-width: 100px;"
	              "    max-width: 100px;"
	              " }" );

	auto devPathLabel = new QLabel("Device path:");
	devPathEdit = new QLineEdit();
	devPathConnectBtn = new QPushButton("Connect");
	auto hl01 = new QHBoxLayout();
	clayout->addLayout(hl01);
	hl01->addWidget(devPathLabel);
	hl01->addWidget(devPathEdit);
	hl01->addWidget(devPathConnectBtn);

	devBaudSelector = new BaudSelector();
	auto idxBaud = devBaudSelector->findData(QSerialPort::Baud115200);
	devBaudSelector->setCurrentIndex(idxBaud);
	devDataBitsSelector = new DataBitsSelector();
	auto idxDB = devDataBitsSelector->findData(QSerialPort::Data8);
	devDataBitsSelector->setCurrentIndex(idxDB);
	devParitySelector = new ParitySelector();
	auto idxPar = devParitySelector->findData(QSerialPort::NoParity);
	devParitySelector->setCurrentIndex(idxPar);
	devStopBitSelector = new StopBitSelector();
	auto idxStop = devStopBitSelector->findData(QSerialPort::OneStop);
	devStopBitSelector->setCurrentIndex(idxStop);
	auto hl01a = new QHBoxLayout();
	clayout->addLayout(hl01a);
	hl01a->addWidget(devBaudSelector);
	hl01a->addWidget(devDataBitsSelector);
	hl01a->addWidget(devParitySelector);
	hl01a->addWidget(devStopBitSelector);

	auto line = new QFrame();
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	clayout->addWidget(line);

	auto recLabel = new QLabel("Received data:");
	recDataOutputFmtCBox = new FormatSelector();
	auto recLabelResetBtn = new QPushButton("Reset");
	auto hl02 = new QHBoxLayout();
	clayout->addLayout(hl02);
	hl02->addWidget(recLabel);
	hl02->addWidget(recDataOutputFmtCBox);
	hl02->addWidget(recLabelResetBtn);

	recDataOutput = new QTextEdit();
	recDataOutput->setReadOnly(true);
	clayout->addWidget(recDataOutput);

	auto line2 = new QFrame();
	line2->setFrameShape(QFrame::HLine);
	line2->setFrameShadow(QFrame::Sunken);
	clayout->addWidget(line2);

	auto sndLabel = new QLabel("Data to send:");
	sndDataInputFmtCBox = new FormatSelector();
	sndLabelSendBtn = new QPushButton("Send Data");
	auto hl03 = new QHBoxLayout();
	clayout->addLayout(hl03);
	hl03->addWidget(sndLabel);
	hl03->addWidget(sndDataInputFmtCBox);
	hl03->addWidget(sndLabelSendBtn);

	sndDataInput = new QTextEdit();
	clayout->addWidget(sndDataInput);

	auto line3 = new QFrame();
	line3->setFrameShape(QFrame::HLine);
	line3->setFrameShadow(QFrame::Sunken);
	clayout->addWidget(line3);

	auto msgOutputLabel = new QLabel("Log");
	clayout->addWidget(msgOutputLabel);
	msgOutput = new QTextEdit();
	msgOutput->setReadOnly(true);
	clayout->addWidget(msgOutput);

	connect( devPathConnectBtn,
	         &QPushButton::clicked,
	         this,
	         &MainWindow::handle_devPathConnectBtn_clicked );

	connect( this,
	         &MainWindow::signal_serialPortConnectionStatus_changed,
	         this,
	         &MainWindow::handle_serialPortConnectionStatus_changed );

	connect( recLabelResetBtn,
	         &QPushButton::clicked,
	         this,
	         &MainWindow::handle_recLabelResetBtn_clicked );

	connect( recDataOutputFmtCBox,
	         SIGNAL(currentIndexChanged(int)),
	         this,
	         SLOT(handle_recDataOutputFmtCBox_changed(int)) );
}

MainWindow::~MainWindow()
{
	try{
		serialPort->close();
	} catch (...) {
		return;
	}
}

void
MainWindow::handle_devPathConnectBtn_clicked()
{
	switch( serialPortConnectionStatus ){
	case connected:
		serialPort->close();

		disconnect( serialPort,
		            &QSerialPort::readyRead,
		            this,
		            &MainWindow::handle_serialPort_readyRead );

		disconnect( sndLabelSendBtn,
		            &QPushButton::clicked,
		            this,
		            &MainWindow::handle_sndLabelSendBtn_clicked );

		serialPortConnectionStatus = disconnected;
		emit signal_serialPortConnectionStatus_changed();
		break;
	case disconnected:
		if( devPathEdit->text().length()==0 ){
			sendMsg("no path given");
			break;
		}
//		QFileInfo check_file(devPathEdit->text());
//		if( !(check_file.exists()) ) {
//			sendMsg("given file does not exist");
//			break;
//		}

		sendMsg("going to connect device");
		serialPort = NULL;
		sendMsg("open " + devPathEdit->text());
		serialPort = new QSerialPort();
		if( serialPort==NULL ){
			break;
		}
		serialPort->setPortName(devPathEdit->text());
		serialPort->setBaudRate(devBaudSelector->currentData());
		serialPort->setDataBits(devDataBitsSelector->currentData());
		serialPort->setParity(devParitySelector->currentData());
		serialPort->setStopBits(devStopBitSelector->currentData());
		if( false == serialPort->open(QIODevice::ReadWrite) ){
			sendMsg( "failed to open device"
			         + serialPort->errorString() );
			break;
		}
		serialPort->flush();
		connect( serialPort,
		         &QSerialPort::readyRead,
		         this,
		         &MainWindow::handle_serialPort_readyRead );

		connect( sndLabelSendBtn,
		         &QPushButton::clicked,
		         this,
		         &MainWindow::handle_sndLabelSendBtn_clicked );

		serialPortConnectionStatus = connected;
		emit signal_serialPortConnectionStatus_changed();
		break;
	}
}

void
MainWindow::handle_serialPortConnectionStatus_changed()
{
	if( serialPortConnectionStatus==connected ){
		devPathConnectBtn->setText("Disconnect");
	} else {
		devPathConnectBtn->setText("Connect");
	}
}

void
MainWindow::handle_serialPort_readyRead()
{
	auto data = serialPort->readAll();

	QString str;
	uint8_t ui8val;

	switch (recDataOutputFmtCBox->currentIndex()) {
	case FormatSelector::HEX:
		for( auto _data : data ){
			ui8val = (uint8_t) _data;
			str += QString::number(ui8val,16) + " ";
		}
		break;
	case FormatSelector::DEC:
		for( auto _data : data ){
			ui8val = (uint8_t) _data;
			str += QString::number(ui8val) + " ";
		}
		break;
	default:
		sendMsg("no valid format set");
		return;
	}

	recDataOutput->insertPlainText(str);
}

void
MainWindow::handle_recLabelResetBtn_clicked()
{
	recDataOutput->clear();
	handle_serialPort_readyRead();
}

void
MainWindow::handle_sndLabelSendBtn_clicked()
{
	// store byte array to send via serial port
	QByteArray ba;

	QRegExp rx("[ ]"); // match a space character
	QString str;
	QStringList strList;

	switch( sndDataInputFmtCBox->currentIndex() ){
	case FormatSelector::HEX:
		str = sndDataInput->toPlainText();
		strList = str.split(rx, QString::SkipEmptyParts);

		for( auto strListElem: strList ){
			if( strListElem.length()>2){
				sendMsg("invalid hex value");
			}
			ba.append(strListElem.toUInt(Q_NULLPTR,16));
		}
		break;
	case FormatSelector::DEC:
		str = sndDataInput->toPlainText();
		strList = str.split(rx, QString::SkipEmptyParts);

		for( auto strListElem: strList ){
			ba.append(strListElem.toUInt());
		}
		break;
	default:
		sendMsg("no valid format set");
		return;
	}

	serialPort->write(ba);
}

void
MainWindow::handle_recDataOutputFmtCBox_changed(int index)
{
	QByteArray ba;

	QRegExp rx("[ ]"); // match a space character
	QString str;
	QStringList strList;
	uint8_t ui8val;

	// read in
	switch( recDataOutputFmtCBoxOld ){
	case FormatSelector::HEX:
		str = recDataOutput->toPlainText();
		strList = str.split(rx, QString::SkipEmptyParts);

		for( auto strListElem: strList ){
			if( strListElem.length()>2){
				sendMsg("invalid hex value");
			}
			ba.append(strListElem.toUInt(Q_NULLPTR,16));
		}
		break;
	case FormatSelector::DEC:
		str = recDataOutput->toPlainText();
		strList = str.split(rx, QString::SkipEmptyParts);

		for( auto strListElem: strList ){
			ba.append(strListElem.toUInt());
		}
		break;
	default:
		sendMsg("no valid format set");
		return;
	}

	str.clear();
	recDataOutput->clear();

	// and print with new format
	switch (index) {
	case FormatSelector::HEX:
		for( auto _data : ba ){
			ui8val = (uint8_t) _data;
			str += QString::number(ui8val,16) + " ";
		}
		break;
	case FormatSelector::DEC:
		for( auto _data : ba ){
			ui8val = (uint8_t) _data;
			str += QString::number(ui8val) + " ";
		}
		break;
	default:
		sendMsg("no valid format set");
		return;
	}

	recDataOutput->insertPlainText(str);
	recDataOutputFmtCBoxOld = index;
}

void
MainWindow::sendMsg(QString text)
{
	msgOutput->append(text);
}
