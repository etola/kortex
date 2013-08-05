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
#ifndef KORTEX_IMAGE_SPACE_MAPPING_H
#define KORTEX_IMAGE_SPACE_MAPPING_H

#include <kortex/image_space_structs.h>

namespace kortex {

    class Image;

    /// maps the image img contents to a linear range [0 255] specified by
    /// [min_v, max_v]; if min_v == max_v == -1, then the img range is used for
    /// detecting the range ; NAN safe
    void image_map_linear(const Image* img, float& min_v, float& max_v, Image* out );

    /// maps the image img contents using a power scale with v=pow(v,pscale);
    /// not very efficient in regards to fixed color space gamma correction;
    /// think of tables. NAN safe;
    void image_power(const Image* img, const float& pscale, Image* out );

    /// maps the image img contents using a log scale with v=log(v); not very
    /// efficient in regards to fixed set of v values. think of tables. NAN
    /// safe;
    void image_log(const Image* img, Image* out );

    /// applies a color mapping regarding ColorMap cm and generates a single
    /// channel uchar image.
    void image_map( const Image* img, const ColorMap& cm, Image* out );


}

#endif
