//
// cpp/gen/lib_sql_gen.cpp
// Generated by AMC
//
// (C) AlgoEngineering LLC 2008-2013
// (C) NYSE | Intercontinental Exchange 2013-2016
//


#include "include/algo.h"  // hard-coded include
#include "include/gen/lib_sql_gen.h"
#include "include/gen/lib_sql_gen.inl.h"
#include "include/gen/algo_gen.h"
#include "include/gen/algo_gen.inl.h"
#include "include/gen/lib_prot_gen.h"
#include "include/gen/lib_prot_gen.inl.h"
#include "include/gen/algo_lib_gen.h"
#include "include/gen/algo_lib_gen.inl.h"
//#pragma endinclude
namespace lib_sql {
// Load statically available data into tables, register tables and database.
static void          InitReflection();
// find trace by row id (used to implement reflection)
static algo::ImrowPtr trace_RowidFind(int t) __attribute__((nothrow));
// Function return 1
static i32           trace_N() __attribute__((__warn_unused_result__, nothrow, pure));
static void          SizeCheck();
} // end namespace lib_sql

// --- lib_sql.FAttr..Uninit
void lib_sql::FAttr_Uninit(lib_sql::FAttr& attr) {
    lib_sql::FAttr &row = attr; (void)row;
    ind_attr_Remove(row); // remove attr from index ind_attr
}

// --- lib_sql.trace..Print
// print string representation of lib_sql::trace to string LHS, no header -- cprint:lib_sql.trace.String
void lib_sql::trace_Print(lib_sql::trace & row, algo::cstring &str) {
    algo::tempstr temp;
    str << "lib_sql.trace";
    (void)row;//only to avoid -Wunused-parameter
}

// --- lib_sql.FDb._db.MainArgs
// Main function
void lib_sql::MainArgs(int argc, char **argv) {
    lib_sql::Main(); // call through to user-defined main
    (void)argc;//only to avoid -Wunused-parameter
    (void)argv;//only to avoid -Wunused-parameter
}

// --- lib_sql.FDb._db.MainLoop
// Main loop.
void lib_sql::MainLoop() {
    SchedTime time(get_cycles());
    algo_lib::_db.clock          = time;
    do {
        algo_lib::_db.next_loop.value = algo_lib::_db.limit;
        algo_lib::Step(); // dependent namespace specified via (dev.targdep)
    } while (algo_lib::_db.next_loop < algo_lib::_db.limit);
}

// --- lib_sql.FDb._db.Step
// Main step
void lib_sql::Step() {
}

// --- lib_sql.FDb._db.InitReflection
// Load statically available data into tables, register tables and database.
static void lib_sql::InitReflection() {
    algo_lib::imdb_InsertMaybe(algo::Imdb("lib_sql", NULL, NULL, lib_sql::MainLoop, NULL, algo::Comment()));

    algo::Imtable t_trace;
    t_trace.imtable         = "lib_sql.trace";
    t_trace.ssimfile        = "";
    t_trace.size            = sizeof(lib_sql::trace);
    t_trace.comment.value   = "";
    t_trace.c_RowidFind     = trace_RowidFind;
    t_trace.NItems          = trace_N;
    t_trace.Print           = (algo::ImrowPrintFcn)lib_sql::trace_Print;
    algo_lib::imtable_InsertMaybe(t_trace);


    // -- load signatures of existing dispatches --
}

// --- lib_sql.FDb._db.StaticCheck
void lib_sql::StaticCheck() {
    algo_assert(_offset_of(lib_sql::FieldId, value) + sizeof(((lib_sql::FieldId*)0)->value) == sizeof(lib_sql::FieldId));
}

// --- lib_sql.FDb._db.InsertStrptrMaybe
// Parse strptr into known type and add to database.
// Return value is true unless an error occurs. If return value is false, algo_lib::_db.errtext has error text
bool lib_sql::InsertStrptrMaybe(algo::strptr str) {
    bool retval = true;
    (void)str;//only to avoid -Wunused-parameter
    return retval;
}

