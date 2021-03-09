#ifndef CPPFROMBLINKY_DRAWING_H
#define CPPFROMBLINKY_DRAWING_H

#include "canvas.h"
#include "symbol.h"

namespace paintbrush {

enum class Axis { Horizontal, Vertical };
enum class Direction {UpRight, DownRight};

template <unsigned width, unsigned height>
void DrawStraightLine(Canvas<width, height> &canvas, unsigned x,
                                       unsigned y, unsigned length, Axis dir, Color c);

template <unsigned width, unsigned height>
void DrawDiagonalLine(Canvas<width, height> &canvas, unsigned x,
                      unsigned y, unsigned length, Direction dir, Color c);


// A symbol is just a canvas!
template <unsigned width, unsigned height, unsigned sym_w, unsigned sym_h>
void DrawSymbol(Canvas<width, height> &canvas, unsigned target_x, unsigned target_y, Canvas<sym_w, sym_h> symbol,
                Color c);

}  // namespace paintbrush

#endif  // CPPFROMBLINKY_DRAWING_H
