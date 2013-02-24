//////////////////////////////////////////////////////////////////////////
//
//  Parent class for screen capture threads
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <QThread>
#include <QTime>
#include "Regions.h"
#include "Settings.h"

class CCaptureThread : public QThread {
	Q_OBJECT

public:
	CCaptureThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess );
	virtual ~CCaptureThread();

protected:
	bool *stopThread;		// Indicates that thread should be stopped
	bool *readyToProcess;	// Indicates that parent widget is ready to process data
	CSettings *settings;
	virtual void init() = 0;	// Init buffers, etc. before thread starts
	virtual void run() = 0;		// Thread routine
	virtual void cleanup() = 0;	// Free all resources after thread ends

	QTime frameTimer;			// Timer to handle framerate
	unsigned int frameCounter;	// Frames per second
	unsigned long delayTime;	// Interframe delay
	void updateDelayTime();		// Update interframe delay to gain desired framerate

signals:
	void onImageCaptured( CRegions * ); // Send regions data to process by parent widget
	void setIsRunning( bool );			// Send thread status updates to parent widget
	void displayRefreshRate( int );		// Send refresh rate to display
};

