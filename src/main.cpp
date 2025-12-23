#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <future>

#include "fractal.h"
#include "mandelbrot.h"
#include "fern.h"
#include "julia.h"
#include "fractal_renderer.h"

static const int WINDOW_WIDTH = 1280;
static const int WINDOW_HEIGHT = 720;

enum FractalType
{
  MANDELBROT,
  FERN,
  JULIA
};

struct AppState
{
  bool needsRedraw = true;
  double zoom = 1.0;
  double offset_x = -0.75;
  double offset_y = 0.0;

  double initial_zoom = zoom;
  double initial_offset_x = offset_x;
  double initial_offset_y = 0.0;

  const double moveSpeed = 0.1;
  int currentScale = 1;

  float scrollDelta = 0.0f;

  Mandelbrot mandelbrotFractal;
  Fern fernFractal;
  Julia juliaFractal;
  Fractal *selectedFractal = &mandelbrotFractal;
  FractalType selectedFractalType = FractalType::MANDELBROT;

  ViewState getViewState(int winWidth, int winHeight) const
  {
    double aspect_ratio = (double)winWidth / winHeight;
    double viewHeight = 2.0 / zoom;
    double viewWidth = viewHeight * aspect_ratio;

    return {
        offset_x - (viewWidth / 2.0),
        offset_x + (viewWidth / 2.0),
        offset_y - (viewHeight / 2.0),
        offset_y + (viewHeight / 2.0),
        100};
  }

  void setSelectedFractal(FractalType fractal)
  {
    selectedFractalType = fractal;
    switch (fractal)
    {
    case FractalType::FERN:
      selectedFractal = &fernFractal;
      break;
    case FractalType::MANDELBROT:
      selectedFractal = &mandelbrotFractal;
      break;
    case FractalType::JULIA:
      selectedFractal = &juliaFractal;
      break;
    }
  }

  int getRenderWidth(int windowWidth) const
  {
    return windowWidth / currentScale;
  }

  int getRenderHeight(int windowHeight) const
  {
    return windowHeight / currentScale;
  }
};

void handleInput(const sf::Event &event, sf::RenderWindow &window, AppState &state)
{
  if (event.is<sf::Event::Closed>())
  {
    window.close();
  }

  if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>())
  {
    if (keyEvent->code == sf::Keyboard::Key::Space)
    {
      state.offset_x = state.initial_offset_x;
      state.offset_y = state.initial_offset_y;
      state.zoom = state.initial_zoom;
      state.needsRedraw = true;
    }

    if (keyEvent->code == sf::Keyboard::Key::Num1)
    {
      state.offset_x = -0.75;
      state.initial_offset_x = -0.75;
      state.offset_y = 0.0;
      state.initial_offset_y = 0.0;
      state.zoom = 1.0;
      state.initial_zoom = 1.0;

      state.setSelectedFractal(FractalType::MANDELBROT);
      state.needsRedraw = true;

      window.setTitle("Fractals - Mandelbrot");
    }

    if (keyEvent->code == sf::Keyboard::Key::Num2)
    {
      state.offset_x = 0.0;
      state.initial_offset_x = state.offset_x;
      state.offset_y = 5.0;
      state.initial_offset_y = state.offset_y;
      state.zoom = 0.1;
      state.initial_zoom = state.zoom;

      state.setSelectedFractal(FractalType::FERN);
      state.needsRedraw = true;

      window.setTitle("Fractals - Barnsley Fern");
    }

    if (keyEvent->code == sf::Keyboard::Key::Num3)
    {
      state.offset_x = 0.0;
      state.initial_offset_x = state.offset_x;
      state.offset_y = 0.0;
      state.initial_offset_y = state.offset_y;
      state.zoom = 0.8;
      state.initial_zoom = state.zoom;

      state.setSelectedFractal(FractalType::JULIA);
      state.needsRedraw = true;

      window.setTitle("Fractals - Julia. Hold Left Mouse and move around");
    }
  }

  if (const auto *wheelEvent = event.getIf<sf::Event::MouseWheelScrolled>())
  {
    state.scrollDelta += wheelEvent->delta;
  }
}

bool handleContinuousInput(AppState &state, sf::RenderWindow &window)
{
  double currentSpeed = state.moveSpeed / state.zoom;
  bool moved = false;

  if (state.scrollDelta != 0.0f)
  {
    if (state.scrollDelta > 0.0f)
      state.zoom *= 1.1;
    else
      state.zoom *= 0.9;

    state.scrollDelta = 0.0f;

    moved = true;
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
  {
    state.offset_x += currentSpeed;
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
  {
    state.offset_x -= currentSpeed;
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
  {
    state.offset_y -= currentSpeed;
    moved = true;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
  {
    state.offset_y += currentSpeed;
    moved = true;
  }

  if (state.selectedFractalType == FractalType::JULIA && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
  {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2u windowSize = window.getSize();

    if (mousePos.x >= 0 && mousePos.x <= (int)windowSize.x && mousePos.y >= 0 && mousePos.y <= (int)windowSize.y)
    {
      double seedRe = ((double)mousePos.x / windowSize.x) * 2.0 - 1.0;
      double seedIm = ((double)mousePos.y / windowSize.y) * 2.0 - 1.0;

      state.juliaFractal.setSeed(seedRe, seedIm);

      moved = true;
    }
  }

  if (moved)
    state.needsRedraw = true;

  return moved;
}

int main()
{
  sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Fractals - Mandelbrot");
  window.setFramerateLimit(30);

  FractalRenderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);

  AppState state;

  renderer.clear(sf::Color::Black);

  int lastScale = 1;

  while (window.isOpen())
  {
    state.currentScale = 1;
    bool isInteracting = false;
    while (const std::optional event = window.pollEvent())
    {
      handleInput(*event, window, state);
    }

    if (handleContinuousInput(state, window))
    {
      isInteracting = true;
    }

    if (isInteracting)
    {
      state.currentScale = 4;
    }

    if (state.currentScale != lastScale)
    {
      state.needsRedraw = true;
    }

    if (state.needsRedraw)
    {
      ViewState vs = state.getViewState(WINDOW_WIDTH, WINDOW_HEIGHT);

      renderer.renderFractal(*state.selectedFractal, vs, state.currentScale);
      state.needsRedraw = false;
      lastScale = state.currentScale;
    }

    window.clear();

    sf::Sprite sprite = renderer.getSprite(state.currentScale);
    renderer.draw(window, sprite);

    window.display();
  }

  return 0;
}