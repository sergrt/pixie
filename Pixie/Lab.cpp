#include "StdAfx.h"
#include "Lab.h"

CLAB::CLAB() {
	initConstants();
}

CLAB::CLAB( const unsigned char R, const unsigned char G, const unsigned char B ) {
	initConstants();

	// RGB -> XYZ
	double var_R = ( R / 255.0 );        //R from 0 to 255
	double var_G = ( G / 255.0 );        //G from 0 to 255
	double var_B = ( B / 255.0 );        //B from 0 to 255

	if ( var_R > 0.04045 )
		var_R = pow( ( ( var_R + 0.055 ) / 1.055 ), 2.4 );
	else
		var_R = var_R / 12.92;

	if ( var_G > 0.04045 )
		var_G = pow( ( ( var_G + 0.055 ) / 1.055 ), 2.4 );
	else
		var_G = var_G / 12.92;

	if ( var_B > 0.04045 )
		var_B = pow( ( ( var_B + 0.055 ) / 1.055 ), 2.4 );
	else
		var_B = var_B / 12.92;

	var_R = var_R * 100.0;
	var_G = var_G * 100.0;
	var_B = var_B * 100.0;

	//Observer. = 2°, Illuminant = D65
	double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

	//////////////////////////////////////////////////////////////////////////
	// XYZ -> LAB

	double var_X = X / ref_X;          //ref_X =  95.047   Observer= 2°, Illuminant= D65
	double var_Y = Y / ref_Y;          //ref_Y = 100.000
	double var_Z = Z / ref_Z;          //ref_Z = 108.883

	if ( var_X > 0.008856 )
		var_X = pow( var_X, 1 / 3.0 );
	else
		var_X = ( 7.787 * var_X ) + ( 16 / 116.0 );

	if ( var_Y > 0.008856 )
		var_Y = pow( var_Y, 1 / 3.0 );
	else
		var_Y = ( 7.787 * var_Y ) + ( 16 / 116.0 );

	if ( var_Z > 0.008856 )
		var_Z = pow( var_Z, 1 / 3.0 );
	else
		var_Z = ( 7.787 * var_Z ) + ( 16 / 116.0 );

	mL = ( 116.0 * var_Y ) - 16.0;
	mA = 500.0 * ( var_X - var_Y );
	mB = 200.0 * ( var_Y - var_Z );
}

void CLAB::initConstants() {
	ref_X = 95.047;
	ref_Y = 100.000;
	ref_Z = 108.883;
}

QColor CLAB::getRGB() const {
	//////////////////////////////////////////////////////////////////////////
	// LAB to XYZ

	double var_Y = ( mL + 16 ) / 116.0;
	double var_X = mA / 500.0 + var_Y;
	double var_Z = var_Y - mB / 200.0;

	if ( pow( var_Y, 3.0 ) > 0.008856 )
		var_Y = pow( var_Y, 3.0 );
	else
		var_Y = ( var_Y - 16 / 116.0 ) / 7.787;

	if ( pow( var_X, 3.0 ) > 0.008856 )
		var_X = pow( var_X, 3.0 );
	else
		var_X = ( var_X - 16 / 116.0 ) / 7.787;

	if ( pow( var_Z, 3.0 ) > 0.008856 )
		var_Z = pow( var_Z, 3.0 );
	else
		var_Z = ( var_Z - 16 / 116.0 ) / 7.787;

	double X = ref_X * var_X;     //ref_X =  95.047     Observer= 2°, Illuminant= D65
	double Y = ref_Y * var_Y;     //ref_Y = 100.000
	double Z = ref_Z * var_Z;     //ref_Z = 108.883

	//////////////////////////////////////////////////////////////////////////
	// XYZ to RGB

	var_X = X / 100.0;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
	var_Y = Y / 100.0;        //Y from 0 to 100.000
	var_Z = Z / 100.0;        //Z from 0 to 108.883

	double var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
	double var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
	double var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

	if ( var_R > 0.0031308 )
		var_R = 1.055 * pow( var_R,  1 / 2.4 ) - 0.055;
	else
		var_R = 12.92 * var_R;

	if ( var_G > 0.0031308 )
		var_G = 1.055 * pow( var_G, 1 / 2.4 ) - 0.055;
	else
		var_G = 12.92 * var_G;

	if ( var_B > 0.0031308 )
		var_B = 1.055 * pow( var_B, 1 / 2.4 ) - 0.055;
	else
		var_B = 12.92 * var_B;

	unsigned char R = var_R * 255.0;
	unsigned char G = var_G * 255.0;
	unsigned char B = var_B * 255.0;

	return QColor( B, G, R );
}