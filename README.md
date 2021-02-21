# Waveshare Library
Changes made:

* conditional `extern 'C' ` added to headers called directly from C++ code
* capitalization issues in `#include`s fixed
* `#include <stdio.h>` added in a couple of places to 
  get rid of warnings about `printf`