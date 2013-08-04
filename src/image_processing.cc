// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See licence.txt file for licence information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
//
// ---------------------------------------------------------------------------

#include <kortex/image_processing.h>
#include <kortex/types.h>

#include "image_processing.tcc"

namespace kortex {

    template float bilinear_interpolation(const uchar* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);
    template float bilinear_interpolation(const float* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);
    template float  bicubic_interpolation(const uchar* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);
    template float  bicubic_interpolation(const float* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);

}
