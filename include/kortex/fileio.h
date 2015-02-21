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
#ifndef KORTEX_FILEIO_H
#define KORTEX_FILEIO_H

#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ostream;

#include <kortex/check.h>
#include <kortex/string.h>

#include <cfloat>

#ifdef DBL_DECIMAL_DIG
#define OP_DBL_Digs (DBL_DECIMAL_DIG)
#else
#ifdef DECIMAL_DIG
#define OP_DBL_Digs (DECIMAL_DIG)
#else
#define OP_DBL_Digs (DBL_DIG + 3)
#endif
#endif

namespace kortex {

    enum FileFormat { FF_NONE=0,
                      FF_PGM, FF_PPM, FF_JPG, FF_PNG,
                      FF_IBIN /*binary image file*/,
                      FF_CALIBRATION, FF_TXT };

    FileFormat get_file_format( const string& str );


    void check_file_stream_error( const ifstream& fin,  const char* msg=NULL );
    void check_file_stream_error( const ofstream& fout, const char* msg=NULL );

    void reset_file_contents(const char* file_name);
    int  create_folder(const string& path, mode_t perm=0777);
    bool delete_file(const string& path);

    void open_or_fail(const string& file, ifstream &fin);
    void open_or_fail(const string& file, ofstream &fout);

    void file_exists_or_fail(const string& file);
    bool file_exists(const string& file);
    bool file_exists(const char* file);
    bool file_create( const string& file );

//
//
//
    inline string in_str( const int   & v ) {
        char buffer[1024];
        sprintf( buffer, "%d", v );
        return string(buffer);
    }
    inline string in_str( const float & v ) {
        char buffer[1024];
        sprintf( buffer, "%.*e", OP_DBL_Digs-1, v );
        return string(buffer);
    }
    inline string in_str( const double& v ) {
        char buffer[1024];
        sprintf( buffer, "%.*e", OP_DBL_Digs-1, v );
        return string(buffer);
    }
    inline string in_str( const size_t& v ) {
        char buffer[1024];
        sprintf( buffer, "%zd", v );
        return string(buffer);
    }
    inline string in_str( const uint  & v ) {
        char buffer[1024];
        sprintf( buffer, "%ud", v );
        return string(buffer);
    }
    inline string in_str( const bool  & v ) {
        char buffer[1024];
        sprintf( buffer, "%d", v );
        return string(buffer);
    }
    inline string in_str( const char  & v ) {
        char buffer[1024];
        sprintf( buffer, "%c", v );
        return string(buffer);
    }
    inline string in_str( const string& v ) {
        return v;
    }

    template<typename T>
    void write_param( ofstream& fout, const char* param_name, const T& param ) {
        if( param_name ) fout << param_name << " ";
        fout << in_str( param ) << "\n";
    }

    template<typename T>
    void write_array( ofstream& fout, const char* param_name, const vector<T>& arr ) {
        write_param( fout, param_name, (int)arr.size() );
        int n_arr = arr.size();
        if( n_arr == 0 ) return;
        for( int i=0; i<n_arr-1; i++ )
            fout << in_str(arr[i]) << " ";
        fout << in_str( arr[n_arr-1] ) << "\n";
        check_file_stream_error( fout );
    }

    template<typename T>
    void write_array( ofstream& fout, const char* param_name, const T* arr, const int& n_arr ) {
        assert_pointer( arr );
        write_param( fout, param_name, (int)n_arr );
        if( n_arr == 0 ) return;
        for( int i=0; i<n_arr-1; i++ )
            fout << in_str(arr[i]) << " ";
        fout << in_str( arr[n_arr-1] ) << "\n";
        check_file_stream_error( fout );
    }


//
//

    inline void in_value( const char* str, float  & v ) { v = atof(str);       }
    inline void in_value( const char* str, double & v ) { v = atof(str);       }
    inline void in_value( const char* str, int    & v ) { v = atoi(str);       }
    inline void in_value( const char* str, size_t & v ) { v = atoi(str);       }
    inline void in_value( const char* str, uint   & v ) { v = atoi(str);       }
    inline void in_value( const char* str, bool   & v ) { v = (bool)atoi(str); }
    inline void in_value( const char* str, char   & v ) { v = str[0];          }
    inline void in_value( const char* str, string & v ) { v = string(str);     }

