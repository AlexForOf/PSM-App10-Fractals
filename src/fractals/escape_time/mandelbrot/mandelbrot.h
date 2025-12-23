
#include "escape_time_fractal.h"

class Mandelbrot : public EscapeTimeFractal
{
public:
 sf::Image render(int width, int height, const ViewState &viewState) override;
};