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
#ifndef KORTEX_EIGEN_CONVERSION_H
#define KORTEX_EIGEN_CONVERSION_H

#ifdef WITH_EIGEN

#define EIGEN_DEFAULT_TO_ROW_MAJOR
#include <Eigen/Dense>
#include <Eigen/SVD>

#include <kortex/kmatrix.h>

namespace kortex {

    void convert_mat( const Eigen::MatrixXd& m, kortex::KMatrix& km );
    void convert_mat( const kortex::KMatrix& km, Eigen::MatrixXd& m );
    void convert_mat( const double* A, int nr, int nc, int nld, Eigen::MatrixXd& m );
    void convert_mat( const Eigen::MatrixXd& m, double* A, int asz );

}

#endif

#endif