    template<typename T>
    void read_param( ifstream& fin, const char* param_name, T& param ) {
        char buffer[1024];
        fin.getline( buffer, 1024 );
        check_file_stream_error(fin, param_name);
        string format;
        if( param_name ) {
            format = string(param_name)+" %s";
            if( compare_string_nc( buffer, param_name ) ) {
                logman_fatal_g("param name [%s] does not match what is read [%s]", param_name, buffer );
            }
        } else {
            format = "%s";
        }

        char stmp[1024];
        sscanf( buffer, format.c_str(), &stmp );
        in_value(stmp, param);
    }

    template<typename T>
    void read_array( ifstream& fin, const char* param_name, T* arr, const int& n_arr ) {
        passert_pointer( arr );
        passert_pointer_size( n_arr );
        int asz;
        read_param( fin, param_name, asz );
        passert_statement_g( asz == n_arr, "array sizes do not match [%d - %d]", asz, n_arr );
        if( n_arr == 0 ) return;
        for( int i=0; i<n_arr; i++ )
            fin >> arr[i];
        char buf[8];
        fin.getline(buf,8); // get rid of the newline character at the end.
        check_file_stream_error( fin );
    }

    template<typename T>
    void read_array( ifstream& fin, const char* param_name, vector<T>& arr ) {
        arr.clear();
        int n_arr;
        read_param( fin, param_name, n_arr );
        if( n_arr == 0 ) return;
        arr.resize(n_arr);
        T rv;
        for( int i=0; i<n_arr; i++ ) {
            fin >> rv;
            arr[i] = rv;
        }
        char buf[8];
        fin.getline(buf,8); // get rid of the newline character at the end.
        check_file_stream_error( fin );
    }

    void read_string(ifstream& fin, string& param, const char* check_against);

//
//  binary
//

    void check_binary_stream_begin_tag(ifstream& fin);
    void check_binary_stream_end_tag  (ifstream& fin);

    void insert_binary_stream_begin_tag(ofstream& fout);
    void insert_binary_stream_end_tag  (ofstream& fout);

//
    template<typename T>
    void write_bparam( ofstream& fout, const T& v ) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }

    template<> inline
    void write_bparam( ofstream& fout, const string& v ) {
        int nsz = v.size();
        write_bparam( fout, nsz );
        fout.write( (const char*)v.c_str(), sizeof(char)*nsz );
        check_file_stream_error(fout);
    }
    template<typename T>
    void write_barray( ofstream& fout, const T* varr, const int& nv ) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }

    template<typename T>
    void write_barray( ofstream& fout, const vector<T>& varr ) {
        int nv = varr.size();
        write_bparam( fout, nv );
        for( int i=0; i<nv; i++ )
            write_bparam( fout, varr[i] );
        check_file_stream_error(fout);
    }


//
    template<typename T>
    void read_bparam( ifstream& fin, T& v ) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    template<> inline
    void read_bparam( ifstream& fin, string& v ) {
        int nsz = 0;
        read_bparam( fin, nsz );
        char buffer[2049];
        passert_statement( nsz < 2048, "buffer overflow" );
        fin.read( buffer, sizeof(*buffer)*nsz );
        buffer[nsz] = '\0';
        v = buffer;
        check_file_stream_error(fin);
    }

    template<typename T>
    void read_barray( ifstream& fin, T* varr, const int& nv ) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }

    template<typename T>
    void read_barray( ifstream& fin, vector<T>& varr ) {
        int nv = 0;
        read_bparam( fin, nv );
        varr.resize(nv);
        for( int i=0; i<nv; i++ ) {
            T b;
            read_bparam( fin, b ); // otherwise compiler complains for bool
            varr[i] = b;
        }
        check_file_stream_error(fin);
    }

//

    void save_ascii( const string& file, const vector<bool>& array );
    void load_ascii( const string& file,       vector<bool>& array );

}

#endif
