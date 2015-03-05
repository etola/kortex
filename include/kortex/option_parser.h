// ---------------------------------------------------------------------------
//
// This file is part of the <kortex> library suite
//
// Copyright (C) 2015 Engin Tola
//
// See LICENSE file for license information.
//
// author: Engin Tola
// e-mail: engintola@gmail.com
// web   : http://www.engintola.com
// web   : http://www.aurvis.com
//
// ---------------------------------------------------------------------------
#ifndef KORTEX_OPTION_PARSER_H
#define KORTEX_OPTION_PARSER_H

#include <kortex/check.h>
#include <kortex/types.h>
#include <kortex/string.h>

namespace kortex {

    enum OptionType { OP_NO_INPUT=0, OP_SINGLE_INPUT=1, OP_MULTI_INPUT=-1 };

    struct OptionItem {

        string         name;
        string         explanation;
        vector<string> value;
        DataType       data_type;
        OptionType     opt_type;

        void   clear() { value.clear(); }

        string get_value( int i ) const { return value[i];     }
        int    n_values()         const { return value.size(); }

        void push_value( const string& str ) { value.push_back( str ); }

        OptionItem() {
            name           = "";
            explanation    = "";
            data_type      = TYPE_BOOL;
            opt_type       = OP_NO_INPUT;
        }
    };

    class OptionParser {
    public:

        OptionParser() {
            add_option( "-h", "print help", "", OP_NO_INPUT );
        }

        void add_option( const string& name, const string& explanation, const string& type, OptionType opt_type, const string& default_value="" );

        void print_help   () const;
        void print_options() const;

        int  n_options    () const { return m_options.size(); }

        int get_option( const char* str ) const;
        const OptionItem& get_option( int i ) const {
            assert_boundary( i, 0, n_options() );
            return m_options[i];
        }
        OptionItem& get_option( int i ) {
            assert_boundary( i, 0, n_options() );
            return m_options[i];
        }

        void parse( int argc, char** argv );

        //
        // access to options
        //
        template<typename T>
        bool   get( const char* str, T& v ) const {
            int oid = get_option( str );
            if( oid == -1 ) {
                logman_fatal_g( "no such key [opt %s]\n", str );
            }
            const OptionItem& opt = get_option(oid);
            if( opt.data_type != get_type(v) )
                logman_fatal_g( "option type and requested data type does not match [opt %s]\n", opt.name.c_str() );

            switch( opt.opt_type ) {
            case OP_NO_INPUT:
                if( opt.n_values() ) in_value("1", v);
                else                 in_value("0", v);
                break;
            case OP_SINGLE_INPUT:
                if( opt.n_values() == 0 )
                    return false;
                in_value( opt.get_value(0).c_str(), v );
                return true;
                break;
            case OP_MULTI_INPUT:
                logman_fatal_g( "multi-element option - cannot be parsed [opt %s]\n", opt.name.c_str() );
                break;
            }
            return false;
        }

        int    geti( const char* str ) const { int    v; get( str, v ); return v; }
        double getd( const char* str ) const { double v; get( str, v ); return v; }
        string gets( const char* str ) const { string v; get( str, v ); return v; }
        bool   getb( const char* str ) const { bool   v; get( str, v ); return v; }

        template<typename T>
        void get( const char* str, vector<T>& v ) const {
            int oid = get_option( str );
            if( oid == -1 ) {
                printf( "no such key [%s]\n", str );
                exit(1);
            }
            const OptionItem& opt = get_option(oid);
            T tmp_v;
            if( opt.data_type != get_type( tmp_v  ) )
                logman_fatal_g( "option type and requested data type does not match [opt %s]\n", opt.name.c_str() );
            if( opt.opt_type != OP_MULTI_INPUT )
                logman_fatal_g( "can only process multi-element options [opt %s]\n", opt.name.c_str() );

            v.clear();
            if( opt.n_values() == 0 ) {
                printf( "option not set [%s]!\n", str );
                return;
            }

            for( int i=0; i<opt.n_values(); i++ ) {
                T tp;
                in_value( opt.get_value(i).c_str(), tp );
                v.push_back( tp );
            }
        }

    private:
        vector<OptionItem> m_options;
    };



}

#endif
