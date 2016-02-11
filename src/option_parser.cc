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

    void OptionParser::add_separator() {
        OptionItem opt;
        opt.name = "reserved";
        opt.opt_type = OP_SEPARATOR;
        m_options.push_back( opt );
    }

    void OptionParser::set_default( const string& name, const string& v0 ) {
        int oid = get_option( name );
        passert_statement_g( oid != -1, "option does not exist [%s]", name.c_str() );
        OptionItem& opt = get_option(oid);
        opt.push_value(v0);
    }
    void OptionParser::set_default( const string& name, const string& v0, const string& v1 ) {
        int oid = get_option( name );
        passert_statement_g( oid != -1, "option does not exist [%s]", name.c_str() );
        OptionItem& opt = get_option(oid);
        opt.push_value(v0);
        opt.push_value(v1);
    }
    void OptionParser::set_default( const string& name,
                                    const string& v0, const string& v1, const string& v2 ) {
        int oid = get_option( name );
        passert_statement_g( oid != -1, "option does not exist [%s]", name.c_str() );
        OptionItem& opt = get_option(oid);
        opt.push_value(v0);
        opt.push_value(v1);
        opt.push_value(v2);
    }
    void OptionParser::set_default( const string& name,
                                    const string& v0, const string& v1,
                                    const string& v2, const string& v3 ) {
        int oid = get_option( name );
        passert_statement_g( oid != -1, "option does not exist [%s]", name.c_str() );
        OptionItem& opt = get_option(oid);
        opt.push_value(v0);
        opt.push_value(v1);
        opt.push_value(v2);
        opt.push_value(v3);
    }

    void OptionParser::print_help() const {
        printf("\n");

        int opt_len = 3;
        int exp_len = 20;
        for(int i=0; i<n_options(); i++ ) {
            const OptionItem& opt = get_option(i);
            exp_len = std::min( 60, std::max( exp_len, (int)opt.explanation.size() ) );
            opt_len = std::min( 16, std::max( opt_len, (int)opt.name.size() ) );
        }

        for( int i=0; i<n_options(); i++ ) {
            const OptionItem& opt = get_option(i);
            switch( opt.opt_type ) {
            case OP_MULTI_INPUT:
                if( !opt.n_values() ) {
                    printf( "%-*s  * %-7s| %-*s |\n", opt_len, opt.name.c_str(), data_type(opt.data_type).c_str(), exp_len, opt.explanation.c_str() );
                } else {
                    printf( "%-*s  * %-7s| %-*s | def: ", opt_len, opt.name.c_str(), data_type(opt.data_type).c_str(), exp_len, opt.explanation.c_str() );
                    for( int j=0; j<opt.n_values(); j++ )
                        printf( "%s ", opt.get_value(j).c_str() );
                    printf("\n");
                }
                break;
            case OP_SINGLE_INPUT: {
                if( !opt.n_values() )
                    printf( "%-*s  %-7s| %-*s |\n", opt_len+2, opt.name.c_str(), data_type(opt.data_type).c_str(), exp_len, opt.explanation.c_str() );
                else
                    printf( "%-*s  %-7s| %-*s | def: %s\n", opt_len+2, opt.name.c_str(), data_type(opt.data_type).c_str(), exp_len, opt.explanation.c_str(), opt.get_value(0).c_str() );
            }
                break;
            case OP_NO_INPUT:
                printf( "%-*s  %-7s| %-*s |\n", opt_len+2, opt.name.c_str(), "", exp_len, opt.explanation.c_str() );
                break;
            case OP_SEPARATOR:
                printf( "\n" );
                break;
            }
        }
        printf("\n");
    }

    void OptionParser::print_options() const {
        printf( "\n" );
        for( int i=0; i<n_options(); i++ ) {
            const OptionItem& opt = get_option(i);
            if( opt.opt_type == OP_MULTI_INPUT ) {
                if( opt.n_values() == 0 ) {
                    printf( "%-20s: %-20s\n", opt.name.c_str(), "" );
                }
                for( int j=0; j<opt.n_values(); j++ ) {
                    if( j == 0 )
                        printf( "%-20s: %-20s\n", opt.name.c_str(), opt.get_value(j).c_str() );
                    else
                        printf( "%-20s  %-20s\n", "", opt.get_value(j).c_str() );
                }
            } else if( opt.opt_type == OP_SINGLE_INPUT ) {
                if( opt.n_values() == 0 )
                    printf( "%-20s: %-20s\n", opt.name.c_str(), "" );
                else
                    printf( "%-20s: %-20s\n", opt.name.c_str(), opt.get_value(0).c_str() );
            } else {
                printf( "%-20s: %-20d\n", opt.name.c_str(), this->getb(opt.name.c_str()) );
            }
        }
        printf( "\n" );
    }

    int OptionParser::get_option( const string& str ) const {
        int oid = -1;
        for( int i=0; i<n_options(); i++ ) {
            if( !strcmp( str.c_str(), m_options[i].name.c_str() ) ) {
                oid = i;
                break;
            }
        }
        return oid;
    }

    int OptionParser::num_option_arguments( int argc, char** argv, int start ) const {

        if( start < 0 || start >= argc ) {
            logman_warning_g( "passing oob starting point [ %d, %d]", start, argc );
            return -1;
        }

        int counter = 0;
        while( start < argc ) {
            const char* str = argv[start++];
            if( str[0] != '-' ) {
                counter++;
                continue;
            }
            if( strlen(str) <= 1 )
                break;
            if( isdigit(str[1]) || str[1] =='.' )
                counter++;
            else
                break;
        }

        return counter;
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
                printf( "something wrong with the option arguments [%s]", opt.name.c_str() );
                print_help();
                exit(1);
            }

            int n;
            if( opt.opt_type == OP_SINGLE_INPUT ) n = 1;
            else                                  n = num_option_arguments( argc, argv, cnt );
            if( n == -1 ) {
                printf( "something wrong with the number of option arguments [%s]", opt.name.c_str() );
                print_help();
                exit(1);
            }

            opt.clear();
            for( int i=0; i<n; i++ ) {
                if( cnt >= argc ) {
                    printf( "something wrong with the option arguments [%s]", opt.name.c_str() );
                    print_help();
                    exit(1);
                }
                opt.push_value( argv[cnt++] );
            }
        }
    }

    double OptionParser::getd( const char* str, int id ) const {
        int oid = get_option( str );
        if( oid == -1 )
            logman_fatal_g( "no such key [opt %s]\n", str );
        const OptionItem& opt = get_option(oid);
        double v;
        if( opt.data_type != get_type(v) )
            logman_fatal_g( "option type and requested data type does not match [opt %s]\n", opt.name.c_str() );
        passert_statement_g( opt.opt_type == OP_MULTI_INPUT,
                             "need to be multi-element [%s]", str );
        passert_boundary( id, 0, opt.n_values() );
        in_value( opt.get_value(id).c_str(), v );
        return v;
    }

    int OptionParser::geti( const char* str, int id ) const {
        int oid = get_option( str );
        if( oid == -1 )
            logman_fatal_g( "no such key [opt %s]\n", str );
        const OptionItem& opt = get_option(oid);
        int v;
        if( opt.data_type != get_type(v) )
            logman_fatal_g( "option type and requested data type does not match [opt %s]\n", opt.name.c_str() );
        passert_statement_g( opt.opt_type == OP_MULTI_INPUT,
                             "need to be multi-element [%s]", str );
        passert_boundary( id, 0, opt.n_values() );
        in_value( opt.get_value(id).c_str(), v );
        return v;
    }

    string OptionParser::gets( const char* str, int id ) const {
        int oid = get_option( str );
        if( oid == -1 )
            logman_fatal_g( "no such key [opt %s]\n", str );
        const OptionItem& opt = get_option(oid);
        string v;
        if( opt.data_type != get_type(v) )
            logman_fatal_g( "option type and requested data type does not match [opt %s]\n", opt.name.c_str() );
        passert_statement_g( opt.opt_type == OP_MULTI_INPUT,
                             "need to be multi-element [%s]", str );
        passert_boundary( id, 0, opt.n_values() );
        v = opt.get_value(id);
        return v;
    }


}
