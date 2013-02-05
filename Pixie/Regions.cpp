#include "stdafx.h"
#include "Regions.h"
CRegions::CRegions( const int hW, const int hH, const int vW, const int vH ) {
	hWidth	= hW;
	hHeight = hH;
	vWidth	= vW;
	vHeight = vH;
}
unsigned int CRegions::getHSize() const {
	return hWidth * hHeight * BITS_PER_PIXEL / 8;
}
unsigned int CRegions::getVSize() const {
	return vWidth * vHeight * BITS_PER_PIXEL / 8;
}