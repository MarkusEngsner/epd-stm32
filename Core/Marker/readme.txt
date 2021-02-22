Marker
------------

### What? ###
This is meant to be a replacement for the Waveshare E-ink libraries

### Why? ###
I found their libraries ugly (global variables), non-mcu-friendly (using malloc and pointers),
and instead wanted to make something with "better" abstractions.

As a longer term goal I would also like to make something that doesn't use the HAL,
and which isn't blocking/polling.