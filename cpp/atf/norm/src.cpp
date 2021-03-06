// (C) 2017-2019 NYSE | Intercontinental Exchange
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
// Target: atf_norm (exe) -- Normalization tests (see normcheck table)
// Exceptions: yes
// Source: cpp/atf/norm/src.cpp -- source file (cpp/, include/) normalizations including amc
//
// Created By: alexei.lebedev
// Recent Changes: alexei.lebedev
//

#include "include/atf_norm.h"

// -----------------------------------------------------------------------------

// indent any source files modified in the last commit
void atf_norm::normcheck_indent_srcfile() {
    // indent recently modified source files
    SysCmd("for X in $(git diff-tree --name-only  HEAD -r --no-commit-id cpp include"
           " | egrep -v '(cpp/gen/|include/gen|extern/)'); do if [ -f $X ]; then echo $X; fi; done "
           "> temp/atf_norm_indent.list",FailokQ(true));
    SysCmd("printf %s 'indenting files:';"
           " for X in $(head temp/atf_norm_indent.list); do"
           " printf '%c %s' ' ' $X; done; echo ...",FailokQ(false));
    SysCmd("bin/cpp-indent $(cat temp/atf_norm_indent.list) > temp/atf_norm_indent.log 2>&1",FailokQ(false));
    CheckCleanDirs("cpp include");
}

// -----------------------------------------------------------------------------

// indent all script files modified in the last commit
void atf_norm::normcheck_indent_script() {
    SysCmd("update-scriptfile");
    CheckCleanDirs(SsimFname(atf_norm::_db.cmdline.in, dmmeta_Ssimfile_ssimfile_dev_scriptfile));
    tempstr modfiles(SysEval("git diff-tree --name-only  HEAD -r --no-commit-id",FailokQ(true),1024*1024*10));
    ind_beg(Line_curs,line,modfiles) {
        if (atf_norm::FScriptfile *scriptfile = ind_scriptfile_Find(line)) {
            // indent script files -- there are few of them,
            // so it takes no time to indent them all.
            SysCmd(tempstr()<<"bin/cpp-indent "<<scriptfile->gitfile
                   <<" >> temp/atf_norm_indent.log 2>&1",FailokQ(false));
            CheckCleanDirs(scriptfile->gitfile);
        }
    }ind_end;
}

// -----------------------------------------------------------------------------

// update copyright info in source files
void atf_norm::normcheck_copyright() {
    command::src_hdr src_hdr;
    src_hdr.write=true;
    SysCmd(src_hdr_ToCmdline(src_hdr),FailokQ(false));
    CheckCleanDirs("cpp include");
}

// -----------------------------------------------------------------------------

// source code police
void atf_norm::normcheck_src_lim() {
    command::src_lim src_lim;
    src_lim.strayfile=true;
    src_lim.linelim=true;
    SysCmd(src_lim_ToCmdline(src_lim), FailokQ(false));
}

// -----------------------------------------------------------------------------

// run amc
void atf_norm::normcheck_amc() {
    command::amc amc;
    amc.report = false;
    SysCmd(amc_ToCmdline(amc),FailokQ(false));
    CheckCleanDirs("include/gen cpp/gen");
}

// -----------------------------------------------------------------------------

void atf_norm::normcheck_tempcode() {
    // extra double-quote needed to avoid this check
    // from failing on this file
    int rc=SysCmd("grep -RHn TEMP""CODE cpp include");
    if (rc==0) {// FOUND???
        prerr("SCALPEL LEFT IN PATIENT");
        prerr("It looks like some testing code made its way into the commit.");
        prerr("Please examine the found instances above carefully.");
        prerr("Diallowed code is indicated by the presence of the words TEMP""CODE.");
        algo_lib::_db.exit_code=1;
    }
}

// -----------------------------------------------------------------------------

static void GenCheck(strptr dir) {
    ind_beg(Dir_curs,file,DirFileJoin(dir,"*")) {
        int idx=FindStr(file.filename,"_gen.");
        if (idx!=-1) {
            atf_norm::FNs *ns=atf_norm::ind_ns_Find(ch_FirstN(file.filename,idx));
            if (!ns) {
                prlog("# success:N file doesn't appear to be generated by amc (to fix: pipe to  |grep ^acr_ed|sh)");
                prlog("acr_ed -del -srcfile:"<<file.pathname<<" -write");
                algo_lib::_db.exit_code=1;
            }
        }
    }ind_end;
}

void atf_norm::normcheck_stray_gen() {
    GenCheck("include/gen");
    GenCheck("cpp/gen");
}

// -----------------------------------------------------------------------------

static void BuildWith(strptr compiler) {
    if (SysEval(tempstr() << compiler << " --version",FailokQ(true),1024*10) != "") {
        command::abt_proc abt;
        abt.cmd.compiler = compiler;
        abt.cmd.cfg = dev_Cfg_cfg_release;
        abt.cmd.target.expr = "%";
        abt_ExecX(abt);
    } else {
        prlog("# "<<compiler<<" not installed, skipping build");
    }
}

// -----------------------------------------------------------------------------

void atf_norm::normcheck_build_clang() {
    BuildWith(dev_Compiler_compiler_clangPP);
}

// -----------------------------------------------------------------------------

void atf_norm::normcheck_build_gcc9() {
    BuildWith(dev_Compiler_compiler_gPP_9);
}
