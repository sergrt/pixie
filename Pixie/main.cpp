#include "stdafx.h"
#include "pixie.h"
#include <QApplication>

#ifdef WIN32
#include <c:\Program Files (x86)\Visual Leak Detector\include\vld.h>
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Pixie w;
	//w.show();
	return a.exec();
}
