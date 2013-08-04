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
#ifndef KORTEX_MATH_H
#define KORTEX_MATH_H

namespace kortex {

    inline float  sq(const float&  v) { return v*v; }
    inline double sq(const double& v) { return v*v; }
    inline int    sq(const int&    v) { return v*v; }

    double dot ( const double * a, const double* b, int asz );


}

#endif
