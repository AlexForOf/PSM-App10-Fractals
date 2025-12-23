#include <SFML/Graphics.hpp>

#include "mandelbrot.h"
#include <iostream>
#include <functional>

#include <complex>

sf::Image Mandelbrot::render(int width, int height, const ViewState &viewState)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height));

 const auto setupFunc = [](std::complex<double> &z, std::complex<double> &c, std::complex<double> &mappedPos)
 {
  z = 0;
  c = mappedPos;
 };

 const auto colorFunc = [](int iter, int maxIter)
 {
  if (iter == maxIter)
   return sf::Color::Black;

  int r = (iter * 5) % 255;
  int g = (iter * 2) % 255;
  return sf::Color(r, g, 0);
 };

 renderEngine(image, width, height, viewState, setupFunc, colorFunc);
 return image;
}