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
#ifndef KORTEX_PROGRESS_BAR_H
#define KORTEX_PROGRESS_BAR_H

#include <string>
#include <kortex/timer.h>

namespace kortex {

    /// displays a progress bar
    class progress_bar {
    public:
        explicit progress_bar(int start, int end, int divisions);

        void reset();
        void reset(int start, int end, int divisions);

        void set_text(const std::string& text);
        void set_end_text( const std::string& text);
        void set_format(const std::string& format_string);

        void report(int current);
        void finalize();

        std::ostream& operator>>(std::ostream& os);

    private:
        int m_start;
        int m_current;
        int m_end;
        int m_divisions;
        mutable int m_progress;

        int m_lock;

        Timer       timer;

        std::string m_message;
        std::string m_end_message;
        std::string m_done;
        std::string m_processing;
        std::string m_not_done;
        std::string m_limit;

        std::ostream& out(std::ostream& os);
        progress_bar& operator()(int current);
    };

    inline std::ostream& operator<<(std::ostream& os, progress_bar& pb) {
        return pb >> os;
    }

}


#endif
