#pragma once

#include "fractal.h"
#include <functional>
#include <future>
#include <complex>

class EscapeTimeFractal : public Fractal
{
protected:
  template <typename SetupFunc, typename ColorFunc>
  void renderEngine(sf::Image &image, int width, int height, const ViewState &viewState, SetupFunc setupLogic, ColorFunc colorLogic)
  {
    unsigned int threadCount = std::thread::hardware_concurrency();

    if (threadCount == 0)
      threadCount = 4;

    std::vector<std::future<void>> tasks;

    int rowsPerThread = height / threadCount;

    for (unsigned int t = 0; t < threadCount; ++t)
    {
      int startY = t * rowsPerThread;
      int endY = (t == threadCount - 1) ? height : startY + rowsPerThread;
      const auto task = [=, &image]()
      {
        for (int y = 0; y < endY; ++y)
        {
          double mapY = map(y, 0, height, viewState.minIm, viewState.maxIm);
          for (int x = 0; x < width; ++x)
          {
            double mapX = map(x, 0, width, viewState.minRe, viewState.maxRe);

            std::complex<double> mappedPos(mapX, mapY);
            std::complex<double> z, c;

            setupLogic(z, c, mappedPos);

            int iter = 0;

            while (iter < viewState.maxIter && std::norm(z) < 4.0)
            {
              z = z * z + c;
              iter++;
            }

            if (iter == viewState.maxIter)
            {
              image.setPixel(sf::Vector2u(x, y), sf::Color::Black);
            }
            else
            {
              sf::Color color = colorLogic(iter, viewState.maxIter);

              image.setPixel(sf::Vector2u(x, y), color);
            }
          }
        }
      };

      tasks.emplace_back(std::async(std::launch::async, task));
    }
    for (auto &task : tasks)
    {
      task.get();
    }
  }
};