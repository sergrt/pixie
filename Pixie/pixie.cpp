#include "stdafx.h"
#include "pixie.h"
#include "helpers.h"
#include "GDISimpleThread.h"
#include "DXSimpleThread.h"

/*** define USE_LAB to use LAB colors - NOT RECOMMENDED ***/
// #define USE_LAB
#ifdef USE_LAB
	#include "lab.h"
#endif

Pixie::Pixie(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags) {
	ui.setupUi(this);
	
	// Set up comboboxes
	for ( int i = 1; i <= 20; i++ )
		ui.cbPort->addItem( QString( "COM%1" ).arg( i ), i );

	ui.cbBaud->addItem( "9600", BAUD9600 );
	ui.cbBaud->addItem( "57600", BAUD57600 );
	ui.cbBaud->addItem( "115200", BAUD115200 );

	ui.cbLanguage->addItem( "Russian (RU)", CSettings::RUSSIAN_RU );
	ui.cbLanguage->addItem( "English (US)", CSettings::ENGLISH_US );

	// Initialize tray icon and menu
	trayIcon = new QSystemTrayIcon( QIcon( ":/Pixie/Resources/Pixie16.png" ), this );
	QObject::connect( trayIcon, SIGNAL( activated ( QSystemTrayIcon::ActivationReason ) ), this, SLOT( showHide( QSystemTrayIcon::ActivationReason ) ) );
	QAction *quit_action = new QAction( tr( "Выход" ), trayIcon );
	QObject::connect( quit_action, SIGNAL( triggered() ), this, SLOT( onCloseClick() ) );

	QMenu *trayIconMenu = new QMenu;
	trayIconMenu->addAction( quit_action );
	trayIcon->setContextMenu( trayIconMenu );
	trayIcon->show();

	// Set translation
	translator = new QTranslator();
	onLangChanged();

	QObject::connect( ui.bnStartStop, SIGNAL( clicked() ), this, SLOT( onStartStopClick() ) );
	QObject::connect( ui.bnPreviewEnabled, SIGNAL( clicked() ), this, SLOT( onPreviewClick() ) );
	QObject::connect( ui.sGamma, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.sR, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.sG, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.sB, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.sThreshold, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.sLimitter, SIGNAL( valueChanged( int ) ), this, SLOT( onSliderChanged() ) );
	QObject::connect( ui.cbLanguage, SIGNAL( currentIndexChanged( int ) ), this, SLOT( onLangChanged() ) );

	ui.framePreview->installEventFilter( this );

	stopThread = true;
	readyToProcess = true;	
	captureThread = NULL;
	port = NULL;

	// load settings
	settings.loadSettings();	
	applySettingsToUI();

	setThreadIsRunning( false );
	displayRefreshRate( 0 );
	if ( !settings.startMinimized )
		show();

	if ( settings.autoStart )
		onStartStopClick();	
}

