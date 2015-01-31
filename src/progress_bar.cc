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
#include <kortex/progress_bar.h>

#include <ostream>
#include <iostream>
// #include <ctime>

using std::string;
using std::ostream;

namespace kortex {

    progress_bar::progress_bar( int start, int end, int divisions )
        : m_start(start),
          m_current(start),
          m_end(end),
          m_progress(0),
          m_message("Progress: "),
          m_end_message(" "),
          m_done("-"),
          m_processing(">"),
          m_not_done(" "),
          m_limit("|") {
        m_divisions = std::min(divisions,end);
        timer.reset();
        // time(&m_starting_time);
        m_lock = 0;
    }

    void progress_bar::reset() {
        m_current = m_start;
        m_progress = 0;
        timer.reset();
        // time(&m_starting_time);
        m_lock = 0;
    }

    void progress_bar::reset( int start, int end, int divisions ) {
        m_start = start;
        m_current = start;
        m_end = end;
        m_divisions = divisions;
        m_progress = 0;
        m_lock = 0;
        // time(&m_starting_time);
        timer.reset();
    }

    std::ostream& progress_bar::operator>>( std::ostream& os ) {
        return out(os);
    }

    void progress_bar::report(int current) {
        m_current = current;
        if( m_lock ) return;
        m_lock = 1;
        out(std::cout);
        m_lock = 0;
    }

    void progress_bar::finalize() {
        std::cout<<m_message<<std::endl;
    }

    std::ostream& progress_bar::out(std::ostream& os) {
        if( m_current > (m_progress * (m_end - m_start) / m_divisions) ||
            m_current == m_end ) {
            m_progress++;
            os << m_message << m_limit;
            for(int c = 1; c <= m_divisions; ++c) {
                if(c < m_progress || m_current == m_end) {
                    os << m_done;
                } else if(c > m_progress) {
                    os << m_not_done;
                } else {
                    os << m_processing;
                }
            }
            os << m_limit;

            double duration  = timer.duration();
            double percent   = double(m_current-m_start)/double(m_end-m_start);
            double eta       = duration / percent;
            double remaining = eta - duration;

            int rem_day  = (int)(remaining/86400); remaining -= rem_day*86400;
            int rem_hour = (int)(remaining/3600);  remaining -= rem_hour*3600;
            int rem_min  = (int)(remaining/60);    remaining -= rem_min*60;
            int rem_sec  = (int)(remaining);

            int eta_day  = (int) (eta/86400); eta -= eta_day*86400;
            int eta_hour = (int) (eta/3600);  eta -= eta_hour*3600;
            int eta_min  = (int) (eta/60);    eta -= eta_min*60;
            int eta_sec  = (int) (eta);

            os<<" ";
            if( rem_day != 0 ) { os.width(2); os.fill('0'); os<<rem_day<<":"; }
            os.width(2); os.fill('0'); os<<rem_hour<<":";
            os.width(2); os.fill('0'); os<<rem_min <<":";
            os.width(2); os.fill('0'); os<<rem_sec <<" ";
            os<<"/";
            if( eta_day != 0 ) { os.width(2); os.fill('0'); os<<eta_day<<":"; }
            os.width(2); os.fill('0'); os<<eta_hour<<":";
            os.width(2); os.fill('0'); os<<eta_min <<":";
            os.width(2); os.fill('0'); os<<eta_sec <<" ";
            os << m_end_message;
            if(m_current == m_end) { os << "\n" << std::flush; }
            else                   { os << "\r" << std::flush; }
        }
        return os;
    }

    progress_bar& progress_bar::operator()(int current) {
        m_current = current;
        return *this;
    }

    void progress_bar::set_text(const std::string& text) {
        m_message = text;
    }

    void progress_bar::set_end_text( const std::string& text ) {
        m_end_message = text;
    }

    void progress_bar::set_format( const std::string& format_string ) {
        if(format_string.length() < 5)
            return;
        m_limit = format_string[0];
        m_done = format_string[1];
        m_processing = format_string[2];
        m_not_done = format_string[3];
    }

}
