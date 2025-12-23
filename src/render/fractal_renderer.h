#include <SFML/Graphics.hpp>
#include "fractals/fractal.h"

class FractalRenderer
{
public:
 FractalRenderer(int w, int h);

 void clear(sf::Color color);
 void renderFractal(Fractal &fractal, const ViewState &viewState, int scale);
 void draw(sf::RenderWindow &window, sf::Sprite &sprite);

 int getWidth() const;
 int getHeight() const;
 sf::Sprite getSprite(int scale) const;

private:
 int width, height;
 sf::Texture texture;
};