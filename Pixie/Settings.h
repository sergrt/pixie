//////////////////////////////////////////////////////////////////////////
//
// Settings storage and processing class
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "qextserialport.h"
//////////////////////////////////////////////////////////////////////////
// INI file constants
namespace INIFILE {
	const QString settingsFileName		= "settings.ini";	
	// Section Main
	const QString sectionMain			= "Main";

	const QString LEDnumH				= "LEDnumH";			// Horizontal number of LEDs
	const QString LEDnumV				= "LEDnumV";			// Vertical number of LEDs
	const QString horizontalHeightP		= "horizontalHeightP";	// Percent of screen height in horizontal block
	const QString verticalWidthP		= "verticalWidthP";		// Percent of screen width in vertical block
	const QString captureEngine			= "captureEngine";		// Engine to use (GDI, DX)
	const QString desiredFramerate		= "desiredFramerate";	// Frame rate to limit
	const QString autoStart				= "autoStart";			// Start capture thread on program start
	const QString startMinimized		= "startMinimized";		// Start program minimized to tray
	const QString previewEnabled		= "enablePreview";		// Enable preview
	const QString baudRate				= "baudRate";
	const QString portNumber			= "portNumber";
	const QString language				= "language";
	
	// Section Processing
	const QString sectionProcessing		= "Processing";
	const QString useColorControl		= "useColorControl";
	const QString gamma					= "gamma";
	const QString redColor				= "redColor";
	const QString greenColor			= "greenColor";
	const QString blueColor				= "blueColor";	
	const QString useCompression		= "useCompression";
	const QString threshold				= "threshold";
	const QString limitter				= "limitter";
	const QString useGrouping			= "useGrouping";
	const QString groupHorizontal		= "groupHorizontal";
	const QString groupVertical			= "groupVertical";
}

class CSettings {
public:
	CSettings();
	~CSettings();

	void loadSettings();
	void saveSettings();

	unsigned short LEDnumH;				// Number of horizontal LEDs
	unsigned short LEDnumV;				// Number of vertical LEDs
	unsigned short horizontalHeightP;	// Height of horizontal segment (% of screen)
	unsigned short verticalWidthP;		// Width of vertical segment (% of screen)

	enum CAPTURE_ENGINE {
		CE_GDI = 0,
		CE_DX = 1
	};
	CAPTURE_ENGINE captureEngine;
	unsigned short desiredFramerate;
	bool autoStart;						// Begin capture on program start
	bool startMinimized;				// Start program minimized to tray
	bool previewEnabled;				// Enable preview window

	BaudRateType baudRate;				// COM-port baud rate
	unsigned char portNumber;			// COM number

	bool useColorControl;				// Use (or not) gamma and RGB controls
	int gamma;
	int redColor;
	int greenColor;
	int blueColor;
	
	bool useCompression;				// Use (or not) color threshold and limitting
	int threshold;						// Threshold value
	int limitter;						// Limitter value

	bool useGrouping;
	unsigned short groupHorizontal;
	unsigned short groupVertical;

	enum LANG {
		RUSSIAN_RU = 0,
		ENGLISH_US = 1
	};
	LANG language;						// Program language

	// Functions to get segments dimensions - in pixels
	int getHorizontalSegmentWidth( const int screenWidth ) const;
	int getHorizontalHeight( const int screenHeight ) const;
	int getVerticalSegmentHeight( const int screenHeight ) const;
	int getVerticalWidth( const int screenWidth ) const;
};

