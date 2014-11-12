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

    int create_folder(const string& path, mode_t perm) {
        return mkdir( path.c_str(), perm );
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

    void write_string( ofstream& fout, const string& str ) {
        fout << str << endl;
        check_file_stream_error( fout );
    }

    void write_param(ofstream& fout, const string& param_name, const string& val ) {
        fout << param_name << " " << val << endl;
        check_file_stream_error( fout );
    }
    void write_param(ofstream& fout, const string& param_name, const int& val ) {
        fout << param_name << " " << val << endl;
        check_file_stream_error( fout );
    }
    void write_param(ofstream& fout, const string& param_name, const unsigned int& val ) {
        fout << param_name << " " << val << endl;
        check_file_stream_error( fout );
    }
    void write_param(ofstream& fout, const string& param_name, const double& val ) {
        fout << param_name << " "
             << std::setprecision(24)
             << val << endl;
        check_file_stream_error( fout );
    }
    void write_param(ofstream& fout, const string& param_name, const bool& val ) {
        fout << param_name << " " << val << endl;
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const string& param_name, const vector<int>& arr ) {
        fout << param_name << " ";
        fout << (int)arr.size() << " ";
        for( int i=0; i<(int)arr.size(); i++ ) {
            fout << arr[i] << " ";
        }
        fout << endl;
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const vector<int>& arr ) {
        fout << (int)arr.size() << " ";
        for( int i=0; i<(int)arr.size(); i++ ) {
            fout << arr[i] << " ";
        }
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const float* arr, const int& narr ) {
        fout << narr << " ";
        for( int i=0; i< narr; i++ )
            fout << arr[i] << " ";
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const string& param_name, const float* arr, const int& narr ) {
        fout << param_name << " " << narr << " ";
        for( int i=0; i< narr; i++ )
            fout << arr[i] << " ";
        fout << endl;
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const double* arr, const int& narr ) {
        fout << narr << " ";
        for( int i=0; i< narr; i++ )
            fout << arr[i] << " ";
        check_file_stream_error( fout );
    }
    void write_param( ofstream& fout, const int* arr, const int& narr ) {
        fout << narr << " ";
        for( int i=0; i< narr; i++ )
            fout << arr[i] << " ";
        check_file_stream_error( fout );
    }

    void write_param( ofstream& fout, const string& param_name, const double* arr, const int& narr ) {
        fout << param_name << " " << narr << " ";
        for( int i=0; i< narr; i++ )
            fout << std::setprecision(24)
                 << arr[i] << " ";
        fout << endl;
        check_file_stream_error( fout );
    }


    void read_string(ifstream& fin, string& param, const char* check_against) {
        fin >> param;
        check_file_stream_error(fin);
        if( check_against == NULL ) return;
        if( !is_exact_match( param.c_str(), check_against ) )
            logman_fatal_g("read param error. read[%s], expecting[%s]", param.c_str(), check_against);
    }
    void read_param(ifstream& fin, const char* param_name, string& param) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);
        fin>>param;
        check_file_stream_error(fin, param_name);
    }
    void read_param(ifstream& fin, const char* param_name, int& param, const int& param_min, const int& param_max) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);
        fin >> param;
        check_file_stream_error(fin, param_name);
        if( param < param_min || param > param_max )
            logman_fatal_g("read param error [%s] read[%d] pmin[%d] pmax[%d]", param_name, param, param_min, param_max);
    }

    void read_param(ifstream& fin, const char* param_name, uint& param, const uint& param_min, const uint& param_max) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);
        fin >> param;
        check_file_stream_error(fin, param_name);
        if( param < param_min || param > param_max )
            logman_fatal_g("read param [%s] error: out-of-bounds read[%d] pmin[%d] pmax[%d]", param_name, param, param_min, param_max);
    }

    void read_param( ifstream& fin, float* arr, const int& n_arr ) {
        passert_pointer( arr );
        passert_pointer_size( n_arr );
        int asz;
        fin >> asz;
        passert_statement_g( asz == n_arr, "read param error [asz %d] [narr %d]", asz, n_arr );
        for( int i=0; i<n_arr; i++ )
            fin >> arr[i];
        check_file_stream_error( fin );
    }

    void read_param( ifstream& fin, const char* param_name, float* arr, const int& n_arr ) {
        passert_pointer( arr );
        passert_pointer( param_name );
        passert_pointer_size( n_arr );

        string stmp;
        read_string(fin, stmp, param_name);

        int asz;
        fin >> asz;
        passert_statement_g( asz == n_arr, "read param error [%s] [asz %d] [narr %d]", param_name, asz, n_arr );
        for( int i=0; i<n_arr; i++ )
            fin >> arr[i];
        check_file_stream_error( fin );
    }

    void read_param( ifstream& fin, int* arr, const int& n_arr ) {
        passert_pointer( arr );
        passert_pointer_size( n_arr );
        int asz;
        fin >> asz;
        passert_statement_g( asz == n_arr, "read param error [asz %d] [narr %d]", asz, n_arr );
        for( int i=0; i<n_arr; i++ )
            fin >> arr[i];
        check_file_stream_error( fin );
    }

    void read_param( ifstream& fin, double* arr, const int& n_arr ) {
        passert_pointer( arr );
        passert_pointer_size( n_arr );
        int asz;
        fin >> asz;
        passert_statement_g( asz == n_arr, "read param error [asz %d] [narr %d]", asz, n_arr );
        for( int i=0; i<n_arr; i++ )
            fin >> arr[i];
        check_file_stream_error( fin );
    }
    void read_param(ifstream& fin, const char* param_name, double* arr, const int& n_arr) {
        passert_pointer( arr );
        passert_pointer( param_name );
        passert_pointer_size( n_arr );
        string stmp;
        read_string(fin, stmp, param_name);
        int asz;
        fin >> asz;
        passert_statement_g( asz == n_arr, "read param error [%s] [asz %d] [n_arr %d]", param_name, asz, n_arr );
        for(int i=0; i<n_arr; i++) {
            fin >> arr[i];
        }
        check_file_stream_error(fin);
    }

    void read_param(ifstream& fin, const char* param_name, double& param, const double& param_min, const double& param_max) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);
        fin>>param;
        check_file_stream_error(fin, param_name);
        if( param < param_min || param > param_max )
            logman_fatal_g("read param [%s] error: out-of-bounds read[%f] pmin[%f] pmax[%f]", param_name, param, param_min, param_max);
    }

    void read_param(ifstream& fin, const char* param_name, bool& param) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);
        fin>>param;
        check_file_stream_error(fin, param_name);
    }

    void read_param(ifstream& fin, const char* param_name, vector<int>& varr) {
        passert_pointer( param_name );
        string stmp;
        read_string(fin, stmp, param_name);

        int nvarr;
        fin >> nvarr;
        check_file_stream_error(fin, param_name);
        passert_boundary(nvarr,0,MAX_ARR_SIZE);

        varr.clear();
        varr.resize(nvarr);
        int itmp;
        for(int i=0; i<nvarr; i++) {
            fin >> itmp;
            check_file_stream_error(fin, param_name);
            varr[i] = itmp;
        }
    }

    void read_param(ifstream& fin, vector<int>& varr) {
        int nvarr;
        fin >> nvarr;
        passert_boundary(nvarr,0,MAX_ARR_SIZE);
        varr.clear();
        varr.resize(nvarr);
        for(int i=0; i<nvarr; i++) {
            fin >> varr[i];
        }
        check_file_stream_error(fin);
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



    void read_bparam(ifstream& fin, int& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, float& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, double& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, uint& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, bool& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, uchar& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, size_t& v) {
        fin.read( (char*)&v, sizeof(v) );
        check_file_stream_error(fin);
    }


    void read_bparam(ifstream& fin, int* varr, const int& nv) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, bool* varr, const int& nv) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, float * varr, const int& nv) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, double* varr, const int& nv) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }
    void read_bparam(ifstream& fin, uchar* varr, const int& nv) {
        passert_pointer( varr );
        passert_pointer_size( nv );
        fin.read( (char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fin);
    }

    void read_bparam(ifstream& fin, string& str) {
        int nsz = 0;
        fin.read( (char*)&nsz, sizeof(nsz) );
        char buffer[2561];
        passert_boundary( nsz, 0, 2560 );
        fin.read( buffer, sizeof(*buffer)*nsz );
        buffer[nsz] = '\0';
        str = buffer;
        check_file_stream_error(fin);
    }

    void read_bparam(ifstream& fin, vector<int>& arr) {
        arr.clear();
        int narr;
        read_bparam(fin,narr);
        passert_boundary(narr,0,MAX_ARR_SIZE);
        arr.resize(narr);
        int itmp;
        for(int i=0; i<narr; i++) {
            read_bparam(fin, itmp);
            arr[i] = itmp;
        }
        check_file_stream_error(fin);
    }

    void read_bparam(ifstream& fin, vector<bool>& arr) {
        arr.clear();
        int narr;
        read_bparam(fin,narr);
        passert_boundary(narr,0,MAX_ARR_SIZE);
        arr.resize(narr);
        bool btmp;
        for(int i=0; i<narr; i++) {
            read_bparam(fin, btmp);
            arr[i] = btmp;
        }
        check_file_stream_error(fin);
    }

    void write_bparam(ofstream& fout, const string& str) {
        int nsz = str.size();
        fout.write( (const char*)&nsz, sizeof(nsz) );
        fout.write( (const char*)str.c_str(), sizeof(char)*nsz );
        check_file_stream_error(fout);
    }

    void write_bparam(ofstream& fout, const int& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const float& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const double& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const uint& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const bool& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const uchar& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const size_t& v) {
        fout.write( (const char*)&v, sizeof(v) );
        check_file_stream_error(fout);
    }

    void write_bparam(ofstream& fout, const uchar* varr, const int& nv) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const bool* varr, const int& nv) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const float * varr, const int& nv) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const double* varr, const int& nv) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }
    void write_bparam(ofstream& fout, const int* varr, const int& nv) {
        fout.write( (const char*)varr, sizeof(*varr)*nv );
        check_file_stream_error(fout);
    }

    void write_bparam(ofstream& fout, const vector<int>& arr) {
        int narr = arr.size();
        write_bparam(fout, narr);
        vector<int>::const_iterator it = arr.begin();
        while( it != arr.end() ) {
            write_bparam(fout, (*it) );
            ++it;
        }
        check_file_stream_error(fout);
    }

    void write_bparam(ofstream& fout, const vector<bool>& arr) {
        int narr = arr.size();
        write_bparam(fout, narr);
        vector<bool>::const_iterator it = arr.begin();
        while( it != arr.end() ) {
            write_bparam(fout, (*it) );
            ++it;
        }
        check_file_stream_error(fout);
    }




    void save_ascii(const string& file, const vector<bool>& array) {
        ofstream fout;
        open_or_fail(file, fout);
        fout<<array.size()<<"\n";
        for(int i=0; i<(int)array.size(); i++)
            fout<<array[i]<<" ";
        fout<<"\n";
        check_file_stream_error(fout);
        fout.close();
    }

    void load_ascii(const string& file, vector<bool>& array) {
        array.clear();
        ifstream fin;
        open_or_fail(file, fin);
        int narr;
        fin>>narr;
        passert_boundary(narr,0,MAX_ARR_SIZE);
        array.resize(narr);
        int v;
        for(int i=0; i<narr; i++) {
            fin >> v;
            array[i] = bool(v);
        }
        check_file_stream_error(fin);
        fin.close();
    }



}

