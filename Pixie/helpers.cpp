#include "stdafx.h"
#include "helpers.h"
#include "math.h"

double round( double d ) {
	return ( d > 0.0 ) ? floor( d + 0.5 ) : ceil( d - 0.5 );
}
const QString getCOMPortName( const unsigned char N ) {
	return QString( "\\\\.\\COM%1" ).arg( N );
}