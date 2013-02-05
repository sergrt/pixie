//////////////////////////////////////////////////////////////////////////
//
// Lab color handling class
// Based on some examples found over internet
// !!! It's VERY SLOW and has no optimizations
// !!! Not used in this build
//
//////////////////////////////////////////////////////////////////////////
#pragma once
class CLAB {
public:
	CLAB();
	CLAB( const unsigned char R, const unsigned char G, const unsigned char B );
	
	QColor getRGB() const;	// Get RGB QColor from LAB

	double mL;
	double mA;
	double mB;
private:
	void initConstants();

	double ref_X;
	double ref_Y;
	double ref_Z;
};

