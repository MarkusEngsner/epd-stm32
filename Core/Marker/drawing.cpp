#include "drawing.h"

namespace paintbrush {

template <unsigned int width, unsigned int height>
void DrawStraightLine(Canvas<width, height> canvas, unsigned int x,
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
void DrawDiagonalLine(Canvas<width, height> canvas, unsigned int x, unsigned int y,
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

}  // namespace paintbrush