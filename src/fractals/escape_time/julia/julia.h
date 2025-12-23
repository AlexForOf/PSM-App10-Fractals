#include "fractal.h"
#include "escape_time_fractal.h"
#include <complex>

class Julia : public EscapeTimeFractal
{
public:
 Julia();

 sf::Image render(int width, int height, const ViewState &viewState) override;

 void setSeed(double real, double imag);

private:
 std::complex<double> seed;
};