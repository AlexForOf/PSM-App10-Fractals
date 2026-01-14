#include "newton/newton.h"
#include <cmath>

Newton::Newton()
{
 unityRoots.push_back(std::complex<double>(1.0, 0.0));
 unityRoots.push_back(std::complex<double>(-0.5, std::sqrt(3.0) / 2.0));
 unityRoots.push_back(std::complex<double>(-0.5, -std::sqrt(3.0) / 2.0));

 rootColors.push_back(sf::Color::Red);
 rootColors.push_back(sf::Color::Green);
 rootColors.push_back(sf::Color::Blue);
}

sf::Image Newton::render(int width, int height, const ViewState &viewState)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height));
 int maxIter = 60;

 for (int y = 0; y < height; ++y)
 {
  double mapY = map(y, 0, height, viewState.minIm, viewState.maxIm);

  for (int x = 0; x < width; ++x)
  {
   double mapX = map(x, 0, width, viewState.minRe, viewState.maxRe);

   std::complex<double> z(mapX, mapY);

   int iterations = 0;
   int foundRoot = -1;

   for (int i = 0; i < maxIter; ++i)
   {
    if (std::abs(z) < 0.000001 || foundRoot > -1)
     break;

    std::complex<double> z2 = z * z;
    z = (2.0 * z * z2 + 1.0) / (3.0 * z2);

    double tolerance = 0.001;

    if (std::abs(z - unityRoots[0]) < tolerance)
    {
     foundRoot = 0;
     iterations = i;
    }
    if (std::abs(z - unityRoots[1]) < tolerance)
    {
     foundRoot = 1;
     iterations = i;
    }
    if (std::abs(z - unityRoots[2]) < tolerance)
    {
     foundRoot = 2;
     iterations = i;
    }
   }

   if (foundRoot != -1)
   {
    sf::Color color = rootColors[foundRoot];

    float shadeFactor = 1.0f - (float)iterations / 30.0f;
    if (shadeFactor < 0.1f)
     shadeFactor = 0.1f;

    color.r = static_cast<uint8_t>(color.r * shadeFactor);
    color.g = static_cast<uint8_t>(color.g * shadeFactor);
    color.b = static_cast<uint8_t>(color.b * shadeFactor);

    image.setPixel(sf::Vector2u(x, y), color);
   }
   else
   {
    image.setPixel(sf::Vector2u(x, y), sf::Color::Black);
   }
  }
 }
 return image;
}