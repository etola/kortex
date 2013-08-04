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

#include <kortex/math.h>

namespace kortex {

    double dot( const double * a, const double* b, int asz ) {
        double s = 0.0;
        for( int i=0; i<asz; i++ )
            s += a[i]*b[i];
        return s;
    }


}
