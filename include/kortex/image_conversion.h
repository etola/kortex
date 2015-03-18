// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2013 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_IMAGE_CONVERSION_H
#define KORTEX_IMAGE_CONVERSION_H

#include <kortex/image.h>

namespace kortex {

    void convert_image( const Image* src, Image* dst );
    void convert_image( const Image& src, ImageType type, Image& dst );

}

#endif
