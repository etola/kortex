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
#include <kortex/message.h>
#include "message.tcc"

namespace kortex {

    template void matrix_print(const char* str, const int   * A, int ar, int ac, bool legend, bool print_zeros);
    template void matrix_print(const char* str, const float * A, int ar, int ac, bool legend, bool print_zeros);
    template void matrix_print(const char* str, const double* A, int ar, int ac, bool legend, bool print_zeros);

}
