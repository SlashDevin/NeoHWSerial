The **NeoHWSerial** class is a drop-in replacement for the Arduino built-in class `HardwareSerial`.  It adds the capability to handle received characters with a user-defined function *during* the RX interrupt.  This can improve performance significantly by eliminating all processing associated with storing and retrieving characters in the ring buffer **and** all polling code that constantly checked for new characters: `available` and `read` are unnecessary.

### Installation

1. Download the NeoHWSerial master zip file.
2. Extract all files into a tempory working directory.
3. Create a `NeoHWSerial` subdirectory in your `Arduino/Libraries` directory.
4. Copy all files from the version number subdirectory that corresponds to your IDE version into the `NeoHWSerial` subdirectory you created in step 1.  For example, if you are using Arduino IDE version 1.0.5, copy all files from the extracted `NeoHWSerial-master/1.0.5` subdirectory into your `Arduino/Libraries/NeoHWSerial` subdirectory.

### Usage

To handle all received characters with your function, you must register it with the specific `NeoSerial[n]` instance:

    #include <NeoHWSerial.h>
    
    volatile uint32_t newlines = 0UL;
    
    static void handleRxChar( uint8_t c )
    {
      if (c == '\n')
        newlines++;
    }
    
    void setup()
    {
      NeoSerial1.attachInterrupt( handleRxChar );
      NeoSerial1.begin( 9600 ); // Instead of 'Serial1'
    }

Remember that the registered function is called from an interrupt context, and it should return as quickly as possible.  Taking too much time in the function will cause many unpredictable behaviors, including loss of received data.  See the similar warnings for the built-in [`attachInterrupt`](https://www.arduino.cc/en/Reference/AttachInterrupt) for digital pins.

The registered function will be called from the ISR whenever a character is received.  The received character **will not** be stored in the `rx_buffer`, and it **will not** be returned from `read()`.  Any characters that were received and buffered before `attachInterrupt` was called remain in `rx_buffer`, and could be retrieved by calling `read()`.

If `attachInterrupt` is never called, or it is passed a `NULL` function, the normal buffering occurs, and all received characters must be obtained by calling `read()`.

The original `HardwareSerial` files were modified to include two new methods, `attachInterrupt` and `detachInterrupt`, and one new data member, the private `_isr`:

```
    typedef void (* isr_t)( uint8_t );
    void attachInterrupt( isr_t fn );
    void detachInterrupt() { attachInterrupt( (isr_t) NULL ); };

  private:
    isr_t  _isr;
```

### NOTES

To avoid name collisions, all `HardwareSerial` instances are prefixed with "Neo" in this replacement library.  All parts of your sketch, including other libraries, must use

*  `NeoSerial` instead of `Serial`, 
*  `NeoSerial1` instead of `Serial1`,
*  `NeoSerial2` instead of `Serial2`, and 
*  `NeoSerial3` instead of `Serial3`.

If there are any references to the original `HardwareSerial` instances, you will get one or more linker errors:

    core.a(HardwareSerial.cpp.o): In function `__vector_18':
    <Arduino Dir>\hardware\arduino\cores\arduino/HardwareSerial.cpp:115: multiple definition of `__vector_18'
    NeoHWSerial\NeoHWSerial.cpp.o: <Libraries Dir>\NeoHWSerial/NeoHWSerial.cpp:116: first defined here
    core.a(HardwareSerial.cpp.o): In function `serialEvent()':
    <Arduino Dir>\hardware\arduino\cores\arduino/HardwareSerial.cpp:112: multiple definition of `rx_buffer'
    NeoHWSerial\NeoHWSerial.cpp.o: <Libraries Dir>\NeoHWSerial/NeoHWSerial.cpp:90: first defined here

If you see this error, some part of your code is still using `Serial`, `Serial1`, `Serial2` or `Serial3`.  Be sure to check all your libraries.

All `serialEvent` code has been removed, as this modification is intended to solve polling-related problems.  `serialEvent` uses a polling technique.

As new Arduino IDEs are released, new versions will appear in the root of this repository, named with the Arduino IDE version number.

### See Also

If you are also using software serial ports, you may be interested in [NeoICSerial](https://github.com/SlashDevin/NeoICSerial) or [NeoSWSerial](https://github.com/SlashDevin/NeoSWSerial).
