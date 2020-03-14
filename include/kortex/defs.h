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
#ifndef KORTEX_DEFS_H
#define KORTEX_DEFS_H

#define AC_SAFETY_BEGIN_NUM  0x5555
#define AC_SAFETY_END_NUM    0xAAAA
#define MAX_IMAGE_DIM        16384
#define MAX_IMAGE_NO         16384
#define MAX_ARR_SIZE         16384
#define NRM_EPS              1e-16

#ifdef __GNUC__
#define BYTE_ALIGNED_16 __attribute__ ((__aligned__ (16)))
#else
#define BYTE_ALIGNED_16
#endif

#if defined(_WIN32) || defined(_WIN64)
#define WND_BYTE_ALIGNED_16 __declspec(align(16))
#else
#define WND_BYTE_ALIGNED_16
#endif

namespace kortex {

	static const long   MINUTE    = 60;    // seconds
	static const long   HOUR      = 3600;  // seconds
	static const long   DAY       = 86400; // seconds

	static const double KB        = 1024.0;            // 1024 ^ 1
	static const double MB        = 1048576.0;         // 1024 ^ 2
	static const double GB        = 1073741824.0;      // 1024 ^ 3

	static const double PI       = 3.14159265358979311599796346854418516159057617187500000; // pi
	static const double PI2      = 6.28318530717958623199592693708837032318115234375000000; // 2*pi
	static const double PI_2     = 1.57079632679489655799898173427209258079528808593750000; // pi/2
	static const double PI_3     = 1.04719755119659763131778618117095902562141418457031250; // pi/3
	static const double PI_4     = 0.78539816339744827899949086713604629039764404296875000; // pi/4
	static const double PI_18    = 0.17453292519943294780482290207146434113383293151855469; // pi/18
	static const double PI_34    = 2.35619449019234483699847260140813887119293212890625000; // 3*pi/4
	static const double SQRTPI   = 1.77245385090551588191942755656782537698745727539062500; // sqrt(pi);
	static const double SQRTPI2  = 2.50662827463100024161235523934010416269302368164062500; // sqrt(2*pi)
	static const double ISQRTPI2 = 0.39894228040143270286321808271168265491724014282226562; // 1/sqrt(2*pi)
	static const double RADIANS  = 0.01745329251994329547437168059786927187815308570861816; // pi/180
	static const double DEGREES  = 57.2957795130823228646477218717336654663085937500000000; // 180/pi
	static const double LOG2     = 0.69314718055994528622676398299518041312694549560546875; // ln(2.0)
	static const double SQRT2    = 1.41421356237309514547462185873882845044136047363281250; // sqrt(2.0)
	static const double ISQRT2   = 0.70710678118654746171500846685376018285751342773437500; // 1/sqrt(2.0)

	enum DIRECTION {
		NORTH_WEST = 0,
		NORTH      = 1,
		NORTH_EAST = 2,
		WEST       = 3,
		EAST       = 4,
		SOUTH_WEST = 5,
		SOUTH      = 6,
		SOUTH_EAST = 7,
		CENTER     = 8
	};

	inline void get_direction_vector( const DIRECTION& dir, int& dx, int& dy ) {
		switch( dir ) {
			case NORTH_WEST: dx = -1; dy = -1; break;
			case NORTH     : dx =  0; dy = -1; break;
			case NORTH_EAST: dx =  1; dy = -1; break;
			case WEST      : dx = -1; dy =  0; break;
			case EAST      : dx = +1; dy =  0; break;
			case SOUTH_WEST: dx = -1; dy =  1; break;
			case SOUTH     : dx =  0; dy =  1; break;
			case SOUTH_EAST: dx =  1; dy =  1; break;
			case CENTER    : dx =  0; dy =  0; break;
		}
	}

}

#endif
