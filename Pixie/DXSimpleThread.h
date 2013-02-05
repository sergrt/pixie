//////////////////////////////////////////////////////////////////////////
//
// DirectX capture thread class
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CaptureThread.h"
#include <d3d9.h>

class CDXSimpleThread : public CCaptureThread {
public:
	CDXSimpleThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess );
	~CDXSimpleThread();	
private:
	void init();
	void run();
	void cleanup();
	
	unsigned char * buf;
	unsigned int bufSize;

	IDirect3D9 * g_pD3D;				// Some
	IDirect3DDevice9 * g_pd3dDevice;	//      DirectX
	IDirect3DSurface9 * pSurface;		//              Stuff

	int ScreenWidth, ScreenHeight;		// Last detected screen resolution
};

