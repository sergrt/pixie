#include "stdafx.h"
#include "UniCaptureThread.h"

CUniCaptureThread::CUniCaptureThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess, const int d ) 
	: CCaptureThread( parent, stopThread, settings, readyToProcess ) {
	mainWnd = parent;
	desktopID = d;

	bufHSize = 0;
	bufH = NULL;

	bufVSize = 0;
	bufV = NULL;
}

CUniCaptureThread::~CUniCaptureThread() {
}
void CUniCaptureThread::init() {
	scr = mainWnd->windowHandle()->screen();	
	QRect screenGeometry = scr->geometry();

	ScreenWidth  = screenGeometry.width();
	ScreenHeight = screenGeometry.height();
	initBuffers();
}
void CUniCaptureThread::initBuffers() {
	bufHSize = settings->getHorizontalHeight( ScreenHeight ) * settings->getHorizontalSegmentWidth( ScreenWidth ) * BITS_PER_PIXEL / 8;
	bufH = new unsigned char[ bufHSize ];

	bufVSize = settings->getVerticalWidth( ScreenWidth ) * settings->getVerticalSegmentHeight( ScreenHeight ) * BITS_PER_PIXEL / 8;
	bufV = new unsigned char[ bufVSize ];
}
void CUniCaptureThread::run() {
	emit setIsRunning( true );
	init();

	delayTime = 10;	// initial delay time
	frameCounter = 0;
	frameTimer.start();
	
	while( !*stopThread ) {
		if ( *readyToProcess == true ) {			
			// Check resolution changes
			QRect screenGeometry = scr->geometry();

			int nScreenWidth  = screenGeometry.width();
			int nScreenHeight = screenGeometry.height();

			if ( nScreenWidth != ScreenWidth || nScreenHeight != ScreenHeight )	{
				ScreenWidth = nScreenWidth;
				ScreenHeight = nScreenHeight;

				cleanupBuffers();
				initBuffers();
			}  

			// Creating and filling regions data
			CRegions * regions = new CRegions( settings->getHorizontalSegmentWidth( ScreenWidth ), settings->getHorizontalHeight( ScreenHeight ),
				settings->getVerticalWidth( ScreenWidth ), settings->getVerticalSegmentHeight( ScreenHeight ) );
			
			// Horizontals
			// Top
			for ( unsigned short x = 0; x < settings->LEDnumH; x++ ) {
				QImage im = scr->grabWindow( desktopID, x * regions->hWidth, 0, regions->hWidth, regions->hHeight ).toImage();
				unsigned char * bufH_tmp = new unsigned char[ bufHSize ];
				memcpy( bufH_tmp, im.bits(), bufHSize );

				regions->regionHTop.push_back( bufH_tmp );
			}
			
			// Bottom
			for ( unsigned short x = 0; x < settings->LEDnumH; x++ ) {
				QImage im = scr->grabWindow( desktopID, x * regions->hWidth, ScreenHeight - regions->hHeight, regions->hWidth, regions->hHeight ).toImage();
				unsigned char * bufH_tmp = new unsigned char[ bufHSize ];
				memcpy( bufH_tmp, im.bits(), bufHSize );

				regions->regionHBottom.push_back( bufH_tmp );			
			}

			// Verticals
			// Left
			for ( int x = 0; x < settings->LEDnumV; x++ ) {
				QImage im = scr->grabWindow( desktopID, 0, x * regions->vHeight, regions->vWidth, regions->vHeight ).toImage();
				unsigned char * bufV_tmp = new unsigned char[ bufVSize ];
				memcpy( bufV_tmp, im.bits(), bufVSize );

				regions->regionVLeft.push_back( bufV_tmp );
			}
			
			// Right
			for ( int x = 0; x < settings->LEDnumV; x++ ) {
				QImage im = scr->grabWindow( desktopID, ScreenWidth - regions->vWidth, x * regions->vHeight, regions->vWidth, regions->vHeight ).toImage();
				unsigned char * bufV_tmp = new unsigned char[ bufVSize ];
				memcpy( bufV_tmp, im.bits(), bufVSize );

				regions->regionVRight.push_back( bufV_tmp );			
			}

			*readyToProcess = false;
			emit onImageCaptured( regions );
			++frameCounter;
			updateDelayTime();
		}		
		usleep( delayTime );
	}

	cleanup();
	emit setIsRunning( false );
	//*stopThread = true;
}
void CUniCaptureThread::cleanup() {
	cleanupBuffers();
}
void CUniCaptureThread::cleanupBuffers() {	
	if ( bufH ) {
		delete [] bufH;
		bufH = NULL;
	}
	if ( bufV ) {
		delete [] bufV;
		bufV = NULL;
	}
}