#include "drawing.h"

namespace paintbrush {

template <unsigned int width, unsigned int height>
void DrawStraightLine(Canvas<width, height> &canvas, unsigned int x,
                                       unsigned int y, unsigned int length,
                                       Axis axis, Color c) {
  if (axis == Axis::Horizontal){
    // the horizontal version could be implemented a lot more efficently
    // by using the fact that most segments are stored within a single byte
    for (unsigned i = 0; i < length; i++){
      canvas.Set(x + i, y, c);
    }
  } else {
    for (unsigned i = 0; i < length; i++){
      canvas.Set(x, y + i, c);
    }
  }
}
template <unsigned int width, unsigned int height>
void DrawDiagonalLine(Canvas<width, height> &canvas, unsigned int x, unsigned int y,
                      unsigned int length, Direction dir, Color c) {
  if (dir == Direction::DownRight){
    for (unsigned i = 0; i < length; ++i) {
      canvas.Set(x + i, y + i, c);
    }
  } else {
    for (unsigned i = 0; i < length; ++i) {
      canvas.Set(x - i, y - i, c);
    }

  }

}
template <unsigned int width, unsigned int height, unsigned int sym_w,
          unsigned int sym_h>
void DrawSymbol(Canvas<width, height> &canvas, unsigned int target_x,
                unsigned int target_y, Canvas<sym_w, sym_h> symbol, Color c) {
  for (auto x = 0; x < sym_w; x++){
    for (auto y = 0; y < sym_w; y++){
      // todo: reimplement as member function for efficiency
      // todo: add support for inverted coloring?
      const Color color = symbol(x, y);
      canvas.Set(target_x + x, target_y + y, color);
    }

  }

}

}  // namespace paintbrush