// --- lib_sql.FDb._db.LoadTuplesMaybe
// Load all finputs from given directory.
bool lib_sql::LoadTuplesMaybe(algo::strptr root) {
    bool retval = true;
    (void)root;//only to avoid -Wunused-parameter
    return retval;
}

// --- lib_sql.FDb._db.LoadSsimfileMaybe
// Load specified ssimfile.
bool lib_sql::LoadSsimfileMaybe(algo::strptr fname) {
    bool retval = true;
    if (FileQ(fname)) {
        retval = algo_lib::LoadTuplesFile(fname, lib_sql::InsertStrptrMaybe, true);
    }
    return retval;
}

// --- lib_sql.FDb._db.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Call Unref or Delete to cleanup partially inserted row.
bool lib_sql::_db_XrefMaybe() {
    bool retval = true;
    return retval;
}

// --- lib_sql.FDb.attr.Alloc
// Allocate memory for new default row.
// If out of memory, process is killed.
lib_sql::FAttr& lib_sql::attr_Alloc() {
    lib_sql::FAttr* row = attr_AllocMaybe();
    if (UNLIKELY(row == NULL)) {
        FatalErrorExit("lib_sql.out_of_mem  field:lib_sql.FDb.attr  comment:'Alloc failed'");
    }
    return *row;
}

// --- lib_sql.FDb.attr.AllocMaybe
// Allocate memory for new element. If out of memory, return NULL.
lib_sql::FAttr* lib_sql::attr_AllocMaybe() {
    lib_sql::FAttr *row = (lib_sql::FAttr*)attr_AllocMem();
    if (row) {
        new (row) lib_sql::FAttr; // call constructor
    }
    return row;
}

// --- lib_sql.FDb.attr.AllocMem
// Allocate space for one element. If no memory available, return NULL.
void* lib_sql::attr_AllocMem() {
    u64 new_nelems     = _db.attr_n+1;
    // compute level and index on level
    u64 bsr   = algo::u64_BitScanReverse(new_nelems);
    u64 base  = u64(1)<<bsr;
    u64 index = new_nelems-base;
    void *ret = NULL;
    // if level doesn't exist yet, create it
    lib_sql::FAttr*  lev   = NULL;
    if (bsr < 32) {
        lev = _db.attr_lary[bsr];
        if (!lev) {
            lev=(lib_sql::FAttr*)algo_lib::malloc_AllocMem(sizeof(lib_sql::FAttr) * (u64(1)<<bsr));
            _db.attr_lary[bsr] = lev;
        }
    }
    // allocate element from this level
    if (lev) {
        _db.attr_n = new_nelems;
        ret = lev + index;
    }
    return ret;
}

// --- lib_sql.FDb.attr.RemoveAll
// Remove all elements from Lary
void lib_sql::attr_RemoveAll() {
    for (u64 n = _db.attr_n; n>0; ) {
        n--;
        attr_qFind(u64(n)).~FAttr(); // destroy last element
        _db.attr_n = n;
    }
}

// --- lib_sql.FDb.attr.RemoveLast
// Delete last element of array. Do nothing if array is empty.
void lib_sql::attr_RemoveLast() {
    u64 n = _db.attr_n;
    if (n > 0) {
        n -= 1;
        attr_qFind(u64(n)).~FAttr();
        _db.attr_n = n;
    }
}

// --- lib_sql.FDb.attr.XrefMaybe
// Insert row into all appropriate indices. If error occurs, store error
// in algo_lib::_db.errtext and return false. Call Unref or Delete to cleanup partially inserted row.
bool lib_sql::attr_XrefMaybe(lib_sql::FAttr &row) {
    bool retval = true;
    (void)row;
    // insert attr into index ind_attr
    if (true) { // user-defined insert condition
        bool success = ind_attr_InsertMaybe(row);
        if (UNLIKELY(!success)) {
            ch_RemoveAll(algo_lib::_db.errtext);
            algo_lib::_db.errtext << "lib_sql.duplicate_key  xref:lib_sql.FDb.ind_attr"; // check for duplicate key
            return false;
        }
    }
    return retval;
}

