#pragma once
#pragma once
#include "CaptureThread.h"
#include <qdesktopwidget.h>

class CUniCaptureThread : public CCaptureThread {
public:
	CUniCaptureThread( QWidget *parent, bool *stopThread, CSettings *settings, bool *readyToProcess, const int d );
	~CUniCaptureThread();
private:
	virtual void init();
	virtual void run();
	virtual void cleanup();
	void cleanupBuffers();	
	void initBuffers();	// Buffers (re)initialization - in the beginning and every screen resolution change

	QWidget *mainWnd;
	QScreen *scr;
	int desktopID;

	unsigned char * bufH;	// Buffer for horizontal element
	unsigned char * bufV;	// Buffer for vertical element
	unsigned int bufHSize;	// Corresponding
	unsigned int bufVSize;	//           sizes of buffers

	int ScreenWidth, ScreenHeight;	// Last detected screen resolution
};

