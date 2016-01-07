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

#include <cmath>

#include <kortex/color.h>
#include <kortex/string.h>

namespace kortex {

    void hsv_to_rgb( float h, float s, float v, float &r, float &g, float &b ) {
        if( s == 0.0f ) {
            r = g = b = v;
            return;
        }
        h /= 60.0f;
        int   i = (int)floor(h);
        float f = h - i;
        float p = v * ( 1 - s );
        float q = v * ( 1 - s * f );
        float t = v * ( 1 - s * ( 1 - f ) );

        switch( i ) {
        case  0: r = v; g = t; b = p; break;
        case  1: r = q; g = v; b = p; break;
        case  2: r = p; g = v; b = t; break;
        case  3: r = p; g = q; b = v; break;
        case  4: r = t; g = p; b = v; break;
        default: r = v; g = p; b = q; break;
        }
    }

    void get_color(ColorName color, uchar &cr, uchar &cg, uchar &cb) {
        switch( color ) {
        case COLOR_BLACK    : cr =   0; cg =   0; cb =   0; break;
        case COLOR_WHITE    : cr = 255; cg = 255; cb = 255; break;
        case COLOR_RED      : cr = 255; cg =   0; cb =   0; break;
        case COLOR_GREEN    : cr =   0; cg = 255; cb =   0; break;
        case COLOR_BLUE     : cr =   0; cg =   0; cb = 255; break;
        case COLOR_CYAN     : cr =   0; cg = 255; cb = 255; break;
        case COLOR_YELLOW   : cr = 255; cg = 255; cb =   0; break;
        case COLOR_MAGENTA  : cr = 255; cg =   0; cb = 255; break;
        case COLOR_ORANGE   : cr = 255; cg = 165; cb =   0; break;
        case COLOR_PURPLE   : cr = 128; cg =   0; cb = 128; break;
        case COLOR_PINK     : cr = 255; cg = 192; cb = 203; break;
        case COLOR_BROWN    : cr = 136; cg =  69; cb =  19; break;
        case COLOR_GOLD     : cr = 255; cg = 215; cb =   0; break;
        case COLOR_SILVER   : cr = 190; cg = 190; cb = 190; break;
        case COLOR_GRAY     : cr = 128; cg = 128; cb = 128; break;
        case COLOR_LRED     : cr = 255; cg =  51; cb =  51; break;
        case COLOR_LGREEN   : cr =  51; cg = 255; cb =  51; break;
        case COLOR_LBLUE    : cr =  51; cg =  51; cb = 255; break;
        case COLOR_LCYAN    : cr =  51; cg = 255; cb = 255; break;
        case COLOR_LMAGENTA : cr = 255; cg =  51; cb = 255; break;
        case COLOR_LYELLOW  : cr = 255; cg = 255; cb =  51; break;
        case COLOR_LORANGE  : cr = 255; cg = 153; cb =  51; break;
        case COLOR_LPURPLE  : cr = 178; cg = 102; cb = 255; break;
        case COLOR_LPINK    : cr = 255; cg = 102; cb = 255; break;
        case COLOR_LBROWN   : cr = 205; cg = 133; cb =  63; break;
        case COLOR_LGOLD    : cr = 250; cg = 250; cb = 210; break;
        case COLOR_LGRAY    : cr =  90; cg =  90; cb =  90; break;
        default             : cr = 255; cg =   0; cb =   0; break;
        }
    }

