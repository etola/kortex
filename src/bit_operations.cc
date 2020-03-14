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

#include <kortex/bit_operations.h>
#include <kortex/check.h>

namespace kortex {

	void set_bit( uchar& v, const uint8_t& bit_n, const bool& value  ) {
		assert_boundary( bit_n, 0, 9 );
		if( value ) {
			switch( bit_n ) {
				case 0: v &= ~(1UL << 0); break;
				case 1: v &= ~(1UL << 1); break;
				case 2: v &= ~(1UL << 2); break;
				case 3: v &= ~(1UL << 3); break;
				case 4: v &= ~(1UL << 4); break;
				case 5: v &= ~(1UL << 5); break;
				case 6: v &= ~(1UL << 6); break;
				case 7: v &= ~(1UL << 7); break;
				default: switch_fatality();
			}
		} else {
			switch( bit_n ) {
				case 0: v |= 1UL << 0; break;
				case 1: v |= 1UL << 1; break;
				case 2: v |= 1UL << 2; break;
				case 3: v |= 1UL << 3; break;
				case 4: v |= 1UL << 4; break;
				case 5: v |= 1UL << 5; break;
				case 6: v |= 1UL << 6; break;
				case 7: v |= 1UL << 7; break;
				default: switch_fatality();
			}
		}
	}

	bool get_bit( uchar value, const uint8_t& bit_n ) {
		assert_boundary( bit_n, 0, 9 );
		uint8_t bit = 0;
		switch(bit_n) {
			case 0: bit = (value >> 0) & 1U; break;
			case 1: bit = (value >> 1) & 1U; break;
			case 2: bit = (value >> 2) & 1U; break;
			case 3: bit = (value >> 3) & 1U; break;
			case 4: bit = (value >> 4) & 1U; break;
			case 5: bit = (value >> 5) & 1U; break;
			case 6: bit = (value >> 6) & 1U; break;
			case 7: bit = (value >> 7) & 1U; break;
			default: switch_fatality();
		}
		return (bool)bit;
	}

}
