#include "StdAfx.h"
#include "CaptureThread.h"
#include "pixie.h"

CCaptureThread::CCaptureThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess ) {
	this->stopThread = stopThread;
	this->readyToProcess = readyToProcess;
	this->settings = settings;
	
	/* QT4 version of signals */
	/*
	QObject::connect( this, SIGNAL( onImageCaptured( CRegions * ) ), parent, SLOT( onImageCaptured( CRegions * ) ) );
	QObject::connect( this, SIGNAL( setIsRunning( bool ) ), parent, SLOT( setThreadIsRunning( bool ) ) );
	QObject::connect( this, SIGNAL( displayRefreshRate( int ) ), parent, SLOT( displayRefreshRate( int ) ) );
	*/

	QObject::connect( this, &CCaptureThread::onImageCaptured, (Pixie*)parent, &Pixie::onImageCaptured);
	QObject::connect( this, &CCaptureThread::setIsRunning, (Pixie*)parent, &Pixie::setThreadIsRunning);
	QObject::connect( this, &CCaptureThread::displayRefreshRate, (Pixie*)parent, &Pixie::displayRefreshRate);
}


CCaptureThread::~CCaptureThread() {
}

void CCaptureThread::updateDelayTime() {
	if ( frameTimer.elapsed() > 1000 ) {
		double delayAdd = 1000.0 / settings->desiredFramerate - 1000.0 / frameCounter;
		if ( delayAdd > 0 )
			delayTime += delayAdd * 1000;

		qDebug( "frameCounter is %d ", frameCounter );
		emit displayRefreshRate( frameCounter );
		frameCounter = 0;
		frameTimer.start();
	}
}