    ColorName get_color( const string& str ) {

        if( !compare_string_nc(str, "black"         ) ) return  COLOR_BLACK    ;
        if( !compare_string_nc(str, "white"         ) ) return  COLOR_WHITE    ;
        if( !compare_string_nc(str, "red"           ) ) return  COLOR_RED      ;
        if( !compare_string_nc(str, "green"         ) ) return  COLOR_GREEN    ;
        if( !compare_string_nc(str, "blue"          ) ) return  COLOR_BLUE     ;
        if( !compare_string_nc(str, "cyan"          ) ) return  COLOR_CYAN     ;
        if( !compare_string_nc(str, "yellow"        ) ) return  COLOR_YELLOW   ;
        if( !compare_string_nc(str, "magenta"       ) ) return  COLOR_MAGENTA  ;
        if( !compare_string_nc(str, "orange"        ) ) return  COLOR_ORANGE   ;
        if( !compare_string_nc(str, "purple"        ) ) return  COLOR_PURPLE   ;
        if( !compare_string_nc(str, "pink"          ) ) return  COLOR_PINK     ;
        if( !compare_string_nc(str, "brown"         ) ) return  COLOR_BROWN    ;
        if( !compare_string_nc(str, "gold"          ) ) return  COLOR_GOLD     ;
        if( !compare_string_nc(str, "silver"        ) ) return  COLOR_SILVER   ;
        if( !compare_string_nc(str, "gray"          ) ) return  COLOR_GRAY     ;
        if( !compare_string_nc(str, "light-red"     ) ) return  COLOR_LRED     ;
        if( !compare_string_nc(str, "light-green"   ) ) return  COLOR_LGREEN   ;
        if( !compare_string_nc(str, "light-blue"    ) ) return  COLOR_LBLUE    ;
        if( !compare_string_nc(str, "light-cyan"    ) ) return  COLOR_LCYAN    ;
        if( !compare_string_nc(str, "light-magenta" ) ) return  COLOR_LMAGENTA ;
        if( !compare_string_nc(str, "light-yellow"  ) ) return  COLOR_LYELLOW  ;
        if( !compare_string_nc(str, "light-orange"  ) ) return  COLOR_LORANGE  ;
        if( !compare_string_nc(str, "light-purple"  ) ) return  COLOR_LPURPLE  ;
        if( !compare_string_nc(str, "light-pink"    ) ) return  COLOR_LPINK    ;
        if( !compare_string_nc(str, "light-brown"   ) ) return  COLOR_LBROWN   ;
        if( !compare_string_nc(str, "light-gold"    ) ) return  COLOR_LGOLD    ;
        if( !compare_string_nc(str, "light-gray"    ) ) return  COLOR_LGRAY    ;

        return COLOR_RED;
    }

    string get_color_string( const ColorName& color ) {
        switch( color ) {
        case COLOR_BLACK    : return string("black"         );
        case COLOR_WHITE    : return string("white"         );
        case COLOR_RED      : return string("red"           );
        case COLOR_GREEN    : return string("green"         );
        case COLOR_BLUE     : return string("blue"          );
        case COLOR_CYAN     : return string("cyan"          );
        case COLOR_YELLOW   : return string("yellow"        );
        case COLOR_MAGENTA  : return string("magenta"       );
        case COLOR_ORANGE   : return string("orange"        );
        case COLOR_PURPLE   : return string("purple"        );
        case COLOR_PINK     : return string("pink"          );
        case COLOR_BROWN    : return string("brown"         );
        case COLOR_GOLD     : return string("gold"          );
        case COLOR_SILVER   : return string("silver"        );
        case COLOR_GRAY     : return string("gray"          );
        case COLOR_LRED     : return string("light-red"     );
        case COLOR_LGREEN   : return string("light-green"   );
        case COLOR_LBLUE    : return string("light-blue"    );
        case COLOR_LCYAN    : return string("light-cyan"    );
        case COLOR_LYELLOW  : return string("light-yellow"  );
        case COLOR_LMAGENTA : return string("light-magenta" );
        case COLOR_LORANGE  : return string("light-orange"  );
        case COLOR_LPURPLE  : return string("light-purple"  );
        case COLOR_LPINK    : return string("light-pink"    );
        case COLOR_LBROWN   : return string("light-brown"   );
        case COLOR_LGOLD    : return string("light-gold"    );
        case COLOR_LGRAY    : return string("light-gray"    );
        }
        return "no-such-color";
    }


}
