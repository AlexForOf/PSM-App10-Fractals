#include <SFML/Graphics.hpp>

#include "julia.h"
#include <iostream>
#include <complex>
#include <functional>

Julia::Julia()
{
 seed = std::complex<double>(-0.8, 0.156);
}

void Julia::setSeed(double real, double imag)
{
 seed = std::complex<double>(real, imag);
}

sf::Image Julia::render(int width, int height, const ViewState &viewState)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height));

 const auto setupFunc = [this](std::complex<double> &z, std::complex<double> &c, std::complex<double> mappedPos)
 {
  z = mappedPos;
  c = this->seed;
 };

 const auto colorFunc = [](int iter, int maxIter) -> sf::Color
 {
  if (iter == maxIter)
   return sf::Color::Black;

  int b = (iter * 10) % 255;

  return sf::Color(0, b / 2, b);
 };

 renderEngine(image, width, height, viewState, setupFunc, colorFunc);

 return image;
}
