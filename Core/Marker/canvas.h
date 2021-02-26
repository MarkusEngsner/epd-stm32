#ifndef CPPFROMBLINKY_CANVAS_H
#define CPPFROMBLINKY_CANVAS_H

/* Same idea as std::bitset,
 * with the two central differences:
 * 1. The bits are stored as an array of bytes.
 * This is essential for transmitting to the eink screen.
 * 2. The interface outwards is 2D as well as 1D
 */

#include <array>

namespace paintbrush {

enum class Color { Black, White };

// Note! dimensions are in bits
// Currently assumed that 8 divides width
template <unsigned int width_, unsigned int height_>
class Canvas {
 public:
  Canvas() : arr{} {}
  inline bool operator()(unsigned x, unsigned y) const {
    return GetBit(x, y) ? Color::White : Color::Black;
  };
  uint8_t* RawData() { return arr.data(); };
  void Fill(Color c) {
    std::fill(arr.begin(), arr.end(), c == Color::White ? 0xFF : 0x00);
  };

  void Set(unsigned x, unsigned y, Color c) {
    c == Color::White ? Set(x, y) : Reset(x, y);
  }

  // more things to add: iterators, set, reset, flip, function which sets color
  // based on coordinates?
 private:
  // Data is stored row after row
  std::array<uint8_t, width_ * height_ / 8> arr;

  inline unsigned int FlatBitIndex(unsigned x, unsigned y) const { return y * width_ + x; }

  inline unsigned int FlatByteIndex(unsigned x, unsigned y) const {
    return FlatBitIndex(x, y) >> 3;
  }

  // returns the index of the bit within a byte
  inline unsigned int InternalBitIndex(unsigned x, unsigned y) const { return 7 - (x & 0b111); }

  void Set(unsigned int x, unsigned int y) {
    const auto i = FlatByteIndex(x, y);
    if (i > 5000){
      printf("ERROR! i=%u\tx=%u\ty=%u", i, x, y);
    }
    const auto bit = InternalBitIndex(x, y);
    arr[i] |= 1 << bit;
  };

  void Reset(unsigned x, unsigned y) {
    const auto i = FlatByteIndex(x, y);
    const auto bit = InternalBitIndex(x, y);
    arr[i] &= ~(1 << bit);
  };

  bool GetBit(unsigned x, unsigned y) const {
    const auto i = FlatByteIndex(x, y);
    const auto bit = InternalBitIndex(x, y);
    return arr[i] & (1 << bit);
  }
};

}  // namespace paintbrush
#endif  // CPPFROMBLINKY_CANVAS_H
