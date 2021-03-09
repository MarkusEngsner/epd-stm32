
#include "drawing.cpp"
template void paintbrush::DrawStraightLine<128u, 296u>(
    paintbrush::Canvas<128u, 296u>&, unsigned int, unsigned int, unsigned int,
    paintbrush::Axis, paintbrush::Color);
template void paintbrush::DrawDiagonalLine<128u, 296u>(
    paintbrush::Canvas<128u, 296u>&, unsigned int, unsigned int, unsigned int,
    paintbrush::Direction, paintbrush::Color);

template void paintbrush::DrawSymbol<128u, 296u>(Canvas<128u, 296u> &canvas,
                                     unsigned int target_x,
                                     unsigned int target_y,
                                     Canvas<24, 24> symbol, Color c);