// --- lib_sql.FDb.ind_attr.Find
// Find row by key. Return NULL if not found.
lib_sql::FAttr* lib_sql::ind_attr_Find(const algo::strptr& key) {
    u32 index = cstring_Hash(0, key) & (_db.ind_attr_buckets_n - 1);
    lib_sql::FAttr* *e = &_db.ind_attr_buckets_elems[index];
    lib_sql::FAttr* ret=NULL;
    do {
        ret       = *e;
        bool done = !ret || (*ret).attr == key;
        if (done) break;
        e         = &ret->ind_attr_next;
    } while (true);
    return ret;
}

// --- lib_sql.FDb.ind_attr.FindX
// Look up row by key and return reference. Throw exception if not found
lib_sql::FAttr& lib_sql::ind_attr_FindX(const algo::strptr& key) {
    lib_sql::FAttr* ret = ind_attr_Find(key);
    vrfy(ret, tempstr() << "lib_sql.key_error  table:ind_attr  key:'"<<key<<"'  comment:'key not found'");
    return *ret;
}

// --- lib_sql.FDb.ind_attr.GetOrCreate
// Find row by key. If not found, create and x-reference a new row with with this key.
lib_sql::FAttr& lib_sql::ind_attr_GetOrCreate(const algo::strptr& key) {
    lib_sql::FAttr* ret = ind_attr_Find(key);
    if (!ret) { //  if memory alloc fails, process dies; if insert fails, function returns NULL.
        ret         = &attr_Alloc();
        (*ret).attr = key;
        bool good = attr_XrefMaybe(*ret);
        if (!good) {
            attr_RemoveLast(); // delete offending row, any existing xrefs are cleared
            ret = NULL;
        }
    }
    return *ret;
}

// --- lib_sql.FDb.ind_attr.InsertMaybe
// Insert row into hash table. Return true if row is reachable through the hash after the function completes.
bool lib_sql::ind_attr_InsertMaybe(lib_sql::FAttr& row) {
    ind_attr_Reserve(1);
    bool retval = true; // if already in hash, InsertMaybe returns true
    if (LIKELY(row.ind_attr_next == (lib_sql::FAttr*)-1)) {// check if in hash already
        u32 index = cstring_Hash(0, row.attr) & (_db.ind_attr_buckets_n - 1);
        lib_sql::FAttr* *prev = &_db.ind_attr_buckets_elems[index];
        do {
            lib_sql::FAttr* ret = *prev;
            if (!ret) { // exit condition 1: reached the end of the list
                break;
            }
            if ((*ret).attr == row.attr) { // exit condition 2: found matching key
                retval = false;
                break;
            }
            prev = &ret->ind_attr_next;
        } while (true);
        if (retval) {
            row.ind_attr_next = *prev;
            _db.ind_attr_n++;
            *prev = &row;
        }
    }
    return retval;
}

// --- lib_sql.FDb.ind_attr.Remove
// Remove reference to element from hash index. If element is not in hash, do nothing
void lib_sql::ind_attr_Remove(lib_sql::FAttr& row) {
    if (LIKELY(row.ind_attr_next != (lib_sql::FAttr*)-1)) {// check if in hash already
        u32 index = cstring_Hash(0, row.attr) & (_db.ind_attr_buckets_n - 1);
        lib_sql::FAttr* *prev = &_db.ind_attr_buckets_elems[index]; // addr of pointer to current element
        while (lib_sql::FAttr *next = *prev) {                          // scan the collision chain for our element
            if (next == &row) {        // found it?
                *prev = next->ind_attr_next; // unlink (singly linked list)
                _db.ind_attr_n--;
                row.ind_attr_next = (lib_sql::FAttr*)-1;// not-in-hash
                break;
            }
            prev = &next->ind_attr_next;
        }
    }
}

