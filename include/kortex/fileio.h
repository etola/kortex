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

    void write_string(std::ofstream& fout, string label, string str);
    bool read_string(std::ifstream& fin, string& label, string& str);
    bool read_string(std::ifstream& fin, const string& check_label, string& label, string& str);

    void write_separator(std::ofstream& fout, string label);
    bool read_separator(std::ifstream& fin, string& label);
    bool read_separator(std::ifstream& fin, const string& check_label, string& label);

    template<typename T>
    void write_number(std::ofstream& fout, string label, T v) {
        fout << label << ":" << in_str(v) << std::endl;
    }
    template<typename T0, typename T1>
    void write_number(std::ofstream& fout, string label, T0 v0, T1 v1) {
        fout << label << ":" << in_str(v0) << ":" << in_str(v1) << std::endl;
    }
    template<typename T0, typename T1, typename T2>
    void write_number(std::ofstream& fout, string label, T0 v0, T1 v1, T2 v2) {
        fout << label << ":" << in_str(v0) << ":" << in_str(v1) << ":" << in_str(v2) << std::endl;
    }

    template<typename T>
    bool read_number(std::ifstream& fin, string& label, T& v) {
        string str;
        read_string(fin,label,str);
        in_value(str.c_str(), v);
        return !fin.fail();
    }
    template<typename T>
    bool read_number(std::ifstream& fin, const string& check_label, string& label, T& v) {
        if( !read_number(fin, label, v) )
            return false;
        if( !is_exact_match(check_label,label) ) {
            logman_warning_g("labels do not match [%s] [%s]", check_label.c_str(), label.c_str() );
            return false;
        }
        return true;
    }
    template<typename T0, typename T1>
    bool read_number(std::ifstream& fin, string& label, T0& v0, T1& v1) {
        string str;
        std::getline(fin, label, ':');
        std::string s0;
        std::getline(fin, s0, ':');
        std::string s1;
        std::getline(fin, s1, '\n');
        in_value(s0.c_str(), v0);
        in_value(s1.c_str(), v1);
        return !fin.fail();
    }
    template<typename T0, typename T1>
    bool read_number(std::ifstream& fin, const string& check_label, string& label, T0& v0, T1& v1) {
        if( !read_number(fin, label, v0, v1) )
            return false;
        if( !is_exact_match(check_label,label) ) {
            logman_warning_g("labels do not match [%s] [%s]", check_label.c_str(), label.c_str() );
            return false;
        }
        return true;
    }

    template<typename T0, typename T1, typename T2>
    bool read_number(std::ifstream& fin, string& label, T0& v0, T1& v1, T2& v2) {
        string str;
        std::getline(fin, label, ':');
        std::string s0; std::getline(fin, s0, ':');
        std::string s1; std::getline(fin, s1, ':');
        std::string s2; std::getline(fin, s2, '\n');
        in_value(s0.c_str(), v0);
        in_value(s1.c_str(), v1);
        in_value(s2.c_str(), v2);
        return !fin.fail();
    }
    template<typename T0, typename T1, typename T2>
    bool read_number(std::ifstream& fin, const string& check_label, string& label, T0& v0, T1& v1, T2& v2) {
        if( !read_number(fin, label, v0, v1, v2) )
            return false;
        if( !is_exact_match(check_label,label) ) {
            logman_warning_g("labels do not match [%s] [%s]", check_label.c_str(), label.c_str() );
            return false;
        }
        return true;
    }

    template<typename T>
    void write_number_array(std::ofstream& fout, string label, T* arr, int32_t asz) {
        fout << label << ":" << in_str(asz) << ":";
        for( int32_t i=0; i<asz-1; i++ ) {
            fout<< in_str(arr[i]) << " ";
        }
        if( asz > 0 )
            fout<< in_str(arr[asz-1]);
        fout << std::endl;
    }

    template<typename T>
    bool read_number_array(std::ifstream& fin, string& label, T* arr, int32_t& asz) {
        std::getline(fin,label,':');
        string szstr;
        std::getline(fin,szstr,':');
        in_value(szstr.c_str(), asz);
        string vstr;
        for( int32_t i=0; i<asz-1; i++) {
            std::getline(fin,vstr,' ');
            in_value(vstr.c_str(), arr[i]);
        }
        std::getline(fin,vstr,'\n');
        if( asz > 0 ) {
            in_value(vstr.c_str(), arr[asz-1]);
        }
        return !fin.fail();
    }

    template<typename T>
    bool read_number_array(std::ifstream& fin, const string& check_label, string& label, T* arr, int32_t& asz) {
        if( !read_number_array(fin, label, arr, asz) )
            return false;
        if( !is_exact_match(check_label,label) ) {
            logman_warning_g("labels do not match [%s] [%s]", check_label.c_str(), label.c_str() );
            return false;
        }
        return true;
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


}

#endif
