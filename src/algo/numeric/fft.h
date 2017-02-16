#pragma once

#include <complex>
#include <vector>

namespace algo {
using Complex = std::complex<double>;

void Butterfly(const std::vector<Complex> &from, std::vector<Complex> &to);

void FFT(std::vector<Complex> &xs, bool forward);
} // namespace algo
