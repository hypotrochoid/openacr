// (C) AlgoEngineering LLC 2008-2012
// (C) 2013-2019 NYSE | Intercontinental Exchange
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
// Source: cpp/amc/ptrary.cpp
//
// Created By: alexei.lebedev hayk.mkrtchyan
// Authors: alexei.lebedev
// Recent Changes: alexei.lebedev hayk.mkrtchyan
//

#include "include/amc.h"

void amc::tclass_Ptrary() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;

    Set(R, "$Rowid", "u32");

    InsVar(R, field.p_ctype     , "$Cpptype**", "$name_elems", "", "array of pointers");
    InsVar(R, field.p_ctype     , "u32", "$name_n", "", "array of pointers");
    InsVar(R, field.p_ctype     , "u32", "$name_max", "", "capacity of allocated array");

    amc::FPtrary &ptrary = *field.c_ptrary;
    if (ptrary.p_field->c_cascdel && !ptrary.unique) {
        amccheck(0,"amc.explicit_multi_delete"
                 <<Keyval("ptrary",ptrary.field)
                 <<Keyval("comment","Ptrary with unique:N cannot be cascdel"));
    }
}

void amc::tfunc_Ptrary_Cascdel() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;

    if (field.c_cascdel) {
        amc::FFunc& cascdel = amc::CreateCurFunc();
        Ins(&R, cascdel.body    , "// Clear $name_n so that calls to $field.Remove do not have to scan");
        Ins(&R, cascdel.body    , "// the array for pointers or shift anything.");
        Ins(&R, cascdel.body    , "// This is somewhat of a hack.");
        Ins(&R, cascdel.body    , "i32 n = $parname.$name_n;");
        Ins(&R, cascdel.body    , "$parname.$name_n = 0;");
        Ins(&R, cascdel.body    , "for (i32 i = n - 1; i >= 0; i--) {");
        Ins(&R, cascdel.body    , "    $Cpptype &row = *$parname.$name_elems[i];");
        if (ptrary.unique) {
            Ins(&R, cascdel.body, "    row.$parname_$name_in_ary = false;");
        }
        Ins(&R, cascdel.body, DeleteExpr(field,"$pararg","row")<<";");
        Ins(&R, cascdel.body    , "}");
    }
}

void amc::tfunc_Ptrary_EmptyQ() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& emptyq = amc::CreateCurFunc();
    Ins(&R, emptyq.ret  , "bool", false);
    Ins(&R, emptyq.proto, "$name_EmptyQ($Parent)", false);
    Ins(&R, emptyq.body, "return $parname.$name_n == 0;");
}

void amc::tfunc_Ptrary_Find() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FFunc& find = amc::CreateCurFunc();
    find.inl = true;
    Ins(&R, find.ret  , "$Cpptype*", false);
    Ins(&R, find.proto, "$name_Find($Parent, $Rowid t)", false);
    Ins(&R, find.body, "$Cpptype *retval = NULL;");
    Ins(&R, find.body, "u64 idx = t;");
    Ins(&R, find.body, "u64 lim = $parname.$name_n;");
    Ins(&R, find.body, "if (idx < lim) {");
    Ins(&R, find.body, "    retval = $parname.$name_elems[idx];");
    Ins(&R, find.body, "}");
    Ins(&R, find.body, "return retval;");
}

void amc::tfunc_Ptrary_Getary() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& getary = amc::CreateCurFunc();
    Ins(&R, getary.ret  , "algo::aryptr<$Cpptype*>", false);
    Ins(&R, getary.proto, "$name_Getary($Parent)", false);
    Ins(&R, getary.body, "return algo::aryptr<$Cpptype*>($parname.$name_elems, $parname.$name_n);");
}

void amc::tfunc_Ptrary_Init() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& init = amc::CreateCurFunc();
    init.inl = true;
    Ins(&R, init.body, "$parname.$name_elems = NULL; // ($field)");
    Ins(&R, init.body, "$parname.$name_n = 0; // ($field)");
    Ins(&R, init.body, "$parname.$name_max = 0; // ($field)");
}

void amc::tfunc_Ptrary_Insert() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;

    tempstr text;

    Ins(&R,         text, "// reserve space");
    Ins(&R,         text, "$name_Reserve($pararg, 1);");
    // - find insert point
    Ins(&R,         text, "u32 n  = $parname.$name_n;");
    Ins(&R,         text, "u32 at = n;");
    Ins(&R,         text, "$Cpptype* *elems = $parname.$name_elems;");
    // actual insertion
    Ins(&R,         text, "elems[at] = &row;");
    Ins(&R,         text, "$parname.$name_n = n+1;");

    // function body
    amc::FFunc& insert = amc::CreateCurFunc();
    Ins(&R, insert.ret  , "void", false);
    Ins(&R, insert.comment, "Insert pointer to row into array. Row must not already be in array.");
    Ins(&R, insert.comment, "If pointer is already in the array, it may be inserted twice.");//TODO UPDATE ME
    Ins(&R, insert.proto, "$name_Insert($Parent, $Cpptype& row)", false);
    if (ptrary.unique) {
        Ins(&R, insert.body, "if (bool_Update(row.$parname_$name_in_ary,true)) {");
        insert.body <<  text << eol;
        Ins(&R, insert.body, "}");
    } else {
        insert.body <<  text << eol;
    }
}

