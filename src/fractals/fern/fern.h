#include "fractal.h"

class Fern : public Fractal
{
public:
 sf::Image render(int width, int height, const ViewState &viewState) override;
};