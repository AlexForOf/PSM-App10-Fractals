#pragma once

#include <SFML/Graphics.hpp>

struct ViewState
{
 double minRe, maxRe, minIm, maxIm;
 int maxIter;
};

class Fractal
{
public:
 virtual ~Fractal() = default;

 virtual sf::Image render(int width, int height, const ViewState &viewState) = 0;

protected:
 double map(double input, double inMin, double inMax, double outMin, double outMax)
 {
  double percentage = (input - inMin) / (inMax - inMin);

  return outMin + (outMax - outMin) * percentage;
 }
 bool pixelInBounds(int px, int py, int width, int height)
 {
  return px >= 0 && px < width && py >= 0 && py < height;
 }
};
