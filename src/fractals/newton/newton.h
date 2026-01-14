#pragma once
#include "fractal.h"
#include <complex>
#include <vector>

class Newton : public Fractal
{
public:
 Newton();
 sf::Image render(int width, int height, const ViewState &viewState) override;

private:
 std::vector<std::complex<double>> unityRoots;
 std::vector<sf::Color> rootColors;
};