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
#ifndef KORTEX_FILTER_H
#define KORTEX_FILTER_H

namespace kortex {

    void filter_hor(const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);
    void filter_ver(const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);
    void filter_hv (const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);

    void filter_hor_par(const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);
    void filter_ver_par(const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);
    void filter_hv_par (const float* im, const int& w, const int& h, const float* kernel, const int& ksize, float* out);

    inline void filter_hor( float*  im, const int& w, const int& h, const float* kernel, const int& ksize ) {
        filter_hor( im, w, h, kernel, ksize, im );
    }
    inline void filter_ver( float*  im, const int& w, const int& h, const float*  kernel, const int& ksize ) {
        filter_ver( im, w, h, kernel, ksize, im );
    }
    inline void filter_hv( float*  im, const int& w, const int& h, const float*  kernel, const int& ksize ) {
        filter_hv( im, w, h, kernel, ksize, im );
    }
    inline void filter_hor_par(float* im, const int& w, const int& h, const float* kernel, const int& ksize) {
        filter_hor_par(im, w, h, kernel, ksize, im);
    }
    inline void filter_ver_par(float*  im, const int& w, const int& h, const float*  kernel, const int& ksize) {
        filter_ver_par(im, w, h, kernel, ksize, im);
    }
    inline void filter_hv_par(float*  im, const int& w, const int& h, const float*  kernel, const int& ksize) {
        filter_hv_par(im, w, h, kernel, ksize, im);
    }

}

#endif
