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
#ifndef KORTEX_IMAGE_IO_H
#define KORTEX_IMAGE_IO_H

#include <string>
using std::string;

namespace kortex {

    class Image;

    void save_image( const string& file, const Image* img );
    void load_image( const string& file,       Image* img );

    void read_image_size( const string& file, int& w, int& h, int& nc );


}

#endif
