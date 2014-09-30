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

#include <kortex/types.h>

#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ostream;


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

    void write_string( ofstream& fout, const string& str );
    void write_param(ofstream& fout, const string& param_name, const string& val );
    void write_param(ofstream& fout, const string& param_name, const int& val );
    void write_param(ofstream& fout, const string& param_name, const unsigned int& val );
    void write_param(ofstream& fout, const string& param_name, const double& val );
    void write_param(ofstream& fout, const string& param_name, const bool& val );
    void write_param(ofstream& fout, const string& param_name, const vector<int>& arr );
    void write_param(ofstream& fout, const vector<int>& arr );

    void write_param(ofstream& fout,                           const int   * arr, const int& narr );
    void write_param(ofstream& fout,                           const float * arr, const int& narr );
    void write_param(ofstream& fout, const string& param_name, const float * arr, const int& narr );
    void write_param(ofstream& fout,                           const double* arr, const int& narr );
    void write_param(ofstream& fout, const string& param_name, const double* arr, const int& narr );

    void read_string(ifstream& fin, string& param, const char* check_against);

    void read_param (ifstream& fin,                         int   * arr, const int& n_arr );
    void read_param (ifstream& fin,                         float * arr, const int& n_arr );
    void read_param (ifstream& fin, const char* param_name, float * arr, const int& n_arr );
    void read_param (ifstream& fin,                         double* arr, const int& n_arr );
    void read_param (ifstream& fin, const char* param_name, double* arr, const int& n_arr );

    void read_param (ifstream& fin, const char* param_name, string& param);
    void read_param (ifstream& fin, const char* param_name, int& param, const int& param_min, const int& param_max);
    void read_param (ifstream& fin, const char* param_name, uint& param, const uint& param_min, const uint& param_max);
    void read_param (ifstream& fin, const char* param_name, double& param, const double& param_min, const double& param_max);
    void read_param (ifstream& fin, const char* param_name, bool& param);
    void read_param (ifstream& fin, const char* param_name, vector<int>& varr);
    void read_param (ifstream& fin, vector<int>& varr);

//
//  binary
//

    void check_binary_stream_begin_tag(ifstream& fin);
    void check_binary_stream_end_tag(ifstream& fin);

    void insert_binary_stream_begin_tag(ofstream& fout);
    void insert_binary_stream_end_tag(ofstream& fout);

    void read_bparam(ifstream& fin, int& v);
    void read_bparam(ifstream& fin, float& v);
    void read_bparam(ifstream& fin, double& v);
    void read_bparam(ifstream& fin, uint& v);
    void read_bparam(ifstream& fin, bool& v);
    void read_bparam(ifstream& fin, uchar& v);
    void read_bparam(ifstream& fin, size_t& v);

    void read_bparam(ifstream& fin, int* varr, const int& nv);
    void read_bparam(ifstream& fin, bool* varr, const int& nv);
    void read_bparam(ifstream& fin, float * varr, const int& nv);
    void read_bparam(ifstream& fin, double* varr, const int& nv);
    void read_bparam(ifstream& fin, uchar* varr, const int& nv);
    void read_bparam(ifstream& fin, string& str);
    void read_bparam(ifstream& fin, vector<int>& arr);
    void read_bparam(ifstream& fin, vector<bool>& arr);

    void write_bparam(ofstream& fout, const string& str);
    void write_bparam(ofstream& fout, const int& v);
    void write_bparam(ofstream& fout, const float& v);
    void write_bparam(ofstream& fout, const double& v);
    void write_bparam(ofstream& fout, const uint& v);
    void write_bparam(ofstream& fout, const bool& v);
    void write_bparam(ofstream& fout, const uchar& v);
    void write_bparam(ofstream& fout, const size_t& v);

    void write_bparam(ofstream& fout, const uchar* varr, const int& nv);
    void write_bparam(ofstream& fout, const bool* varr, const int& nv);
    void write_bparam(ofstream& fout, const float * varr, const int& nv);
    void write_bparam(ofstream& fout, const double* varr, const int& nv);
    void write_bparam(ofstream& fout, const int* varr, const int& nv);
    void write_bparam(ofstream& fout, const vector<int>& arr);
    void write_bparam(ofstream& fout, const vector<bool>& arr);

    void save_ascii(const string& file, const vector<bool>& array);
    void load_ascii(const string& file, vector<bool>& array);

}

#endif
