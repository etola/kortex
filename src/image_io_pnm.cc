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
#include <kortex/image_io_pnm.h>
#include <kortex/image.h>
#include <kortex/check.h>
#include <kortex/types.h>
#include <kortex/defs.h>

#include <fstream>
#include <cstring>
#include <climits>

#define PNM_BUFFER_SIZE 256

using namespace std;

namespace kortex {

    void  read_packed( uchar* data, const int& size, ifstream &f ) {
        uchar c = 0;
        int bitshift = -1;
        for (int pos = 0; pos < size; pos++) {
            if (bitshift == -1) {
                c = static_cast<uchar>(f.get());
                bitshift = 7;
            }
            data[pos] = (c >> bitshift) & 1;
            bitshift--;
        }
    }

    void write_packed( const uchar *data, const int& size, ofstream &f ) {
        uchar c = 0;
        int bitshift = 7;
        for (int pos = 0; pos < size; pos++) {
            c = c | (data[pos] << bitshift);
            bitshift--;
            if ((bitshift == -1) || (pos == size-1)) {
                f.put(c);
                bitshift = 7;
                c = 0;
            }
        }
    }

    void pnm_read( ifstream &file, char *buf ) {
        char doc[PNM_BUFFER_SIZE];
        char c;
        file >> c;
        while (c == '#') {
            file.getline(doc, PNM_BUFFER_SIZE);
            file >> c;
        }
        file.putback(c);
        file.width(PNM_BUFFER_SIZE);
        file >> buf;
        file.ignore();
    }

    int read_pnm_size( const string& file, int &w, int &h, int &nc ) {
        char buf[PNM_BUFFER_SIZE];
        std::ifstream fin( file.c_str(), std::ios::in | std::ios::binary);
        pnm_read(fin, buf);
        if     ( !strncmp(buf, "P6", 2) ) nc = 3; // ppm
        else if( !strncmp(buf, "P5", 2) ) nc = 1; // pgm
        else if( !strncmp(buf, "P4", 2) ) nc = 1; // pbm
        else   logman_fatal("invalid pnm file");
        pnm_read(fin, buf);   w = atoi(buf);
        pnm_read(fin, buf);   h = atoi(buf);
        return 0;
    }

    void load_pgm(const string& file, Image* img) {
        passert_pointer( img );
        char buf[PNM_BUFFER_SIZE];
        ifstream fin(file.c_str(), std::ios::in | std::ios::binary);
        pnm_read(fin, buf);

        if( strncmp(buf, "P5", 2) )
            logman_fatal("pnm type mismatch");

        pnm_read(fin, buf); int w = atoi(buf);
        pnm_read(fin, buf); int h = atoi(buf);

        passert_boundary( w, 0, MAX_IMAGE_DIM );
        passert_boundary( h, 0, MAX_IMAGE_DIM );

        pnm_read(fin, buf);
        if( atoi(buf) > UCHAR_MAX )
            logman_fatal("type mismatch");

        img->create(w, h, IT_U_GRAY);
        for( int y=0; y<h; y++ )
            fin.read( (char*)img->get_row_u(y), w*sizeof(uchar) );
        fin.close();
    }

    void load_ppm(const string& file, Image* img) {
        passert_pointer( img  );
        char buf[PNM_BUFFER_SIZE];
        ifstream fin(file.c_str(), std::ios::in | std::ios::binary);

        pnm_read(fin, buf);
        if( strncmp(buf, "P6", 2) )
            logman_fatal("pnm type mismatch");

        pnm_read(fin, buf); int w = atoi(buf);
        pnm_read(fin, buf); int h = atoi(buf);

        passert_boundary( w, 0, MAX_IMAGE_DIM );
        passert_boundary( h, 0, MAX_IMAGE_DIM );

        pnm_read(fin, buf);
        if( atoi(buf) > UCHAR_MAX )
            logman_fatal("type mismatch");

        img->create(w, h, IT_U_PRGB);

        for( int y = 0; y < h; y++ ) {
            uchar* row = img->get_row_u(y);
            for( int x = 0; x < w; x++ ) {
                fin.read((char*)buf, 3*sizeof(uchar));
                row[3*x  ] = buf[0];
                row[3*x+1] = buf[1];
                row[3*x+2] = buf[2];
            }
        }

        fin.close();
    }

    void save_pgm(const string& file, const Image* img) {
        passert_pointer( img  );
        img->passert_type( IT_U_GRAY | IT_J_GRAY, file.c_str() );

        int vmax = 0;
        if( img->type() == IT_U_GRAY ) {
            vmax = UCHAR_MAX;
        } else if( img->type() == IT_J_GRAY ) {
            vmax = 65535;
        }

        FILE* fp = fopen( file.c_str(), "wb" );
        passert_statement_g( fp, "cannot open file [%s]", file.c_str() );

        const char* magic_head = "P5";
        fprintf( fp, "%s\n%d %d %d\n", magic_head, img->w(), img->h(), vmax );

        if( img->type() == IT_U_GRAY ) {
            for( int y = 0; y < img->h(); y++ ) {
                const uchar* irow = img->get_row_u(y);
                fwrite( (const void*)irow, sizeof(*irow), img->w(), fp );
            }
        } else {
            for( int y = 0; y < img->h(); y++ ) {
                const uint16_t* irow = img->get_row_u16(y);
                fwrite( (const void*)irow, sizeof(*irow), img->w(), fp );
            }
        }

        fclose( fp );

    }

    void save_ppm(const string& file, const Image* img) {
        passert_pointer( img  );
        img->passert_type( IT_U_PRGB | IT_U_IRGB, file.c_str() );
        ofstream fout(file.c_str(), std::ios::out | std::ios::binary);
        fout << "P6\n" << img->w() << " " << img->h() << "\n" << UCHAR_MAX << "\n";
        uchar r, g, b;
        for(int y = 0; y < img->h(); y++) {
            for(int x = 0; x < img->w(); x++ ) {
                img->get(x, y, r, g, b);
                fout << r << g << b;
            }
        }
        fout.close();
    }



}
