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
#ifndef KORTEX_TIMER_H
#define KORTEX_TIMER_H

#include <chrono>

namespace kortex {

    class Timer {
    public:
        Timer();
        double elapsed(); // returns the time from the last call to elapsed()
        double end();     // returns the time from the initialization of the timer
        void reset();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> st, en, el;
    };

}

#endif