// --- lib_sql.FDb.ind_attr.Reserve
// Reserve enough room in the hash for N more elements. Return success code.
void lib_sql::ind_attr_Reserve(int n) {
    u32 old_nbuckets = _db.ind_attr_buckets_n;
    u32 new_nelems   = _db.ind_attr_n + n;
    // # of elements has to be roughly equal to the number of buckets
    if (new_nelems > old_nbuckets) {
        int new_nbuckets = i32_Max(BumpToPow2(new_nelems), u32(4));
        u32 old_size = old_nbuckets * sizeof(lib_sql::FAttr*);
        u32 new_size = new_nbuckets * sizeof(lib_sql::FAttr*);
        // allocate new array. we don't use Realloc since copying is not needed and factor of 2 probably
        // means new memory will have to be allocated anyway
        lib_sql::FAttr* *new_buckets = (lib_sql::FAttr**)algo_lib::malloc_AllocMem(new_size);
        if (UNLIKELY(!new_buckets)) {
            FatalErrorExit("lib_sql.out_of_memory  field:lib_sql.FDb.ind_attr");
        }
        memset(new_buckets, 0, new_size); // clear pointers
        // rehash all entries
        for (int i = 0; i < _db.ind_attr_buckets_n; i++) {
            lib_sql::FAttr* elem = _db.ind_attr_buckets_elems[i];
            while (elem) {
                lib_sql::FAttr &row        = *elem;
                lib_sql::FAttr* next       = row.ind_attr_next;
                u32 index          = cstring_Hash(0, row.attr) & (new_nbuckets-1);
                row.ind_attr_next     = new_buckets[index];
                new_buckets[index] = &row;
                elem               = next;
            }
        }
        // free old array
        algo_lib::malloc_FreeMem(_db.ind_attr_buckets_elems, old_size);
        _db.ind_attr_buckets_elems = new_buckets;
        _db.ind_attr_buckets_n = new_nbuckets;
    }
}

// --- lib_sql.FDb.trace.RowidFind
// find trace by row id (used to implement reflection)
static algo::ImrowPtr lib_sql::trace_RowidFind(int t) {
    return algo::ImrowPtr(t==0 ? u64(&_db.trace) : u64(0));
}

// --- lib_sql.FDb.trace.N
// Function return 1
inline static i32 lib_sql::trace_N() {
    return 1;
}

// --- lib_sql.FDb..Init
// Set all fields to initial values.
void lib_sql::FDb_Init() {
    // initialize LAry attr (lib_sql.FDb.attr)
    _db.attr_n = 0;
    memset(_db.attr_lary, 0, sizeof(_db.attr_lary)); // zero out all level pointers
    lib_sql::FAttr* attr_first = (lib_sql::FAttr*)algo_lib::malloc_AllocMem(sizeof(lib_sql::FAttr) * (u64(1)<<4));
    if (!attr_first) {
        FatalErrorExit("out of memory");
    }
    for (int i = 0; i < 4; i++) {
        _db.attr_lary[i]  = attr_first;
        attr_first    += 1ULL<<i;
    }
    // initialize hash table for lib_sql::FAttr;
    _db.ind_attr_n             	= 0; // (lib_sql.FDb.ind_attr)
    _db.ind_attr_buckets_n     	= 4; // (lib_sql.FDb.ind_attr)
    _db.ind_attr_buckets_elems 	= (lib_sql::FAttr**)algo_lib::malloc_AllocMem(sizeof(lib_sql::FAttr*)*_db.ind_attr_buckets_n); // initial buckets (lib_sql.FDb.ind_attr)
    if (!_db.ind_attr_buckets_elems) {
        FatalErrorExit("out of memory"); // (lib_sql.FDb.ind_attr)
    }
    memset(_db.ind_attr_buckets_elems, 0, sizeof(lib_sql::FAttr*)*_db.ind_attr_buckets_n); // (lib_sql.FDb.ind_attr)

    lib_sql::InitReflection();
}

