# About the project
At the moment, this repo is a lot of things.  
Primarily, it is an alternative API for interacting with
Waveshare's ePaper Displays (only tested with the 2.9" one).

Currently, this library has twice the transmission
speed of the Waveshare one:

Library | Init time | Buffer transmission time
--- | --- | ---
Waveshare       | 420 μs      | 56 ms
This project    | 340 μs   | 28 ms


_Tests performed on a Nucleo F410RB, measurements done on a noname Logic Analyzer with Sigrok._

The init time is of course not all that interesting,
since the transmission time completely dwarfs it,
and is performed a lot more often.

## Roadmap 
- [x] Support for sending and printing data to the screen
- [ ] Support partial screen updates
  - [ ] Only transmitting needed data
- [ ] Proper interface for drawing things on screen
  - [ ] Straight lines
  - [ ] Not-straight lines
  - [ ] Circles
  - [ ] Text
  - [ ] Graphs & Diagrams
- [ ] API to send data from a PC
- [ ] Non-blocking SPI communication with the EPD