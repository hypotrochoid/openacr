//
// include/gen/atfdb_gen.h
// Generated by AMC
//
// (C) AlgoEngineering LLC 2008-2013
// (C) NYSE | Intercontinental Exchange 2013-2016
//


#pragma once
#include "include/gen/algo_gen.h"
//#pragma endinclude

// --- atfdb_FieldIdEnum

enum atfdb_FieldIdEnum {            // atfdb.FieldId.value
     atfdb_FieldId_amctest     = 0
    ,atfdb_FieldId_comment     = 1
    ,atfdb_FieldId_normcheck   = 2
    ,atfdb_FieldId_unittest    = 3
    ,atfdb_FieldId_target      = 4
    ,atfdb_FieldId_testname    = 5
    ,atfdb_FieldId_value       = 6
};

enum { atfdb_FieldIdEnum_N = 7 };

namespace atfdb { struct Amctest; }
namespace atfdb { struct FieldId; }
namespace atfdb { struct Normcheck; }
namespace atfdb { struct Unittest; }
namespace atfdb {
    typedef algo::Smallstr50 UnittestPkey;
}//pkey typedefs
namespace atfdb {

// --- atfdb.Amctest
struct Amctest { // atfdb.Amctest
    algo::Smallstr50   amctest;   //
    algo::Comment      comment;   //
    Amctest();
};

bool                 Amctest_ReadFieldMaybe(atfdb::Amctest &parent, algo::strptr field, algo::strptr strval) __attribute__((nothrow));
// Read fields of atfdb::Amctest from an ascii string.
// The format of the string is an ssim Tuple
bool                 Amctest_ReadStrptrMaybe(atfdb::Amctest &parent, algo::strptr in_str);
// print string representation of atfdb::Amctest to string LHS, no header -- cprint:atfdb.Amctest.String
void                 Amctest_Print(atfdb::Amctest & row, algo::cstring &str) __attribute__((nothrow));

// --- atfdb.FieldId
#pragma pack(push,1)
struct FieldId { // atfdb.FieldId: Field read helper
    i32   value;   //   -1
    inline operator atfdb_FieldIdEnum() const;
    explicit FieldId(i32                            in_value);
    FieldId(atfdb_FieldIdEnum arg);
    FieldId();
};
#pragma pack(pop)

// Get value of field as enum type
atfdb_FieldIdEnum    value_GetEnum(const atfdb::FieldId& parent) __attribute__((nothrow));
// Set value of field from enum type.
void                 value_SetEnum(atfdb::FieldId& parent, atfdb_FieldIdEnum rhs) __attribute__((nothrow));
// Convert numeric value of field to one of predefined string constants.
// If string is found, return a static C string. Otherwise, return NULL.
const char*          value_ToCstr(const atfdb::FieldId& parent) __attribute__((nothrow));
// Convert value to a string. First, attempt conversion to a known string.
// If no string matches, print value as a numeric value.
void                 value_Print(const atfdb::FieldId& parent, algo::cstring &lhs) __attribute__((nothrow));
// Convert string to field.
// If the string is invalid, do not modify field and return false.
// In case of success, return true
bool                 value_SetStrptrMaybe(atfdb::FieldId& parent, algo::strptr rhs) __attribute__((nothrow));
// Convert string to field.
// If the string is invalid, set numeric value to DFLT
void                 value_SetStrptr(atfdb::FieldId& parent, algo::strptr rhs, atfdb_FieldIdEnum dflt) __attribute__((nothrow));
// Convert string to field. Return success value
bool                 value_ReadStrptrMaybe(atfdb::FieldId& parent, algo::strptr rhs) __attribute__((nothrow));

// Read fields of atfdb::FieldId from an ascii string.
// The format of the string is the format of the atfdb::FieldId's only field
bool                 FieldId_ReadStrptrMaybe(atfdb::FieldId &parent, algo::strptr in_str);
// Set all fields to initial values.
void                 FieldId_Init(atfdb::FieldId& parent);
// print string representation of atfdb::FieldId to string LHS, no header -- cprint:atfdb.FieldId.String
void                 FieldId_Print(atfdb::FieldId & row, algo::cstring &str) __attribute__((nothrow));

// --- atfdb.Normcheck
struct Normcheck { // atfdb.Normcheck
    algo::Smallstr50   normcheck;   //
    algo::Comment      comment;     //
    Normcheck();
};

bool                 Normcheck_ReadFieldMaybe(atfdb::Normcheck &parent, algo::strptr field, algo::strptr strval) __attribute__((nothrow));
// Read fields of atfdb::Normcheck from an ascii string.
// The format of the string is an ssim Tuple
bool                 Normcheck_ReadStrptrMaybe(atfdb::Normcheck &parent, algo::strptr in_str);
// print string representation of atfdb::Normcheck to string LHS, no header -- cprint:atfdb.Normcheck.String
void                 Normcheck_Print(atfdb::Normcheck & row, algo::cstring &str) __attribute__((nothrow));

// --- atfdb.Unittest
struct Unittest { // atfdb.Unittest
    algo::Smallstr50   unittest;   //
    algo::Comment      comment;    //
    Unittest();
};

algo::Smallstr16     target_Get(atfdb::Unittest& parent) __attribute__((__warn_unused_result__, nothrow));
algo::Smallstr16     Unittest_target_Get(strptr arg) __attribute__((nothrow));

algo::Smallstr50     testname_Get(atfdb::Unittest& parent) __attribute__((__warn_unused_result__, nothrow));
algo::Smallstr50     Unittest_testname_Get(strptr arg) __attribute__((nothrow));

tempstr              Unittest_Concat_target_testname( const algo::strptr& target ,const algo::strptr& testname );
bool                 Unittest_ReadFieldMaybe(atfdb::Unittest &parent, algo::strptr field, algo::strptr strval) __attribute__((nothrow));
// Read fields of atfdb::Unittest from an ascii string.
// The format of the string is an ssim Tuple
bool                 Unittest_ReadStrptrMaybe(atfdb::Unittest &parent, algo::strptr in_str);
// Read fields of atfdb::Unittest from attributes of ascii tuple TUPLE
bool                 Unittest_ReadTupleMaybe(atfdb::Unittest &parent, algo::Tuple &tuple) __attribute__((nothrow));
// print string representation of atfdb::Unittest to string LHS, no header -- cprint:atfdb.Unittest.String
void                 Unittest_Print(atfdb::Unittest & row, algo::cstring &str) __attribute__((nothrow));
} // end namespace atfdb
namespace algo {
inline algo::cstring &operator <<(algo::cstring &str, const atfdb::FieldId &row);// cfmt:atfdb.FieldId.String
}
