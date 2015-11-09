The **NeoHWSerial** class is intended as a drop-in replacement for the Arduino built-in class `HardwareSerial`.  To handle received characters with your procedure, you must register it with the specific instance `NeoSerial[n]` instance:

```
  NeoSerial1.attachInterrupt( handleRxChar ); // Instead of 'Serial1'
```

To avoid name collisions, all `HardwareSerial` instances are prefixed with "Neo": your sketch will use `NeoSerial`, `NeoSerial1`, `NeoSerial2`, or `NeoSerial3`.

Remember that the registered procedure is called from an interrupt context, and it should return as quickly as possible.  Taking too much time in the procedure will cause many unpredictable behaviors, including loss of received data.  See the similar warnings for the built-in [`attachInterrupt`](https://www.arduino.cc/en/Reference/AttachInterrupt) for digital pins.

The registered procedure will be called from the ISR whenever a character is received.  The received character **will not** be stored in the `rx_buffer`, and it **will not** be returned from `read()`.  Any characters that were received and buffered before `attachInterrupt` was called remain in `rx_buffer`, and could be retrieved by calling `read()`.

If `attachInterrupt` is never called, or it is passed a `NULL` procedure, the normal buffering occurs, and all received characters must be obtained by calling `read()`.

The original `HardwareSerial` files were modified to include two new methods, `attachInterrupt` and `detachInterrupt`, and one new data member, the private `_isr`:

```
    typedef void (* isr_t)( uint8_t );
    void attachInterrupt( isr_t fn = (isr_t) NULL );
    void detachInterrupt() { attachInterrupt( (isr_t) NULL ); };

  private:
    isr_t  _isr;
```

All `serialEvent` code has been removed, as this modification is intended to solve polling-related problems.  `serialEvent` uses a polling technique.

As new Arduino IDEs are released, new versions will appear in the root of this repository, named with the Arduino IDE version number.
