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
#ifndef KORTEX_IMAGE_PROCESSING_H
#define KORTEX_IMAGE_PROCESSING_H

namespace kortex {


    template <typename T>
    float bilinear_interpolation(const T* img, const int& w, const int& h, const int& nc, const int& c,  const float& x, const float& y);

    template <typename T>
    float  bicubic_interpolation(const T* im,  const int& w, const int& h, const int& nc, const int& ch, const float& x, const float& y);


}

#endif
