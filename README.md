The **NeoHWSerial** class is intended as a drop-in replacement for the Arduino built-in class `HardwareSerial`.  The original files are modified to include one new method, `attachInterrupt`.  As new Arduino IDEs are released, new versions will appear in the root of this repository, named with the Arduino IDE version number.

The procedure passed to the `attachInterrupt` method is stored in the HardwareSerial instance.  That procedure will be called from the ISR whenever a character is received.  The received character will not be stored in the `rx_buffer`, and it will not be returned from `read()`.  Any characters that were received and buffered before `attachInterrupt` is called remain in `rx_buffer`, and could be returned from `read()`.

Remember that the registered procedure is called from an interrupt context, and should be very short.  Taking too much time in the procedure will cause many unpredictable behaviors, including loss of received data.  See the similar warnings for the built-in [`attachInterrupt`](https://www.arduino.cc/en/Reference/AttachInterrupt) for digital pins.

If `attachInterrupt` is never called, or it is passed a `NULL` procedure, the normal HardwareSerial buffering occurs, and all received characters must be obtained by calling `read()`.

