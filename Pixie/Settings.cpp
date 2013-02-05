#include "StdAfx.h"
#include "Settings.h"
#include <QSettings>
#include "helpers.h"

CSettings::CSettings() {
	// Provide default values
	LEDnumH				= 20;
	LEDnumV				= 10;
	horizontalHeightP	= 5;
	verticalWidthP		= 5;
	captureEngine		= CE_GDI;
	desiredFramerate	= 40;
	autoStart			= false;
	startMinimized		= false;
	previewEnabled		= false;
	baudRate			= BAUD115200;
	portNumber			= 3;
	
	useColorControl = false;
	gamma			= 0;
	redColor		= 0;
	greenColor		= 0;
	blueColor		= 0;
	useCompression	= false;
	threshold		= 0;
	limitter		= 255;
	language		= ENGLISH_US;
}
CSettings::~CSettings() {
}
void CSettings::loadSettings() {
	QSettings stgs( INIFILE::settingsFileName, QSettings::IniFormat );
	stgs.beginGroup( INIFILE::sectionMain );

	LEDnumH = stgs.value( INIFILE::LEDnumH ).toUInt();
	LEDnumV = stgs.value( INIFILE::LEDnumV ).toUInt();
	horizontalHeightP = stgs.value( INIFILE::horizontalHeightP ).toUInt();
	verticalWidthP = stgs.value( INIFILE::verticalWidthP ).toUInt();
	captureEngine = static_cast< CAPTURE_ENGINE >( stgs.value( INIFILE::captureEngine ).toUInt() );	
	desiredFramerate = stgs.value( INIFILE::desiredFramerate ).toUInt();
	autoStart = stgs.value( INIFILE::autoStart ).toBool();
	startMinimized = stgs.value( INIFILE::startMinimized ).toBool();
	previewEnabled = stgs.value( INIFILE::previewEnabled ).toBool();
	baudRate = static_cast< BaudRateType >( stgs.value( INIFILE::baudRate ).toUInt() );
	portNumber = stgs.value( INIFILE::portNumber ).toUInt();
	language = static_cast< LANG >( stgs.value( INIFILE::language ).toUInt() );
	stgs.endGroup();

	stgs.beginGroup( INIFILE::sectionProcessing );
	useColorControl = stgs.value( INIFILE::useColorControl ).toBool();
	gamma = stgs.value( INIFILE::gamma ).toInt();
	redColor = stgs.value( INIFILE::redColor ).toInt();
	greenColor = stgs.value( INIFILE::greenColor ).toInt();
	blueColor = stgs.value( INIFILE::blueColor ).toInt();
	useCompression = stgs.value( INIFILE::useCompression ).toBool();
	threshold = stgs.value( INIFILE::threshold ).toInt();
	limitter = stgs.value( INIFILE::limitter ).toInt();
	stgs.endGroup();
}
void CSettings::saveSettings() {
	QSettings stgs( INIFILE::settingsFileName, QSettings::IniFormat );
	stgs.beginGroup( INIFILE::sectionMain );

	stgs.setValue( INIFILE::LEDnumH, LEDnumH );
	stgs.setValue( INIFILE::LEDnumV, LEDnumV );
	stgs.setValue( INIFILE::horizontalHeightP, horizontalHeightP );
	stgs.setValue( INIFILE::verticalWidthP, verticalWidthP );
	stgs.setValue( INIFILE::captureEngine, static_cast< unsigned int >( captureEngine ) );
	stgs.setValue( INIFILE::desiredFramerate, desiredFramerate );
	stgs.setValue( INIFILE::autoStart, autoStart );
	stgs.setValue( INIFILE::startMinimized, startMinimized );
	stgs.setValue( INIFILE::previewEnabled, previewEnabled );
	stgs.setValue( INIFILE::baudRate, baudRate );
	stgs.setValue( INIFILE::portNumber, portNumber );
	stgs.setValue( INIFILE::language, static_cast< unsigned int >( language ) );
	stgs.endGroup();

	stgs.beginGroup( INIFILE::sectionProcessing );
	stgs.setValue( INIFILE::useColorControl, useColorControl );
	stgs.setValue( INIFILE::gamma, gamma );
	stgs.setValue( INIFILE::redColor, redColor );
	stgs.setValue( INIFILE::greenColor, greenColor );
	stgs.setValue( INIFILE::blueColor, blueColor );
	stgs.setValue( INIFILE::useCompression, useCompression );
	stgs.setValue( INIFILE::threshold, threshold );
	stgs.setValue( INIFILE::limitter, limitter );

	stgs.endGroup();
}
int CSettings::getHorizontalSegmentWidth( const int screenWidth ) const {
	return round( screenWidth / static_cast< double >( LEDnumH ) );
}
int CSettings::getVerticalSegmentHeight( const int screenHeight ) const {
	return round( screenHeight / static_cast< double >( LEDnumV ) );
}
int CSettings::getHorizontalHeight( const int screenHeight ) const {
	return round( screenHeight * horizontalHeightP / 100.0 );
}
int CSettings::getVerticalWidth( const int screenWidth ) const {
	return round( screenWidth * verticalWidthP / 100.0 );
}