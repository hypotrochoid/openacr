// (C) 2018-2019 NYSE | Intercontinental Exchange
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contacting ICE: <https://www.theice.com/contact>
//
// Target: amc (exe) -- Algo Model Compiler: generate code under include/gen and cpp/gen
// Exceptions: NO
// Source: cpp/amc/outfile.cpp
//
// Created By: alexei.lebedev
// Recent Changes: alexei.lebedev
//

#include "include/amc.h"

// -----------------------------------------------------------------------------

// Write output file to disk
// and deallocate memory associated with it
void amc::gen_ns_write() {
    amc::FNs &ns=*amc::_db.c_ns;
    if (!amc::QueryModeQ() && ch_N(amc::_db.cmdline.out_dir)) {
        int nbefore=algo_lib::_db.stringtofile_nwrite;
        ind_beg(amc::ns_c_outfile_curs, outfile,ns) {
            amc::_db.report.n_cppfile++;
            // save to preassigned filename, or out dir if overridden
            (void)SafeStringToFile(outfile.text, DirFileJoin(amc::_db.cmdline.out_dir, outfile.outfile));
            frep_(i,ch_N(outfile.text)) {
                amc::_db.report.n_cppline += ch_qFind(outfile.text, i) == '\n';
            }
        }ind_end;
        amc::_db.report.n_filemod += algo_lib::_db.stringtofile_nwrite - nbefore;
        c_outfile_Cascdel(ns);
        ns.inl=NULL;
        ns.hdr=NULL;
        ns.cpp=NULL;
    }
}

// -----------------------------------------------------------------------------

// Create outfile record for specified filename
// T here is one outfile per generated output file
amc::FOutfile &amc::outfile_Create(strptr filename) {
    amc::FOutfile &outfile = amc::outfile_Alloc();
    outfile.outfile=filename;
    (void)outfile_XrefMaybe(outfile);
    if (!ch_N(outfile.text)) {
        outfile.text <<
            "//\n"
            "// "<<filename<<"\n"
            "// Generated by AMC\n"
            "//\n"
            "// (C) AlgoEngineering LLC 2008-2013\n"
            "// (C) NYSE | Intercontinental Exchange 2013-2016\n"
            "//\n"
            "\n"
            "\n";
    }
    return outfile;
}
