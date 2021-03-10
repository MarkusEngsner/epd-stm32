#ifndef CPPFROMBLINKY_FONT_H
#define CPPFROMBLINKY_FONT_H

#include <canvas.h>

#include <array>

namespace paintbrush {

template <unsigned sym_w, unsigned sym_h, uint8_t first_symbol, uint8_t length>
class Font {
 public:
  constexpr explicit Font(std::array<Canvas<sym_w, sym_h>, length> arr)
      : table(arr) {}

  Canvas<sym_w, sym_h> operator()(uint8_t ascii) const {
    return table[ascii - first_symbol];
  }
  Canvas<sym_w, sym_h> symbol(uint8_t ascii) const {
    return table[ascii - first_symbol];
  }

 private:
  std::array<Canvas<sym_w, sym_h>, length> table;
};

}  // namespace paintbrush
#endif  // CPPFROMBLINKY_FONT_H
