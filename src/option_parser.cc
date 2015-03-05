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

#include <cstring>
#include <kortex/option_parser.h>

namespace kortex {

    DataType data_type( const string& str ) {
        if( !strcmp( str.c_str(), "int"    ) ) return TYPE_INT;
        if( !strcmp( str.c_str(), "double" ) ) return TYPE_DOUBLE;
        if( !strcmp( str.c_str(), "string" ) ) return TYPE_STRING;
        if( !strcmp( str.c_str(), "bool"   ) ) return TYPE_BOOL;
        logman_fatal_g( "not a recognized type [%s]", str.c_str() );
    }

    string data_type( const DataType& dt ) {
        string str;
        switch( dt ) {
        case TYPE_INT    : str = "int";    break;
        case TYPE_DOUBLE : str = "double"; break;
        case TYPE_BOOL   : str = "bool";   break;
        case TYPE_STRING : str = "string"; break;
        default          : logman_fatal( "not handled" ); break;
        }
        return str;
    }

    void OptionParser::add_option( const string& name, const string& explanation, const string& type, OptionType opt_type, const string& default_value ) {

        passert_statement( name.size() != 0, "option name could not be left blank" );
        passert_statement_g( name[0] == '-', "options should start with a - [%s]", name.c_str() );
        passert_statement_g( get_option( name.c_str() ) == -1, "option [%s] already exists", name.c_str() );

        if( (opt_type != OP_SINGLE_INPUT) && (default_value!="") ) {
            logman_fatal_g( "cannot set default parameters for multi-input options [%s]", name.c_str() );
        }

        OptionItem opt;
        opt.name        = name;
        opt.explanation = explanation;
        opt.opt_type    = opt_type;
        if( opt.opt_type != OP_NO_INPUT )
            opt.data_type = data_type( type );
        else
            opt.data_type = TYPE_BOOL;

        if( (opt_type == OP_SINGLE_INPUT) && (default_value != "") ) {
            opt.push_value( default_value );
        }

        m_options.push_back( opt );

    }


    void OptionParser::print_help() const {
        printf("\n");
        for(int i=0; i<n_options(); i++ ) {
            const OptionItem& opt = get_option(i);
            switch( opt.opt_type ) {
            case OP_MULTI_INPUT:
                printf( "%-10s N %-10s | %s\n", opt.name.c_str(), data_type(opt.data_type).c_str(), opt.explanation.c_str() );
                break;
            case OP_SINGLE_INPUT: {
                if( !opt.n_values() )
                    printf( "%-12s %-10s | %s\n", opt.name.c_str(), data_type(opt.data_type).c_str(), opt.explanation.c_str() );
                else
                    printf( "%-12s %-10s | %s [default %s]\n", opt.name.c_str(), data_type(opt.data_type).c_str(), opt.explanation.c_str(), opt.get_value(0).c_str() );
            }
                break;
            case OP_NO_INPUT:
                printf( "%-12s %-10s | %s\n", opt.name.c_str(), "", opt.explanation.c_str() );
                break;
            }
        }
        printf("\n");
    }

    void OptionParser::print_options() const {

        printf( "\n" );
        for( int i=0; i<n_options(); i++ ) {
            const OptionItem& opt = get_option(i);
            printf( "%s:\n", opt.name.c_str() );
            for( int j=0; j<opt.n_values(); j++ ) {
                printf( "%20s\n", opt.get_value(j).c_str() );
            }
            printf( "\n" );
        }
        printf( "\n" );

    }

    int OptionParser::get_option( const char* str ) const {
        int oid = -1;
        for( int i=0; i<n_options(); i++ ) {
            if( !strcmp( str, m_options[i].name.c_str() ) ) {
                oid = i;
                break;
            }
        }
        return oid;
    }

    void OptionParser::parse( int argc, char** argv ) {

        int cnt = 1;
        while(  cnt < argc ) {
            int oid = get_option( argv[cnt] );
            if( oid == -1 ) {
                printf( "\n\ninvalid option [%s]\n\n", argv[cnt] );
                print_help();
                exit(0);
            }
            cnt++;

            OptionItem& opt = get_option(oid);

            if( opt.opt_type == OP_NO_INPUT ) {
                opt.clear();
                opt.push_value("1");
                continue;
            }

            if( cnt >= argc ) {
                printf( "false input options\n" );
                print_help();
                exit(1);
            }
            int n = 1;
            if( opt.opt_type == OP_MULTI_INPUT )
                n = atoi( argv[cnt++] );

            opt.clear();
            for( int i=0; i<n; i++ ) {
                if( cnt >= argc ) {
                    printf( "false input options\n" );
                    print_help();
                    exit(1);
                }
                opt.push_value( argv[cnt++] );
            }
        }
    }


}
