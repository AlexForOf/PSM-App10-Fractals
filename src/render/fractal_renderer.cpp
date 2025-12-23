#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <complex>

#include "fractal_renderer.h"

FractalRenderer::FractalRenderer(int w, int h) : width(w), height(h)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height), sf::Color::Black);

 bool isLoaded = texture.loadFromImage(image);

 if (!isLoaded)
 {
  std::cout << "Loading new texture in constructor failed" << std::endl;
 }
}

void FractalRenderer::clear(sf::Color color)
{
 sf::Image image = sf::Image(sf::Vector2u(width, height), color);

 bool isLoaded = texture.loadFromImage(image);
 if (!isLoaded)
 {
  std::cout << "Clearing texture failed" << std::endl;
 }
}

void FractalRenderer::renderFractal(Fractal &fractal, const ViewState &viewState, int scale)
{
 int renderWidth = width / scale;
 int renderHeight = height / scale;

 sf::Image image = fractal.render(renderWidth, renderHeight, viewState);

 bool isLoaded = texture.loadFromImage(image);

 if (!isLoaded)
 {
  std::cout << "Render of Fractal Texture failed" << std::endl;
 }
}

void FractalRenderer::draw(sf::RenderWindow &window, sf::Sprite &sprite)
{
 window.draw(sprite);
}

int FractalRenderer::getWidth() const
{
 return width;
}

int FractalRenderer::getHeight() const
{
 return height;
}

sf::Sprite FractalRenderer::getSprite(int scale) const
{
 sf::Sprite sprite(texture);

 sprite.setScale(sf::Vector2f((float)scale, (float)scale));

 return sprite;
}