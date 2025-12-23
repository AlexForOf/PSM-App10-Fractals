#include <SFML/Graphics.hpp>

#include "fern.h"
#include <iostream>
#include <random>

sf::Image Fern::render(int width, int height, const ViewState &viewState)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height));
 static std::random_device rd;
 static std::mt19937 gen(rd());

 std::uniform_real_distribution<double> randomNum(0, 1.0);

 double x = 0.0, y = 0.0;

 const int dotCount = 100000;

 for (int i = 0; i < dotCount; ++i)
 {
  double nextX, nextY;
  double r = randomNum(gen);

  sf::Color pixelColor;
  if (r < 0.01)
  {
   nextX = x;
   nextY = 0.16 * y;

   pixelColor = sf::Color(139, 69, 19);
  }
  else if (r < 0.86)
  {
   nextX = 0.85 * x + 0.04 * y;
   nextY = -0.04 * x + 0.85 * y + 1.6;

   pixelColor = sf::Color(34, 139, 34);
  }
  else if (r < 0.93)
  {
   nextX = 0.2 * x - 0.26 * y;
   nextY = 0.23 * x + 0.22 * y + 1.6;

   pixelColor = sf::Color(255, 215, 0);
  }
  else
  {
   nextX = -0.15 * x + 0.28 * y;
   nextY = 0.26 * x + 0.24 * y + 0.44;

   pixelColor = sf::Color(255, 69, 0);
  }

  x = nextX;
  y = nextY;

  int px = map(x, viewState.minRe, viewState.maxRe, 0, width);
  int py = map(y, viewState.minIm, viewState.maxIm, height, 0);

  if (pixelInBounds(px, py, width, height))
  {
   image.setPixel(sf::Vector2u(px, py), pixelColor);
  }
 }
 return image;
}