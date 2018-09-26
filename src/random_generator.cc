// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2018 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------

#include <kortex/random_generator.h>

namespace kortex {

    RandomGenerator g_random;

    RandomGenerator* random_generator() { return &g_random; }

}
