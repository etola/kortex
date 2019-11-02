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
#ifndef KORTEX_RANDOM_GENERATOR_H
#define KORTEX_RANDOM_GENERATOR_H

#include <random>

namespace kortex {

    class RandomGenerator {
    private:
        unsigned int               seed_value;
        std::mt19937               rv_generator;
        std::normal_distribution<> unit_nrm_distribution;

    public:
        RandomGenerator() {
            seed_value            = 0;
            unit_nrm_distribution = std::normal_distribution<>( 0.0, 1.0 );
            rv_generator.seed( seed_value );

            randomize();
        }

        void set_seed( unsigned int seed_value_ ) {
            seed_value = seed_value_;
            rv_generator.seed( seed_value );
            unit_nrm_distribution.reset();
        }

        void randomize() {
            std::random_device randev;
            seed_value = randev();
            rv_generator.seed( seed_value );
        }

        double normal_sample() {
            return unit_nrm_distribution( rv_generator );
        }

        double uniform_sample() {
            return double( rv_generator() ) / double( rv_generator.max() );
        }

        unsigned int get_seed() const {
            return seed_value;
        }

        uint32_t rv() {
            return uint32_t( rv_generator() );
        }
        uint32_t max() {
            return uint32_t( rv_generator.max() );
        }

    };

    extern RandomGenerator g_random;

    RandomGenerator* random_generator();


}

#endif
