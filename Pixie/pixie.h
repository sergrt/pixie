//////////////////////////////////////////////////////////////////////////
//
// Main app window
//
//////////////////////////////////////////////////////////////////////////
#ifndef PIXIE_H
#define PIXIE_H

#include <QMainWindow>
#include "ui_pixie.h"
#include <QSystemTrayIcon>
#include "CaptureThread.h"
#include "qextserialport.h"

/*** This should be uncommented for SaveToFile() function to work ***/
/*
#define WINVER 0x0500
#include "windows.h"
*/

class Pixie : public QMainWindow {
	Q_OBJECT

public:
	Pixie( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
	~Pixie();

private:
	Ui::PixieClass ui;
	QSystemTrayIcon* trayIcon;
	QTranslator* translator;

	bool stopThread;				// Stop thread flag
	bool readyToProcess;			// App ready to process new regions data flag
	QextSerialPort* port;
	CCaptureThread* captureThread;
	CSettings settings;	

	void applySettingsFromUI();			// Fill settings from UI
	void applySettingsToUI();		// Set settings to UI elements

	// For debug purposes
	/*
	void saveImg( QString fname, unsigned char * buf, unsigned int bufSize, unsigned int iw, unsigned int ih );
	BOOL SaveToFile( HBITMAP hBitmap, QString fname );
	*/

	QList< QColor > colorHTop;
	QList< QColor > colorHBottom;
	QList< QColor > colorVLeft;
	QList< QColor > colorVRight;	
	bool eventFilter( QObject* watched, QEvent* event );	// For preview
	
	void initPort();		// COM-port initialization
	void sendToArduino();	// Send LEDs information to Arduino board
	
	void applyProcessing();											// Apply color modifications - gamma etc.
	void processZone( QList< QColor >& colorZone );					// Apply color modifications to specified zone
	void processGrouping( QList< QColor >& colorZone, const unsigned short group );		// Apply pixel grouping
	int colorInRange( const int oldColor, const int delta ) const;	// Adjust oldColor by delta within range 0-255
	QColor getAverageColor( const unsigned char * const buf, const unsigned short width, const unsigned short height ) const;	// Calculate average color of segment
	void selectComboBoxItemByData( QComboBox * cb, const QVariant val );	// Simply selects Combo box item by it's data
public slots:
	void onStartStopClick();						// Start-stop button click
	void setThreadIsRunning( bool r );				// Update UI from thread by it's state
	void onImageCaptured( CRegions *regions );		// Process regions data from thread
	void displayRefreshRate( int rate );			// Display current refresh rate

	void onCloseClick();							// Tray icon menu "Exit" click
	void showHide( QSystemTrayIcon::ActivationReason reason );	// Show-hide main window on double-click
	void onPreviewClick();							// Enable-disable preview
	void onSliderChanged();							// On any slider changed value
	void onLangChanged();							// On program language changed
};

#endif // PIXIE_H
