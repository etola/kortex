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

#include <kortex/keyed_value.h>
#include <kortex/check.h>
#include <kortex/string.h>

namespace kortex {

    enum FileFormat { FF_NONE=0,
                      FF_PGM, FF_PPM, FF_JPG, FF_PNG,
                      FF_IBIN /*binary image file*/,
                      FF_CALIBRATION, FF_TXT };

    FileFormat get_file_format( const string& str );

    void check_file_stream_error( const ifstream& fin,  const char* msg=NULL );
    void check_file_stream_error( const ofstream& fout, const char* msg=NULL );

    void reset_file_contents(const char* file_name);
    int  create_folder( const string& path );
    bool delete_file  ( const string& path );

    void open_or_fail( const string& file, ifstream &fin,  bool binary );
    void open_or_fail( const string& file, ofstream &fout, bool binary );

    void file_exists_or_fail(const string& file);
    bool file_exists(const string& file);
    bool file_exists(const char* file);
    bool file_create( const string& file );

	bool remove_folder( const string& dir_path );

    // return the files in a folder. if file_extension is specified, only files with that extension is returned. it is case insensitive.
    void get_files(const string& input_folder, vector<string>& ifiles, const char* file_extension=nullptr );

//
//
//
    template<typename T>
    void write_param( ofstream& fout, const char* param_name, const T& param ) {
        if( param_name ) fout << param_name << " ";
        fout << in_str( param ) << "\n";
    }
    template<> inline
    void write_param( ofstream& fout, const char* param_name, const ifloat& param ) {
        if( param_name ) fout << param_name << " ";
        fout << in_str( param.key ) << " " << in_str( param.val ) << "\n";
    }

    template<typename T>
    void write_array( ofstream& fout, const char* param_name, const vector<T>& arr ) {
        write_param( fout, param_name, (int)arr.size() );
        int n_arr = (int)arr.size();
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

    template<> inline
    void read_param( ifstream& fin, const char* param_name, ifloat& param ) {
        char buffer[1024];
        fin.getline( buffer, 1024 );
        check_file_stream_error(fin, param_name);
        string format;
        if( param_name ) {
            format = string(param_name)+" %s %s";
            if( compare_string_nc( buffer, param_name ) ) {
                logman_fatal_g("param name [%s] does not match what is read [%s]", param_name, buffer );
            }
        } else {
            format = "%s %s";
        }
        char itmp[256], vtmp[256];
        sscanf( buffer, format.c_str(), &itmp, &vtmp );
        in_value( itmp, param.key );
        in_value( vtmp, param.val );
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
        uint16_t nsz = (uint16_t)v.size();
        write_bparam( fout, nsz );
        fout.write( (const char*)v.c_str(), sizeof(char)*nsz );
        check_file_stream_error(fout);
    }

    template<> inline
    void write_bparam( ofstream& fout, const iint& v ) {
        write_bparam( fout, v.key );
        write_bparam( fout, v.val );
    }

    template<typename T>
    void write_barray( ofstream& fout, const T* varr, const size_t& nv ) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }

    template<typename T>
    void write_barray( ofstream& fout, const vector<T>& varr ) {
        int nv = (int)varr.size();
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
        uint16_t nsz = 0;
        read_bparam( fin, nsz );
        char buffer[2048];
        passert_statement( nsz < 2048, "buffer overflow" );
        fin.read( buffer, sizeof(*buffer)*nsz );
        buffer[nsz] = '\0';
        v = buffer;
        check_file_stream_error(fin);
    }
    template<> inline
    void read_bparam( ifstream& fin, iint& v ) {
        read_bparam( fin, v.key );
        read_bparam( fin, v.val );
    }
    template<> inline
    void read_bparam( ifstream& fin, ifloat& v ) {
        read_bparam( fin, v.key );
        read_bparam( fin, v.val );
    }

    template<typename T>
    void read_barray( ifstream& fin, T* varr, const size_t& nv ) {
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

    template<typename T>
    void save_ascii( const string& file, const vector<T>& arr ) {
        ofstream fout;
        open_or_fail( file, fout, false );
        write_array( fout, NULL, arr );
        fout.close();
    }
    template<typename T>
    void load_ascii( const string& file, vector<T>& arr ) {
        ifstream fin;
        open_or_fail( file, fin, false );
        read_array( fin, NULL, arr );
        fin.close();
    }

    void read_string(ifstream& fin, string& param, const char* check_against);


}

#endif
