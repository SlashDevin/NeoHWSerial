#include <NeoHWSerial.h>

//
//  Example program to show how to attach to the received character
//  interrupt with NeoHWSerial.
//

static volatile uint16_t ISR_count = 0;

static bool char_received( uint8_t c, uint8_t status )
{
  // This is a little naughty, as it will try to block
  //   in this ISR if the tx_buffer is full.  For this example,
  //   we are only sending as many characters as we have received,
  //   and they arrive at the same rate we are sending them.
  NeoSerial.write( c );
  ISR_count++;
  return false;  // do not store received data to ring buffer
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

  // get number of calls to char_received()
  noInterrupts();
  uint16_t echo_count = ISR_count;
  ISR_count = 0;
  interrupts();

  if (echo_count)
  {
    NeoSerial.print( '\n' );
    NeoSerial.print( echo_count );
    NeoSerial.println( F(" characters echoed") );
  }
  else
  {
    NeoSerial.print( '.' );
  }
  NeoSerial.flush();
}
