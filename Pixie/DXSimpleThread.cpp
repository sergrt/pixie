#include "StdAfx.h"
#include "DXSimpleThread.h"

CDXSimpleThread::CDXSimpleThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess )
	: CCaptureThread( parent, stopThread, settings, readyToProcess ) {
	bufSize = 0;
	buf = NULL;

	g_pd3dDevice = NULL;
	pSurface = NULL;
	g_pD3D = NULL;
}

CDXSimpleThread::~CDXSimpleThread(void) {
	cleanup();
}

void CDXSimpleThread::init() {
	if( ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) == NULL ) {
		throw( "Unable to Create Direct3D" );
	}

	D3DDISPLAYMODE ddm;
	if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &ddm ) ) ) {
		throw( "Unable to Get Adapter Display Mode" );
	}

	ScreenHeight = ddm.Height;
	ScreenWidth = ddm.Width;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( D3DPRESENT_PARAMETERS ) );
	
	HWND hWnd = GetDesktopWindow();
	
	d3dpp.Windowed = TRUE;//WINDOW_MODE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = ddm.Height;
	d3dpp.BackBufferWidth = ddm.Width;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice ) ) ) {
		throw( "Unable to Create Device" );
	}

	if( FAILED( g_pd3dDevice->CreateOffscreenPlainSurface( ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL ) ) ) {
		throw( "Unable to Create Surface" );
	}

	bufSize = ScreenWidth * ScreenHeight * BITS_PER_PIXEL / 8;
	buf = new unsigned char[ bufSize ];
}

void CDXSimpleThread::cleanup() {
	if ( pSurface != NULL ) {
		pSurface->Release();
		pSurface = NULL;
	}

	if ( g_pd3dDevice != NULL ) {
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}

	if ( g_pD3D != NULL ) {
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	if ( buf ) {
		delete [] buf;
		buf = NULL;
	}
}

void CDXSimpleThread::run() {
	emit setIsRunning( true );
	init();

	delayTime = 10;	// initial delay time
	frameCounter = 0;
	frameTimer.start();

	while( !*stopThread ) {
		if ( *readyToProcess == true ) {
			// Check resolution		
			D3DDISPLAYMODE ddm;
			if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &ddm ) ) ) {
				throw( "Unable to get adapter display mode" );
				//break;
			}
			if ( ddm.Height != ScreenHeight || ddm.Width != ScreenWidth ) {
				cleanup();
				init();
			}
		
			// Capture screen
			g_pd3dDevice->GetFrontBufferData( 0, pSurface );
			D3DLOCKED_RECT lockedRect;
			pSurface->LockRect( &lockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY );
	
			for( int i=0 ; i < ScreenHeight ; i++ )	{
				memcpy( buf + i * ScreenWidth * BITS_PER_PIXEL / 8, (BYTE*)lockedRect.pBits + i* lockedRect.Pitch, ScreenWidth * BITS_PER_PIXEL / 8 );
			}
			pSurface->UnlockRect();

			CRegions * regions = new CRegions( settings->getHorizontalSegmentWidth( ScreenWidth ), settings->getHorizontalHeight( ScreenHeight ),
				settings->getVerticalWidth( ScreenWidth ), settings->getVerticalSegmentHeight( ScreenHeight ) )	;
			
			// Horizontal
			// Top
			for( int i = 0; i < settings->LEDnumH; i++ ) {
				unsigned char * buf_tmp = new unsigned char[ regions->hWidth * regions->hHeight * BITS_PER_PIXEL / 8 ];
				for( int x = 0; x < regions->hHeight; x++ )
					memcpy( buf_tmp + x * regions->hWidth * BITS_PER_PIXEL / 8, buf + ( x * ScreenWidth + i * regions->hWidth ) * BITS_PER_PIXEL / 8, regions->hWidth * BITS_PER_PIXEL / 8 );

				regions->regionHTop.push_back( buf_tmp );
			}

			// Bottom
			for( int i = 0; i < settings->LEDnumH; i++ ) {
				unsigned char * buf_tmp = new unsigned char[ regions->hWidth * regions->hHeight * BITS_PER_PIXEL / 8 ];
				for( int x = 0; x < regions->hHeight; x++ )
					memcpy( buf_tmp + x * regions->hWidth * BITS_PER_PIXEL / 8, buf + ( ( ScreenHeight - regions->hHeight ) * ScreenWidth + x * ScreenWidth + i * regions->hWidth ) * BITS_PER_PIXEL / 8, regions->hWidth * BITS_PER_PIXEL / 8 );
				
				regions->regionHBottom.push_back( buf_tmp );
			}

			// Verticals
			// Left
			for ( int i = 0; i < settings->LEDnumV; i++ ) {
				unsigned char * buf_tmp = new unsigned char[ regions->vWidth * regions->vHeight * BITS_PER_PIXEL / 8 ];
				for( int x = 0; x < regions->vHeight; x++ )
					memcpy( buf_tmp + x * regions->vWidth * BITS_PER_PIXEL / 8, buf + ( i * regions->vHeight * ScreenWidth + x * ScreenWidth ) * BITS_PER_PIXEL / 8, regions->vWidth * BITS_PER_PIXEL / 8 );
				
				regions->regionVLeft.push_back( buf_tmp );
			}

			// Right
			for ( int i = 0; i < settings->LEDnumV; i++ ) {
				unsigned char * buf_tmp = new unsigned char[ regions->vWidth * regions->vHeight * BITS_PER_PIXEL / 8 ];
				for( int x = 0; x < regions->vHeight; x++ )
					memcpy( buf_tmp + x * regions->vWidth * BITS_PER_PIXEL / 8, buf + ( i * regions->vHeight *  ScreenWidth + ( ScreenWidth - regions->vWidth ) + x * ScreenWidth ) * BITS_PER_PIXEL / 8, regions->vWidth * BITS_PER_PIXEL / 8 );
				
				regions->regionVRight.push_back( buf_tmp );
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

