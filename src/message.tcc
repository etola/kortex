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
using namespace std;

namespace kortex {

    template< typename T >
    void matrix_print(ostream& os, const char* str, const T* A, int ar, int ac, bool legend, bool print_zeros) {
        os << std::setprecision(9);
        os << std::fixed;
        os.fill(' ');

        os << str << " =\n";

        if( legend ) {
            os << "\t\t" << "  ";
            os.setf(std::ios_base::right);
            for( int j = 0; j < ac; j++ ) {
                MessageFormat::apply(os);
                os << j << " ";
            }
            os << std::endl;
            for( int j = 0; j < MessageFormat::line_width; j++ )
                os << '.';
            os << std::endl;
        }

        for( int i = 0; i < ar; i++ ) {
            if( legend ) {
                MessageFormat::apply(os, std::ios_base::right);
                os << i << "\t" << ": ";
            }

            for(int j = 0; j < ac; j++) {
                MessageFormat::apply(os, std::ios_base::right);
                if( A[i*ac+j] != 0 || print_zeros )
                    os << A[i*ac+j] << " ";
                else
                    os << "." << " ";
            }
            os << std::endl;
        }
    }

    template<typename T>
    void matrix_print(const char* str, const T* A, int ar, int ac, bool legend, bool print_zeros) {
        matrix_print(std::cout, str, A, ar, ac, legend, print_zeros);
    }

}