Pixie::~Pixie() {
	// Stop capture thread if is running
	stopThread = false;
	onStartStopClick();

	// Get settings from UI and save them
	applySettingsFromUI();
	settings.saveSettings();

	// Cleanup some resources
	delete trayIcon->contextMenu();
	trayIcon->setVisible( false );
	delete trayIcon;
	qApp->removeTranslator( translator );
	delete translator;
}
void Pixie::onCloseClick() {
	close();
}
void Pixie::applySettingsFromUI() {
	settings.LEDnumH = ui.leNHoriz->text().toUInt();
	settings.LEDnumV = ui.leNVert->text().toUInt();
	settings.horizontalHeightP = ui.leHorizontalHeightP->text().toUInt();
	settings.verticalWidthP = ui.leVerticalWidthP->text().toUInt();

	if ( ui.bnGDI->isChecked() )
		settings.captureEngine = CSettings::CE_GDI;
	else
		settings.captureEngine = CSettings::CE_DX;

	settings.desiredFramerate = ui.leFrameRate->text().toUInt();
	settings.autoStart = ui.bnAutoStart->isChecked();
	settings.startMinimized = ui.bnStartMinimized->isChecked();
	settings.previewEnabled = ui.bnPreviewEnabled->isChecked();
	settings.portNumber = ui.cbPort->itemData( ui.cbPort->currentIndex() ).toUInt();
	settings.baudRate = static_cast< BaudRateType >( ui.cbBaud->itemData( ui.cbBaud->currentIndex() ).toUInt() );
	settings.language = static_cast< CSettings::LANG >( ui.cbLanguage->itemData( ui.cbLanguage->currentIndex() ).toUInt() );

	settings.gamma = ui.sGamma->value();
	settings.redColor = ui.sR->value();
	settings.greenColor = ui.sG->value();
	settings.blueColor = ui.sB->value();
	settings.useColorControl = ui.groupColorControl->isChecked();
	settings.threshold = ui.sThreshold->value();
	settings.limitter = ui.sLimitter->value();
	settings.useCompression = ui.groupCompression->isChecked();
}
void Pixie::applySettingsToUI() {
	ui.leNHoriz->setText( QString( "%1" ).arg( settings.LEDnumH ) );
	ui.leNVert->setText( QString( "%1" ).arg( settings.LEDnumV ) );
	ui.leHorizontalHeightP->setText( QString( "%1" ).arg( settings.horizontalHeightP ) );
	ui.leVerticalWidthP->setText( QString( "%1" ).arg( settings.verticalWidthP ) );

	ui.bnGDI->setChecked( settings.captureEngine == CSettings::CE_GDI );
	ui.bnDirectX->setChecked( settings.captureEngine != CSettings::CE_GDI );

	ui.leFrameRate->setText( QString( "%1" ).arg( settings.desiredFramerate ) );
	ui.bnAutoStart->setChecked( settings.autoStart );
	ui.bnStartMinimized->setChecked( settings.startMinimized );
	ui.bnPreviewEnabled->setChecked( settings.previewEnabled );

	selectComboBoxItemByData( ui.cbPort, settings.portNumber );
	selectComboBoxItemByData( ui.cbBaud, settings.baudRate );
	selectComboBoxItemByData( ui.cbLanguage, settings.language );

	ui.sGamma->setValue( settings.gamma );
	ui.sR->setValue( settings.redColor );
	ui.sG->setValue( settings.greenColor );
	ui.sB->setValue( settings.blueColor );
	ui.groupColorControl->setChecked( settings.useColorControl );
	ui.sThreshold->setValue( settings.threshold );
	ui.sLimitter->setValue( settings.limitter );
	ui.groupCompression->setChecked( settings.useCompression );
}
void Pixie::onStartStopClick() {
	stopThread = !stopThread;

	if ( stopThread == true ) {
		if ( captureThread ) {
			while( captureThread->isRunning() ) {
				this->thread()->usleep( 100 * 1000 );
			}
		}
		delete captureThread;
		captureThread = NULL;

		if ( port ) {
			if ( port->isOpen() ) {
				port->close();
			}
			delete port;
			port = NULL;
		}
	} else {
		if ( captureThread == NULL ) {
			applySettingsFromUI();
			initPort();
			if ( port )
				port->open( QIODevice::ReadWrite );

			if ( ui.bnGDI->isChecked() )
				captureThread = new CGDISimpleThread( this, &stopThread, &settings, &readyToProcess );
			else
				captureThread = new CDXSimpleThread( this, &stopThread, &settings, &readyToProcess );

			captureThread->start();
		}
	}	
}
void Pixie::setThreadIsRunning( bool r ) {	
	if ( r ) 
		ui.bnStartStop->setText( tr( "Остановить" ) );
	else
		ui.bnStartStop->setText( tr( "Запустить" ) );
	
	ui.groupCapture->setEnabled( !r );
	ui.groupRegions->setEnabled( !r );
	ui.groupComm->setEnabled( !r );
}
QColor Pixie::getAverageColor( const unsigned char * const buf, const unsigned short width, const unsigned short height ) const {
#ifdef USE_LAB	// use LAB mean values
	CLAB * labColors = new CLAB[ width * height ];
	int li = 0;
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			unsigned char R = buf[ y * width * 4 + x * 4 + 0 ];
			unsigned char G = buf[ y * width * 4 + x * 4 + 1 ];
			unsigned char B = buf[ y * width * 4 + x * 4 + 2 ];
			labColors[ li++ ] = CLAB( R, G, B );
		}
	}

	double L_avg = labColors[ 1 ].mL;
	double A_avg = labColors[ 1 ].mA;
	double B_avg = labColors[ 1 ].mB;

	for ( int i = 1; i < li; i++ )
	{
		L_avg += labColors[ i ].mL;
		A_avg += labColors[ i ].mA;
		B_avg += labColors[ i ].mB;
	}
	
	L_avg = L_avg / ( width * height );
	A_avg = A_avg / ( width * height );
	B_avg = B_avg / ( width * height );

	CLAB res;
	res.mL = L_avg;
	res.mA = A_avg;
	res.mB = B_avg;

	QColor resRGB = res.getRGB();

	delete [] labColors;
