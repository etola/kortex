// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2015 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_COLOR_MAP_H
#define KORTEX_COLOR_MAP_H

#include <string>
#include <vector>
using std::vector;
using std::string;

namespace kortex {

    extern float g_colormap_hot[192];
    extern float g_colormap_jet[192];
    extern float g_colormap_hsv[192];
    extern float g_colormap_cool[192];
    extern float g_colormap_winter[192];

    struct ColorMap {
        string           m_type;
        vector<string>   m_types;
        const float *    m_arr;
        static const int m_arr_sz = 64;
        float            m_step_size;

        ColorMap();

        int n_types() const;
        void set_type( int tid );
        void set_type( const string& t );
        void print_types() const;

        void get_color( const float& gray, float& red, float& green, float& blue ) const;
    };

}

#endif
