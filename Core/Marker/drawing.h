#ifndef CPPFROMBLINKY_DRAWING_H
#define CPPFROMBLINKY_DRAWING_H

namespace paintbrush {

enum class Axis { Horizontal, Vertical };
enum class Direction {UpRight, DownRight};

template <unsigned width, unsigned height>
void DrawStraightLine(Canvas<width, height> canvas, unsigned x,
                                       unsigned y, unsigned length, Axis dir, Color c);

template <unsigned width, unsigned height>
void DrawDiagonalLine(Canvas<width, height> canvas, unsigned x,
                      unsigned y, unsigned length, Direction dir, Color c);


}  // namespace paintbrush

#endif  // CPPFROMBLINKY_DRAWING_H