#else	// use simple RGB mean values
	double B_avg = buf[ 0 ];
	double G_avg = buf[ 1 ];
	double R_avg = buf[ 2 ];

	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			int pos = y * width * 4 + x * 4;
			B_avg += buf[ pos + 0 ];
			G_avg += buf[ pos + 1 ];
			R_avg += buf[ pos + 2 ];
		}
	}
	R_avg = R_avg / ( height * width );
	G_avg = G_avg / ( height * width );
	B_avg = B_avg / ( height * width );
	QColor resRGB( R_avg, G_avg, B_avg );
#endif
	return resRGB;
}
bool Pixie::eventFilter( QObject* watched, QEvent* event ) {
	bool res = false;

	if ( watched == ui.framePreview && event->type() == QEvent::Paint && settings.previewEnabled ) {
		QPainter painter( ui.framePreview );
		const int rectH = 20;

		for ( int i = 0; i < colorHTop.count(); i++ ) {
			QRectF rectangle( i * ui.framePreview->width() / colorHTop.count(), 0, ui.framePreview->width() / colorHTop.count(), rectH );
			painter.fillRect( rectangle, colorHTop.at( i ) );
		}

		for ( int i = 0; i < colorHBottom.count(); i++ ) {
			QRectF rectangle( i *  ui.framePreview->width() / colorHBottom.count(), ui.framePreview->height() - rectH, ui.framePreview->width() / colorHBottom.count(), rectH );
			painter.fillRect( rectangle, colorHBottom.at( i ) );
		}

		for ( int i = 0; i < colorVLeft.count(); i++ ) {
			QRectF rectangle( 0, i *  ui.framePreview->height() / colorVLeft.count(), rectH, ui.framePreview->height() / colorVLeft.count() );
			painter.fillRect( rectangle, colorVLeft.at( i ) );
		}

		for ( int i = 0; i < colorVRight.count(); i++ ) {
			QRectF rectangle( ui.framePreview->width() - rectH, i *  ui.framePreview->height() / colorVRight.count(), rectH, ui.framePreview->height() / colorVRight.count() );
			painter.fillRect( rectangle, colorVRight.at( i ) );
		}
		
		QRectF rectangle( rectH, rectH, ui.framePreview->width() - 2*rectH, ui.framePreview->height() - 2*rectH );
		painter.drawRect( rectangle );
		rectangle = QRectF( 0, 0, ui.framePreview->width() - 1, ui.framePreview->height() - 1);
		painter.drawRect( rectangle );		
		
		res = true;
	}

	return res;
}
void Pixie::onImageCaptured( CRegions * regions ) {
	colorHTop.clear();
	colorHBottom.clear();
	colorVLeft.clear();
	colorVRight.clear();

	unsigned int hSize = regions->getHSize();
	for ( int i = 0; i < regions->regionHTop.count(); i++ ) {
		//saveImg( QString( "ht_%1.bmp" ).arg( i ), regions->regionHTop.at( i ), hSize, regions->hWidth, regions->hHeight );
		colorHTop.push_back( getAverageColor( regions->regionHTop.at( i ), regions->hWidth, regions->hHeight ) );
		delete [] regions->regionHTop.at( i );
	}
	for ( int i = 0; i < regions->regionHBottom.count(); i++ ) {
		//saveImg( QString( "hb_%1.bmp" ).arg( i ), regions->regionHBottom.at( i ), hSize, regions->hWidth, regions->hHeight );
		colorHBottom.push_back( getAverageColor( regions->regionHBottom.at( i ), regions->hWidth, regions->hHeight ) );
		delete [] regions->regionHBottom.at( i );
	}

	unsigned int vSize = regions->getVSize();
	for ( int i = 0; i < regions->regionVLeft.count(); i++ ) {
		//saveImg( QString( "vl_%1.bmp" ).arg( i ), regions->regionVLeft.at( i ), vSize, regions->vWidth, regions->vHeight );
		colorVLeft.push_back( getAverageColor( regions->regionVLeft.at( i ), regions->vWidth, regions->vHeight ) );
		delete [] regions->regionVLeft.at( i );
	}
	for ( int i = 0; i < regions->regionVRight.count(); i++ ) {
		//saveImg( QString( "vr_%1.bmp" ).arg( i ), regions->regionVRight.at( i ), vSize, regions->vWidth, regions->vHeight );
		colorVRight.push_back( getAverageColor( regions->regionVRight.at( i ), regions->vWidth, regions->vHeight ) );
		delete [] regions->regionVRight.at( i );
	}

	delete regions;	

	applyProcessing();
	if ( settings.previewEnabled )
		update();
	
	sendToArduino();

	readyToProcess = true;
}
void Pixie::initPort() {
	if ( port ) {
		port->close();
		delete port;
	}

	port = new QextSerialPort( getCOMPortName( settings.portNumber ).toLocal8Bit(), QextSerialPort::Polling );
	port->setBaudRate( settings.baudRate );
	port->setDataBits( DATA_8 );
	port->setFlowControl( FLOW_OFF );
	port->setParity( PAR_NONE );
	port->setStopBits( STOP_1 );
	port->setTimeout( 1 );	
}
int Pixie::colorInRange( const int oldColor, const int delta ) const {
	int res = oldColor + delta;
	if ( res > 255 )
		res = 255;
	if ( res < 0 )
		res = 0;

	return res;
}
void Pixie::processZone( QList< QColor >& colorZone ) {	
	bool useColorControl = ui.groupColorControl->isChecked();
	bool useCompression = ui.groupCompression->isChecked();

	for ( int i = 0; i < colorZone.count(); i++ ) {
		if ( useColorControl ) {
			int dFactor = ui.sGamma->value() * 10;
			if ( dFactor != 0 ) {
				if ( dFactor < 0 )
					colorZone[ i ] = colorZone.at( i ).darker( -dFactor + 100 );
				else
					colorZone[ i ] = colorZone.at( i ).lighter( 100 + dFactor );
			}
		
			if ( ui.sR->value() != 0 )
				colorZone[ i ].setRed( colorInRange( colorZone.at( i ).red(), ui.sR->value() ) );
		
			if ( ui.sG->value() != 0 )
				colorZone[ i ].setGreen( colorInRange( colorZone.at( i ).green(), ui.sG->value() ) );
					
			if ( ui.sB->value() != 0 )
				colorZone[ i ].setBlue( colorInRange( colorZone.at( i ).blue(), ui.sB->value() ) );
		}

		if ( useCompression ) {
			if ( ui.sThreshold->value() != 0 || ui.sLimitter->value() != 255 ) {
				int val = round( ( colorZone.at( i ).red() + colorZone.at( i ).green() + colorZone.at( i ).blue() ) / 3.0 );
				if ( ui.sThreshold->value() != 0 && val < ui.sThreshold->value() )
					colorZone[ i ] = QColor( 0, 0, 0 );

				if ( ui.sLimitter->value() != 255 && val > ui.sLimitter->value() ) {
					int dif = val - ui.sLimitter->value();
					colorZone[ i ] = QColor( colorInRange( colorZone[ i ].red(), -dif ),
						colorInRange( colorZone[ i ].green(), -dif ),
						colorInRange( colorZone[ i ].blue(), -dif ) );
				}			
			}
		}
	}
}
void Pixie::applyProcessing() {	
	processZone( colorVLeft );
	processZone( colorVRight );
	processZone( colorHTop );
	processZone( colorHBottom );
}
void Pixie::sendToArduino() {
	if( port && port->isOpen() ) {
		unsigned char led_num = colorVLeft.count() + colorHTop.count() + colorVRight.count() + colorHBottom.count();
		char * arr = new char[ led_num * 3 + 1 ];
		arr[ 0 ] = led_num;

		int index = 1;
		for ( int i = colorVLeft.count() - 1; i >= 0; i-- ) {
			arr[ index++ ] = (char)colorVLeft.at( i ).red();
			arr[ index++ ] = (char)colorVLeft.at( i ).green();		
			arr[ index++ ] = (char)colorVLeft.at( i ).blue();						
		}	
		for ( int i = 0; i < colorHTop.count(); i++ ) {
			arr[ index++ ] = (char)colorHTop.at( i ).red();
			arr[ index++ ] = (char)colorHTop.at( i ).green();
			arr[ index++ ] = (char)colorHTop.at( i ).blue();
		}	
		for ( int i = 0; i < colorVRight.count(); i++ ) {
			arr[ index++ ] = (char)colorVRight.at( i ).red();
			arr[ index++ ] = (char)colorVRight.at( i ).green();
			arr[ index++ ] = (char)colorVRight.at( i ).blue();
		}
		for ( int i = colorHBottom.count() - 1; i >= 0; i-- ) {
			arr[ index++ ] = (char)colorHBottom.at( i ).red();
			arr[ index++ ] = (char)colorHBottom.at( i ).green();
			arr[ index++ ] = (char)colorHBottom.at( i ).blue();
		}		

		while( true ) {
			QByteArray ba = port->readAll();
			if ( ba.count() > 0 ) {
				port->write( arr, led_num * 3 + 1 );
				delete [] arr;
				break;
			}// else {
				this->thread()->usleep( 50 );
			//}
		}
	}
}
/*
BOOL Pixie::SaveToFile( HBITMAP hBitmap, QString fname ) {
	HDC hDC;
	int iBits;
	WORD wBitCount;
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;
	BITMAP Bitmap;
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;
	HANDLE fh, hDib, hPal,hOldPal=NULL;

	hDC = CreateDC( L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	hDib = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal) {
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT) Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal) {
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFileA( fname.toLocal8Bit(), GENERIC_WRITE,0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

void Pixie::saveImg( QString fname, unsigned char * buf, unsigned int bufSize, unsigned int iw, unsigned int ih ) {
	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(bi));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biWidth = iw;
	bi.bmiHeader.biHeight = -ih;
	bi.bmiHeader.biSizeImage =  0;
	BYTE* pbuff;

	HBITMAP bmp = ::CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (void**)&pbuff, NULL, 0);
	SetDIBits(NULL, bmp, 0, ih, buf, &bi, DIB_RGB_COLORS);

	SaveToFile( bmp, fname.toLocal8Bit() );

	DeleteObject( bmp );
}
*/
void Pixie::displayRefreshRate( int rate ) {
	ui.labelFrameRate->setText( QString( tr( "Частота обновления = %1" ) ).arg( rate ) );
}
void Pixie::showHide( QSystemTrayIcon::ActivationReason reason ) {
	if ( reason == QSystemTrayIcon::ActivationReason::DoubleClick )
		QMainWindow::setVisible( !isVisible() );
}
void Pixie::onPreviewClick() {
	settings.previewEnabled = ui.bnPreviewEnabled->isChecked();
}
void Pixie::onSliderChanged() {
	ui.lGamma->setText( QString( "%1" ).arg( ui.sGamma->value() ) );
	ui.lR->setText( QString( "%1" ).arg( ui.sR->value() ) );
	ui.lG->setText( QString( "%1" ).arg( ui.sG->value() ) );
	ui.lB->setText( QString( "%1" ).arg( ui.sB->value() ) );

	ui.lThreshold->setText( QString( "%1" ).arg( ui.sThreshold->value() ) );
	ui.lLimitter->setText( QString( "%1" ).arg( ui.sLimitter->value() ) );
}
void Pixie::onLangChanged() {
	CSettings::LANG l = static_cast< CSettings::LANG >( ui.cbLanguage->itemData( ui.cbLanguage->currentIndex() ).toUInt() );
	if ( l == CSettings::ENGLISH_US ) {		
		translator->load( "pixie_en" );
	}
	if ( l == CSettings::RUSSIAN_RU ) {		
		translator->load( "pixie_ru" );		
	}
	qApp->installTranslator( translator );
	ui.retranslateUi( this );

	// Update other UI strings
	setThreadIsRunning( !stopThread );
	trayIcon->contextMenu()->actions()[ 0 ]->setText( tr( "Выход" ) );
	displayRefreshRate( 0 );
	onSliderChanged();
}
void Pixie::selectComboBoxItemByData( QComboBox * cb, const QVariant val ) {
	for ( int i = 0; i < cb->count(); i++ ) {
		if ( cb->itemData( i ) == val ) {
			cb->setCurrentIndex( i );
			break;
		}
	}
}