// -----------------------------------------------------------------------------

void amc::tfunc_Ptrary_InsertMaybe() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;
    if (ptrary.unique) {
        amc::FFunc& insmaybe = amc::CreateCurFunc();
        Ins(&R, insmaybe.ret  , "bool", false);
        Ins(&R, insmaybe.proto, "$name_InsertMaybe($Parent, $Cpptype& row)", false);
        Ins(&R, insmaybe.comment, "Insert pointer to row in array.");
        Ins(&R, insmaybe.comment, "If row is already in the array, do nothing.");
        Ins(&R, insmaybe.comment, "Return value: whether element was inserted into array.");
        Ins(&R, insmaybe.body, "bool retval = !row.$parname_$name_in_ary;");
        Ins(&R, insmaybe.body, "$name_Insert($pararg,row); // check is performed in _Insert again");
        Ins(&R, insmaybe.body, "return retval;");
    }
}

// -----------------------------------------------------------------------------

void amc::tfunc_Ptrary_ScanInsertMaybe() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;
    if (!ptrary.unique) {
        amc::FFunc& insmaybe = amc::CreateCurFunc(true);
        //AddProtoArg(insmaybe, "$Parent", "");
        AddProtoArg(insmaybe, Subst(R,"$Cpptype&"), "row");
        AddRetval(insmaybe, "bool", "retval","true");
        Ins(&R, insmaybe.comment, "Insert pointer to row in array.");
        Ins(&R, insmaybe.comment, "If row is already in the array, do nothing.");
        Ins(&R, insmaybe.comment, "Linear search is used to locate the element.");
        Ins(&R, insmaybe.comment, "Return value: whether element was inserted into array.");
        Ins(&R, insmaybe.body, "u32 n  = $parname.$name_n;");
        Ins(&R, insmaybe.body, "for (u32 i = 0; i < n; i++) {");
        Ins(&R, insmaybe.body, "    if ($parname.$name_elems[i] == &row) {");
        Ins(&R, insmaybe.body, "        retval = false;");
        Ins(&R, insmaybe.body, "        break;");
        Ins(&R, insmaybe.body, "    }");
        Ins(&R, insmaybe.body, "}");
        Ins(&R, insmaybe.body, "if (retval) {");
        Ins(&R, insmaybe.body, "    // reserve space");
        Ins(&R, insmaybe.body, "    $name_Reserve($pararg, 1);");
        Ins(&R, insmaybe.body, "    $parname.$name_elems[n] = &row;");
        Ins(&R, insmaybe.body, "    $parname.$name_n = n+1;");
        Ins(&R, insmaybe.body, "}");
    }
}

// -----------------------------------------------------------------------------

void amc::tfunc_Ptrary_N() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& nitems = amc::CreateCurFunc();
    Ins(&R, nitems.ret  , "i32", false);
    Ins(&R, nitems.proto, "$name_N($Cparent)", false);
    Ins(&R, nitems.body, "return $parname.$name_n;");
}

// -----------------------------------------------------------------------------

void amc::tfunc_Ptrary_Remove() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;

    tempstr text;
    Ins(&R, text, "int lim = $parname.$name_n;");
    Ins(&R, text, "$Cpptype* *elems = $parname.$name_elems;");
    Ins(&R, text, "// search backward, so that most recently added element is found first.");
    Ins(&R, text, "// if found, shift array.");
    Ins(&R, text, "for (int i = lim-1; i>=0; i--) {");
    Ins(&R, text, "    $Cpptype* elem = elems[i]; // fetch element");
    Ins(&R, text, "    if (elem == &row) {");
    Ins(&R, text, "        int j = i + 1;");
    Ins(&R, text, "        size_t nbytes = sizeof($Cpptype*) * (lim - j);");
    Ins(&R, text, "        memmove(elems + i, elems + j, nbytes);");
    Ins(&R, text, "        $parname.$name_n = lim - 1;");
    Ins(&R, text, "        break;");
    Ins(&R, text, "    }");
    Ins(&R, text, "}");

    amc::FFunc& rem = amc::CreateCurFunc();
    Ins(&R, rem.ret    , "void", false);
    Ins(&R, rem.proto  , "$name_Remove($Parent, $Cpptype& row)", false);
    if (ptrary.unique) {
        Ins(&R, rem.body, "if (bool_Update(row.$parname_$name_in_ary,false)) {");
        rem.body << text;
        Ins(&R, rem.body, "}");
    } else {
        rem.body << text;
    }
}