// --- lib_sql.FDb..Uninit
void lib_sql::FDb_Uninit() {
    lib_sql::FDb &row = _db; (void)row;

    // lib_sql.FDb.ind_attr.Uninit (Thash)  //
    // skip destruction of ind_attr in global scope

    // lib_sql.FDb.attr.Uninit (Lary)  //
    // skip destruction in global scope
}

// --- lib_sql.FieldId.value.ToCstr
// Convert numeric value of field to one of predefined string constants.
// If string is found, return a static C string. Otherwise, return NULL.
const char* lib_sql::value_ToCstr(const lib_sql::FieldId& parent) {
    const char *ret = NULL;
    switch(value_GetEnum(parent)) {
        case lib_sql_FieldId_value         : ret = "value";  break;
    }
    return ret;
}

// --- lib_sql.FieldId.value.Print
// Convert value to a string. First, attempt conversion to a known string.
// If no string matches, print value as a numeric value.
void lib_sql::value_Print(const lib_sql::FieldId& parent, algo::cstring &lhs) {
    const char *strval = value_ToCstr(parent);
    if (strval) {
        lhs << strval;
    } else {
        lhs << parent.value;
    }
}

// --- lib_sql.FieldId.value.SetStrptrMaybe
// Convert string to field.
// If the string is invalid, do not modify field and return false.
// In case of success, return true
bool lib_sql::value_SetStrptrMaybe(lib_sql::FieldId& parent, algo::strptr rhs) {
    bool ret = false;
    switch (elems_N(rhs)) {
        case 5: {
            switch (u64(ReadLE32(rhs.elems))|(u64(rhs[4])<<32)) {
                case LE_STR5('v','a','l','u','e'): {
                    value_SetEnum(parent,lib_sql_FieldId_value); ret = true; break;
                }
            }
            break;
        }
    }
    return ret;
}

// --- lib_sql.FieldId.value.SetStrptr
// Convert string to field.
// If the string is invalid, set numeric value to DFLT
void lib_sql::value_SetStrptr(lib_sql::FieldId& parent, algo::strptr rhs, lib_sql_FieldIdEnum dflt) {
    if (!value_SetStrptrMaybe(parent,rhs)) value_SetEnum(parent,dflt);
}

// --- lib_sql.FieldId.value.ReadStrptrMaybe
// Convert string to field. Return success value
bool lib_sql::value_ReadStrptrMaybe(lib_sql::FieldId& parent, algo::strptr rhs) {
    bool retval = false;
    retval = value_SetStrptrMaybe(parent,rhs); // try symbol conversion
    if (!retval) { // didn't work? try reading as underlying type
        retval = i32_ReadStrptrMaybe(parent.value,rhs);
    }
    return retval;
}

// --- lib_sql.FieldId..ReadStrptrMaybe
// Read fields of lib_sql::FieldId from an ascii string.
// The format of the string is the format of the lib_sql::FieldId's only field
bool lib_sql::FieldId_ReadStrptrMaybe(lib_sql::FieldId &parent, algo::strptr in_str) {
    bool retval = true;
    retval = retval && lib_sql::value_ReadStrptrMaybe(parent, in_str);
    return retval;
}

// --- lib_sql.FieldId..Print
// print string representation of lib_sql::FieldId to string LHS, no header -- cprint:lib_sql.FieldId.String
void lib_sql::FieldId_Print(lib_sql::FieldId & row, algo::cstring &str) {
    lib_sql::value_Print(row, str);
}

// --- lib_sql...SizeCheck
inline static void lib_sql::SizeCheck() {
}
