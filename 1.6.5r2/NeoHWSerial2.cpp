/*
  NeoHWSerial2.cpp - Hardware serial library with attachInterrupt
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 28 September 2010 by Mark Sproul
  Modified 14 August 2012 by Alarus
  Modified 3 December 2013 by Matthijs Kooijman
  Modified 2 November 2015 by SlashDev
*/

#include "Arduino.h"
#include <NeoHWSerial.h>
#include <NeoHWSerial_private.h>

// Each NeoHWSerial is defined in its own file, sine the linker pulls
// in the entire file when any element inside is used. --gc-sections can
// additionally cause unused symbols to be dropped, but ISRs have the
// "used" attribute so are never dropped and they keep the
// NeoHWSerial instance in as well. Putting each instance in its own
// file prevents the linker from pulling in any unused instances in the
// first place.

#if defined(HAVE_HWSERIAL2)

ISR(USART2_RX_vect)
{
  NeoSerial2._rx_complete_irq();
}

ISR(USART2_UDRE_vect)
{
  NeoSerial2._tx_udr_empty_irq();
}

NeoHWSerial NeoSerial2(&UBRR2H, &UBRR2L, &UCSR2A, &UCSR2B, &UCSR2C, &UDR2);

#endif // HAVE_HWSERIAL2
