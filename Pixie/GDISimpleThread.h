//////////////////////////////////////////////////////////////////////////
//
// GDI capture thread class
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CaptureThread.h"

#define WINVER 0x0500
#include "windows.h"

class CGDISimpleThread : public CCaptureThread {
public:
	CGDISimpleThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess );
	~CGDISimpleThread();	
private:
	virtual void init();
	virtual void run();
	virtual void cleanup();
	void cleanupBuffers();
	
	void initBuffers();	// Buffers (re)initialization - in the beginning and every screen resolution change

	HWND hDesktopWnd;	// Some
	HDC hDesktopDC;		//      capture
	HDC hCaptureDC;		//              stuff

	unsigned char * bufH;	// Buffer for horizontal element
	unsigned char * bufV;	// Buffer for vertical element
	unsigned int bufHSize;	// Corresponding
	unsigned int bufVSize;	//           sizes of buffers
	
	HBITMAP hCaptureBitmapH;
	HBITMAP hCaptureBitmapV;

	int ScreenWidth, ScreenHeight;	// Last detected screen resolution
};

