// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2014 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
//
// ---------------------------------------------------------------------------

#include <kortex/timer.h>

namespace kortex {

    Timer::Timer() {
        st = std::chrono::high_resolution_clock::now();
        el = st;
    }
    double Timer::elapsed() {
        en = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = en-el;
        el = en;
        return elapsed_seconds.count();
    }
    double Timer::end() {
        en = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = en-st;
        return elapsed_seconds.count();
    }
    void Timer::reset() {
        st = std::chrono::high_resolution_clock::now();
    }

}
