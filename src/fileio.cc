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
#include <kortex/fileio.h>
#include <kortex/string.h>
#include <kortex/check.h>
#include <kortex/mem_unit.h>
#include <kortex/types.h>
#include <kortex/defs.h>

#include <sys/stat.h> // mkdir
#include <cstdlib>
#include <iostream>
#include <iomanip>

using namespace std;

namespace kortex {

    FileFormat get_file_format( const string& str ) {
        string fext = get_file_extension( str );
        if     ( !fext.compare("pgm"         ) ) return FF_PGM;
        else if( !fext.compare("ppm"         ) ) return FF_PPM;
        else if( !fext.compare("jpg"         ) ) return FF_JPG;
        else if( !fext.compare("png"         ) ) return FF_PNG;
        else if( !fext.compare("ibin"        ) ) return FF_IBIN;
        else if( !fext.compare("txt"         ) ) return FF_TXT;
        else if( !fext.compare("calibration" ) ) return FF_CALIBRATION;
        else return FF_NONE;
    }

    int create_folder( const string& path ) {
#ifdef __GNUC__
        mode_t perm=0777;
        return mkdir( path.c_str(), perm );
#else
        logman_fatal_g( "folder creation not supported in this OS [%s]", path.c_str() );
#endif
    }

    void check_file_stream_error( const ifstream& fin, const char* msg ) {
        if( !fin.fail() ) return;
        if( msg == NULL ) logman_fatal  ("error while reading file stream");
        else              logman_fatal_g("error while reading file stream [%s]", msg );
    }
    void check_file_stream_error(const ofstream& fout, const char* msg) {
        if( !fout.fail() ) return;
        if( msg == NULL ) logman_fatal  ("error while writing file stream");
        else              logman_fatal_g("error while writing file stream [%s]", msg);
    }

    void reset_file_contents(const char* file_name) {
        passert_pointer( file_name );
        FILE* fp = fopen(file_name,"w");
        if( fp ) fclose(fp);
    }

    bool delete_file(const string& path) {
        logman_log_g("deleting file [%s]", path.c_str());
        if( remove( path.c_str() ) == 0 ) return true;
        else                              return false;
    }

    void open_or_fail(const string& file, ifstream &fin) {
        fin.open( file.c_str() );
        check_file_stream_error(fin, ("cannot open file: "+file).c_str());
    }

    void open_or_fail(const string& file, ofstream &fout) {
        fout.open( file.c_str() );
        check_file_stream_error(fout, ("cannot open file: "+file).c_str());
    }

    void file_exists_or_fail(const string& file) {
        if( !file_exists( file ) )
            logman_fatal_g("file does not exist [%s]", file.c_str());
    }

    bool file_exists(const string& file) {
        return file_exists(file.c_str());
    }

    bool file_exists(const char* file) {
        FILE *fp = fopen(file,"r");
        if( fp == NULL ) {
            return false;
        } else {
            fclose(fp);
            return true;
        }
    }

    bool file_create( const string& file ) {
        FILE *fp = fopen(file.c_str(),"w");
        if( fp == NULL ) {
            logman_fatal_g("could not create file [%s]", file.c_str());
        } else {
            fclose(fp);
            return true;
        }
    }

    void read_string(ifstream& fin, string& param, const char* check_against) {
        char buffer[1024];
        fin.getline( buffer, 1024 );
        param = buffer;
        check_file_stream_error(fin);
        if( check_against == NULL ) return;
        if( !is_exact_match( param.c_str(), check_against ) )
            logman_fatal_g("read param error. read[%s], expecting[%s]", param.c_str(), check_against);
    }


//
//  binary
//

    void check_binary_stream_begin_tag(ifstream& fin) {
        int safety_num;
        fin.read( (char*)&safety_num, sizeof(safety_num) );
        if( safety_num != AC_SAFETY_BEGIN_NUM || fin.fail() )
            logman_fatal("stream corrupted");
        check_file_stream_error(fin);
    }
    void check_binary_stream_end_tag(ifstream& fin) {
        int safety_num;
        fin.read( (char*)&safety_num, sizeof(safety_num) );
        if( safety_num != AC_SAFETY_END_NUM || fin.fail() )
            logman_fatal("stream corrupted");
        check_file_stream_error(fin);
    }

    void insert_binary_stream_begin_tag(ofstream& fout) {
        int safety_num = AC_SAFETY_BEGIN_NUM;
        fout.write( (const char*)&safety_num, sizeof(safety_num) );
        check_file_stream_error(fout);
    }

    void insert_binary_stream_end_tag(ofstream& fout) {
        int safety_num = AC_SAFETY_END_NUM;
        fout.write( (char*)&safety_num, sizeof(safety_num) );
        check_file_stream_error(fout);
    }

//
//

    void save_ascii(const string& file, const vector<bool>& array) {
        ofstream fout;
        open_or_fail(file, fout);
        write_array( fout, NULL, array );
        fout.close();
    }

    void load_ascii(const string& file, vector<bool>& array) {
        array.clear();
        ifstream fin;
        open_or_fail(file, fin);
        int narr;
        read_param( fin, NULL, narr );
        array.resize(narr);
        for( int i=0; i<narr; i++ ) {
            bool b;
            fin >> b;
            array[i] = b;
        }
        fin.close();
    }



}