void amc::tfunc_Ptrary_RemoveAll() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FField &field = *amc::_db.genfield.p_field;
    amc::FPtrary &ptrary = *field.c_ptrary;

    amc::FFunc& removeall = amc::CreateCurFunc();
    removeall.inl = true;
    Ins(&R, removeall.ret  , "void", false);
    Ins(&R, removeall.proto, "$name_RemoveAll($Parent)", false);
    if (ptrary.unique) {
        Ins(&R, removeall.body, "for (u32 i = 0; i < $parname.$name_n; i++) {");
        Ins(&R, removeall.body, "    // mark all elements as not-in-array");
        Ins(&R, removeall.body, "    $parname.$name_elems[i]->$parname_$name_in_ary = false;");
        Ins(&R, removeall.body, "}");
    }
    Ins(&R, removeall.body, "$parname.$name_n = 0;");
}

void amc::tfunc_Ptrary_Reserve() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& reserve = amc::CreateCurFunc();
    Ins(&R, reserve.ret  , "void", false);
    Ins(&R, reserve.proto, "$name_Reserve($Parent, u32 n)", false);
    Ins(&R, reserve.body, "u32 old_max = $parname.$name_max;");
    Ins(&R, reserve.body, "if (UNLIKELY($parname.$name_n + n > old_max)) {");
    Ins(&R, reserve.body, "    u32 new_max  = u32_Max(4, old_max * 2);");
    Ins(&R, reserve.body, "    u32 old_size = old_max * sizeof($Cpptype*);");
    Ins(&R, reserve.body, "    u32 new_size = new_max * sizeof($Cpptype*);");
    Ins(&R, reserve.body, "    void *new_mem = $basepool_ReallocMem($parname.$name_elems, old_size, new_size);");
    Ins(&R, reserve.body, "    if (UNLIKELY(!new_mem)) {");
    Ins(&R, reserve.body, "        FatalErrorExit(\"$ns.out_of_memory  field:$field\");");
    Ins(&R, reserve.body, "    }");
    Ins(&R, reserve.body, "    $parname.$name_elems = ($Cpptype**)new_mem;");
    Ins(&R, reserve.body, "    $parname.$name_max = new_max;");
    Ins(&R, reserve.body, "}");
}

void amc::tfunc_Ptrary_Uninit() {
    algo_lib::Replscope &R = amc::_db.genfield.R;

    amc::FFunc& uninit = amc::CreateCurFunc();
    Ins(&R, uninit.body, "$basepool_FreeMem($parname.$name_elems, sizeof($Cpptype*)*$parname.$name_max); // ($field)");
}

void amc::tfunc_Ptrary_curs() {
    algo_lib::Replscope &R = amc::_db.genfield.R;
    amc::FNs &ns = *amc::_db.genfield.p_field->p_ctype->p_ns;

    Ins(&R, ns.curstext, "");
    Ins(&R, ns.curstext, "struct $Parname_$name_curs {// cursor");
    Ins(&R, ns.curstext, "    typedef $Cpptype ChildType;");
    Ins(&R, ns.curstext, "    $Cpptype** elems;");
    Ins(&R, ns.curstext, "    u32 n_elems;");
    Ins(&R, ns.curstext, "    u32 index;");
    Ins(&R, ns.curstext, "    $Parname_$name_curs() { elems=NULL; n_elems=0; index=0; }");
    Ins(&R, ns.curstext, "};");
    Ins(&R, ns.curstext, "");

    {
        amc::FFunc& curs_reset = amc::ind_func_GetOrCreate(Subst(R,"$field_curs.Reset"));
        curs_reset.inl = true;
        Ins(&R, curs_reset.ret  , "void", false);
        Ins(&R, curs_reset.proto, "$Parname_$name_curs_Reset($Parname_$name_curs &curs, $Partype &parent)", false);
        Ins(&R, curs_reset.body, "curs.elems = parent.$name_elems;");
        Ins(&R, curs_reset.body, "curs.n_elems = parent.$name_n;");
        Ins(&R, curs_reset.body, "curs.index = 0;");
    }

    {
        amc::FFunc& curs_validq = amc::ind_func_GetOrCreate(Subst(R,"$field_curs.ValidQ"));
        curs_validq.inl = true;
        Ins(&R, curs_validq.comment, "cursor points to valid item");
        Ins(&R, curs_validq.ret  , "bool", false);
        Ins(&R, curs_validq.proto, "$Parname_$name_curs_ValidQ($Parname_$name_curs &curs)", false);
        Ins(&R, curs_validq.body, "return curs.index < curs.n_elems;");
    }

    amc::FFunc& curs_next = amc::ind_func_GetOrCreate(Subst(R,"$field_curs.Next"));
    curs_next.inl = true;
    Ins(&R, curs_next.comment, "proceed to next item");
    Ins(&R, curs_next.ret  , "void", false);
    Ins(&R, curs_next.proto, "$Parname_$name_curs_Next($Parname_$name_curs &curs)", false);
    Ins(&R, curs_next.body, "curs.index++;");

    amc::FFunc& curs_access = amc::ind_func_GetOrCreate(Subst(R,"$field_curs.Access"));
    curs_access.inl = true;
    Ins(&R, curs_access.comment, "item access");
    Ins(&R, curs_access.ret  , "$Cpptype&", false);
    Ins(&R, curs_access.proto, "$Parname_$name_curs_Access($Parname_$name_curs &curs)", false);
    Ins(&R, curs_access.body, "return *curs.elems[curs.index];");
}
