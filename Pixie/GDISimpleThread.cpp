#include "StdAfx.h"
#include "GDISimpleThread.h"

CGDISimpleThread::CGDISimpleThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess )
	: CCaptureThread( parent, stopThread, settings, readyToProcess ) {	
	bufHSize = 0;
	bufH = NULL;

	bufVSize = 0;
	bufV = NULL;

	hCaptureBitmapH = NULL;
	hCaptureBitmapV = NULL;
	hDesktopWnd = NULL;
	hDesktopDC = NULL;
	hCaptureDC = NULL;
}

CGDISimpleThread::~CGDISimpleThread(void) {
	cleanup();
}

void CGDISimpleThread::init() {
	hDesktopWnd = GetDesktopWindow();
    hDesktopDC = GetDC( hDesktopWnd );
    hCaptureDC = CreateCompatibleDC( hDesktopDC );

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	initBuffers();
}

void CGDISimpleThread::initBuffers() {
	bufHSize = settings->getHorizontalHeight( ScreenHeight ) * settings->getHorizontalSegmentWidth( ScreenWidth ) * BITS_PER_PIXEL / 8;
	bufH = new unsigned char[ bufHSize ];

	hCaptureBitmapH = CreateCompatibleBitmap( hDesktopDC, settings->getHorizontalSegmentWidth( ScreenWidth ), settings->getHorizontalHeight( ScreenHeight ) );
	
	bufVSize = settings->getVerticalWidth( ScreenWidth ) * settings->getVerticalSegmentHeight( ScreenHeight ) * BITS_PER_PIXEL / 8;
	bufV = new unsigned char[ bufVSize ];

	hCaptureBitmapV = CreateCompatibleBitmap( hDesktopDC, settings->getVerticalWidth( ScreenWidth ), settings->getVerticalSegmentHeight( ScreenHeight ) );
}

void CGDISimpleThread::cleanup() {
	cleanupBuffers();

	if ( hDesktopDC )
		ReleaseDC( hDesktopWnd, hDesktopDC );
	if ( hCaptureDC )
		DeleteDC( hCaptureDC );	
}

void CGDISimpleThread::cleanupBuffers() {	
	if ( hCaptureBitmapH )
		DeleteObject( hCaptureBitmapH );
	if ( hCaptureBitmapV )
		DeleteObject( hCaptureBitmapV );

	if ( bufH ) {
		delete [] bufH;
		bufH = NULL;
	}
	if ( bufV ) {
		delete [] bufV;
		bufV = NULL;
	}
}

void CGDISimpleThread::run() {
	emit setIsRunning( true );
	init();

	delayTime = 10;	// initial delay time
	frameCounter = 0;
	frameTimer.start();
	
	while( !*stopThread ) {
		if ( *readyToProcess == true ) {
			// Check for resolution changes
			int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
			int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

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
			SelectObject( hCaptureDC, hCaptureBitmapH );
			// Top
			for ( unsigned short x = 0; x < settings->LEDnumH; x++ ) {
				BitBlt( hCaptureDC, 0, 0, regions->hWidth, regions->hHeight, hDesktopDC, x * regions->hWidth, 0, SRCCOPY|CAPTUREBLT );
				GetBitmapBits( hCaptureBitmapH, bufHSize, bufH );

				unsigned char * bufH_tmp = new unsigned char[ bufHSize ];
				memcpy( bufH_tmp, bufH, bufHSize );

				regions->regionHTop.push_back( bufH_tmp );
			}
			
			// Bottom
			for ( unsigned short x = 0; x < settings->LEDnumH; x++ ) {
				BitBlt( hCaptureDC, 0, 0, regions->hWidth, regions->hHeight, hDesktopDC, x * regions->hWidth, ScreenHeight - regions->hHeight, SRCCOPY|CAPTUREBLT );
				GetBitmapBits( hCaptureBitmapH, bufHSize, bufH );

				unsigned char * bufH_tmp = new unsigned char[ bufHSize ];
				memcpy( bufH_tmp, bufH, bufHSize );

				regions->regionHBottom.push_back( bufH_tmp );			
			}

			// Verticals
			SelectObject(hCaptureDC,hCaptureBitmapV);
			// Left
			for ( int x = 0; x < settings->LEDnumV; x++ ) {
				BitBlt( hCaptureDC, 0, 0, regions->vWidth, regions->vHeight, hDesktopDC, 0, x * regions->vHeight, SRCCOPY|CAPTUREBLT );
				GetBitmapBits( hCaptureBitmapV, bufVSize, bufV );

				unsigned char * bufV_tmp = new unsigned char[ bufVSize ];
				memcpy( bufV_tmp, bufV, bufVSize );

				regions->regionVLeft.push_back( bufV_tmp );
			}
			
			// Right
			for ( int x = 0; x < settings->LEDnumV; x++ ) {
				BitBlt( hCaptureDC, 0, 0, regions->vWidth, regions->vHeight, hDesktopDC, ScreenWidth - regions->vWidth, x * regions->vHeight, SRCCOPY|CAPTUREBLT );
				GetBitmapBits( hCaptureBitmapV, bufVSize, bufV );

				unsigned char * bufV_tmp = new unsigned char[ bufVSize ];
				memcpy( bufV_tmp, bufV, bufVSize );

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
