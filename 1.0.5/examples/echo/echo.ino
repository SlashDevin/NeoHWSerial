#include <NeoHWSerial.h>

//
//  Example program to show how to attach to the received character
//  interrupt with NeoHWSerial.
//

static volatile uint16_t count = 0;

static void char_received( uint8_t c )
{
  // This is a little naughty, as it will try to block
  //   in this ISR if the tx_buffer is full.  For this example,
  //   we are only sending as many characters as we have received,
  //   and they arrive at the same rate we are sending them.
  NeoSerial.write( c );
  count++;
}

void setup()
{
  NeoSerial.begin( 9600 );
  NeoSerial.attachInterrupt( char_received );
  NeoSerial.println( F("Enter data to echo:") );
}

void loop()
{
  delay( 1000 );

  uint8_t oldSREG = SREG;
  noInterrupts();
    uint16_t old_count = count;
    count = 0;
  SREG = oldSREG;
  
  if (old_count) {
    NeoSerial.print( '\n' );
    NeoSerial.print( old_count );
    NeoSerial.println( F(" characters echoed") );
  } else
    NeoSerial.print( '.' );
  NeoSerial.flush();
}