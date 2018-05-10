# rpi-many-ws2812b

Control WS2812B LEDs connected to a Raspberry Pi. This library supports up to 18 channels.

## API usage
Just initiate a `WS2812BOut`. Supply `chainCount` (channel count) and `chainLength` (maximum number of LEDs per channel) in the constructor.
Then, to transmit data, call `operator()` with an `uint8_t**` (array of pointers to RGB buffers).
You need to supply `chainCount` pointers. Each buffer pointed to needs to contain `chainLength` RGB pixels.
WS2812B chains ignore extra pixels at the end.

## Hardware usage
Channels are mapped to (BCM-)GPIOs 8 to 25.
Basically, the Raspberry Pi GPIOs can be connected directly to the signal input of WS2812B.
Of course, LED GND and Raspberry Pi GND need to be connected too.
However, some pages recommend a resistor in the signal line or level shifting 3.3V GPIOs to 5V.

## Framerate / LED count
- Transmitting a bit: 1.2µs
- Time per LED (24-bit RGB): 28.8µs

For example, using all channels, a 500x18 24-bit-RGB display can be controlled with 69.4 fps.
The framerate is limited by the longest chain.
20000 LEDs per channel seem to work (framerate drops as expected, below 2 fps).

## How it works
This library uses the SMI (Secondary Memory Interface) peripheral with 8-bit, 16-bit or 18-bit writes.
RGB data is bit-banged to a buffer and written out to the SMI port via DMA.
Because of the one-wire WS2812B protocol, this provides up to 18 channels.

## How to build and run the demo
(on a Raspberry Pi, repo cloned)
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make -j
$ ./demo1/demo1 100 100 100  # 3 chains with 100 LEDs
```

## License
rpi-many-ws2812b is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

rpi-many-ws2812b is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with rpi-many-ws2812b.  If not, see <http://www.gnu.org/licenses/>.
