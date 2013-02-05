/*** ARDUINO CODE FOR PIXEL LIGHT ***/
#include <SPI.h>
#include <SmallUart.h>

unsigned long lastTime;  // Time strip was updated last time
const unsigned long fadeTimeout = 3000;

////////////////////////////////////////////////////////////
//
void setup() {
  UART_Init(115200);  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  blackoutAll();
  delay(1);
  lastTime = millis();
}

////////////////////////////////////////////////////////////
//
void loop() {
  uint8_t data; 
  UART_SendByte( 'R' ); // Byte "We're ready"
  bool valid = false;
  data = uartRead( valid );
  if ( valid ) {
    uint16_t pix_num = data * 3; // Total following bytes
    for( uint16_t i=0; i < pix_num; i++ ) {
      data = uartRead( valid );
      if ( !valid )
        break;
      SPI.transfer( data ); // Transfer byte to SPI
    }
    lastTime = millis();
  }
  
  if ( millis() - lastTime > fadeTimeout )
    blackoutAll();
}

////////////////////////////////////////////////////////////
// Turn off all possible 256 leds
void blackoutAll() {
  for ( int16_t i = 0; i < 768; i++ )
    SPI.transfer( 0 ); //погасить все пикселы ленты
}

////////////////////////////////////////////////////////////
// Read byte with timeout
unsigned char uartRead( bool& valid ) {
  uint8_t res = 0;
  valid = false;
  for ( uint8_t i = 0; i < 255; ++i ) { // Max timeout 256*10
    if( UART_ReadByte( res ) ) {
      valid = true;
      break;
    }
    delayMicroseconds(10);
  } 
  return res;
}
