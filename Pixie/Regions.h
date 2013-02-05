//////////////////////////////////////////////////////////////////////////
//
// Simple class for raw regions data storage
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <QList>

class CRegions
{
public:
	explicit CRegions( const int hW, const int hH, const int vW, const int vH );	// Do provide dimensions, or else there's no sense
	QList< unsigned char * > regionHTop;
	QList< unsigned char * > regionHBottom;
	QList< unsigned char * > regionVLeft;
	QList< unsigned char * > regionVRight;

	unsigned int getHSize() const;	// Size (int bytes) of horizontal segment
	unsigned int getVSize() const;	// Size (int bytes) of vertical segment

//private:
	unsigned short hWidth;		// Width (in pixels) of horizontal segment
	unsigned short hHeight;		// Height (in pixels) of horizontal segment

	unsigned short vWidth;		// Width (in pixels) of vertical segment
	unsigned short vHeight;		// Height (in pixels) of vertical segment
};

