+++ top ----------------------------------------
| outfile      - "vcsn_wrap.cxx"
| name         - "vcsn"
| module       - 0xb7cf9cc8
| classes      - 0xb7cd61f8
| infile       - "vcsn.i"
| outfile_h    - "vcsn_wrap.h"

      +++ include ----------------------------------------
      | name         - "../../swig-1.3.27/Lib/swig.swg"

            +++ namespace ----------------------------------------
            | sym:name     - "std"
            | name         - "std"
            | csym:nextSibling - 0xb7cec638
            | sym:nextSibling - 0xb7cec638
            | typescope    - 0xb7cf9d38
            | symtab       - 0xb7cd4338
            | sym:symtab   - 0xb7cd02e8
            | sym:overname - "__SWIG_0"
            | 
            +++ typemap ----------------------------------------
            | code         - "delete [] $1;"
            | method       - "newfree"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd44c8
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($1) delete [] $1;\n  if ($input) {\n     $1 = ($1_type) (new char[strlen($input)+1]);\n     strcpy((char *) $1,$input);\n  } else {\n     $1 = 0;\n  }\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4618
                  | 
            +++ typemap ----------------------------------------
            | kwargs       - 0xb7cd4688
            | code         - "{\n  if ($input) {\n     $1 = ($1_type) (new char[strlen($input)+1]);\n     strcpy((char *) $1,$input);\n  } else {\n     $1 = 0;\n  }\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4748
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($1) delete [] $1;\n  if ($input) {\n     $1 = ($1_type) (new char[strlen($input)+1]);\n     strcpy((char *) $1,$input);\n  } else {\n     $1 = 0;\n  }\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4848
                  | 
            +++ typemap ----------------------------------------
            | kwargs       - 0xb7cd48b8
            | code         - "{\n  if ($input) {\n     $1 = ($1_type) (new char[strlen($input)+1]);\n     strcpy((char *) $1,$input);\n  } else {\n     $1 = 0;\n  }\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4978
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($input) strncpy($1,$input,$1_dim0);\n  else $1[0] = 0;\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4a88
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($input) strncpy($1,$input,$1_dim0);\n  else $1[0] = 0;\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4b98
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($input) strcpy($1,$input);\n  else $1[0] = 0;\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4c88
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  if ($input) strcpy($1,$input);\n  else $1[0] = 0;\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4d78
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  size_t ii;\n  $1_basetype *b = ($1_basetype *) $1;\n  for (ii = 0; ii < (size_t)$1_size; ii++) b[ii] = *(($1_basetype *) $input + ii);\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4e88
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  size_t ii;\n  $1_basetype *b = ($1_basetype *) $1;\n  for (ii = 0; ii < (size_t)$1_size; ii++) b[ii] = *(($1_basetype *) $input + ii);\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd4f98
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  $basetype (*inp)[$dim1] = ($basetype (*)[$dim1])($input);\n  $basetype (*dest)[$dim1] = ($basetype (*)[$dim1])($1);\n  size_t ii = 0;\n  for (; ii < $dim0; ++ii) {\n    $basetype *ip = inp[ii];\n    $basetype *dp = dest[ii];\n    size_t jj = 0;\n    for (; jj < $dim1; ++jj) dp[jj] = ip[jj];\n  }\n}"
            | method       - "memberin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd50c8
                  | 
            +++ typemap ----------------------------------------
            | code         - "{\n  $basetype (*inp)[$dim1] = ($basetype (*)[$dim1])($input);\n  $basetype (*dest)[$dim1] = ($basetype (*)[$dim1])($1);\n  size_t ii = 0;\n  for (; ii < $dim0; ++ii) {\n    $basetype *ip = inp[ii];\n    $basetype *dp = dest[ii];\n    size_t jj = 0;\n    for (; jj < $dim1; ++jj) dp[jj] = ip[jj];\n  }\n}"
            | method       - "globalin"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd51f8
                  | 
            +++ typemap ----------------------------------------
            | kwargs       - 0xb7cd5278
            | code         - ""
            | method       - "in"

                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd5348
                  | 
                  +++ typemapitem ----------------------------------------
                  | pattern      - 0xb7cd53f8
                  | 
            +++ insert ----------------------------------------
            | code         - "\n#ifdef __cplusplus\ntemplate<class T> class SwigValueWrapper {\n    T *tt;\npublic:\n    SwigValueWrapper() : tt(0) { }\n    SwigValueWrapper(const SwigValueWrapper<T>& rhs) : tt(new T(*rhs.tt)) { }\n    SwigValueWrapper(const T& t) : tt(new T(t)) { }\n    ~SwigValueWrapper() { delete tt; } \n    SwigValueWrapper& operator=(const T& t) { delete tt; tt = new T(t); return *this; }\n    operator T&() const { return *tt; }\n    T *operator&() { return tt; }\nprivate:\n    SwigValueWrapper& operator=(const SwigValueWrapper<T>& rhs);\n};\n#endif\n"
            | section      - "runtime"
            | 
            +++ insert ----------------------------------------
            | code         - "/***********************************************************************\n *\n *  This section contains generic SWIG labels for method/variable\n *  declarations/attributes, and other compiler dependent labels.\n *\n ************************************************************************/\n\n/* template workaround for compilers that cannot correctly implement the C++ standard */\n#ifndef SWIGTEMPLATEDISAMBIGUATOR\n#  if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)\n#    define SWIGTEMPLATEDISAMBIGUATOR template\n#  else\n#    define SWIGTEMPLATEDISAMBIGUATOR \n#  endif\n#endif\n\n/* inline attribute */\n#ifndef SWIGINLINE\n# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))\n#   define SWIGINLINE inline\n# else\n#   define SWIGINLINE\n# endif\n#endif\n\n/* attribute recognised by some compilers to avoid \'unused\' warnings */\n#ifndef SWIGUNUSED\n# if defined(__GNUC__) || defined(__ICC)\n#   define SWIGUNUSED __attribute__ ((unused)) \n# else\n#   define SWIGUNUSED \n# endif\n#endif\n\n/* internal SWIG method */\n#ifndef SWIGINTERN\n# define SWIGINTERN static SWIGUNUSED\n#endif\n\n/* internal inline SWIG method */\n#ifndef SWIGINTERNINLINE\n# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE\n#endif\n\n/* exporting methods for Windows DLLs */\n#ifndef SWIGEXPORT\n# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)\n#   if defined(STATIC_LINKED)\n#     define SWIGEXPORT\n#   else\n#     define SWIGEXPORT __declspec(dllexport)\n#   endif\n# else\n#   define SWIGEXPORT\n# endif\n#endif\n\n/* calling conventions for Windows */\n#ifndef SWIGSTDCALL\n# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)\n#   define SWIGSTDCALL __stdcall\n# else\n#   define SWIGSTDCALL\n# endif \n#endif\n\n"
            | section      - "runtime"
            | 
      +++ include ----------------------------------------
      | name         - "vcsn.i"
      | module       - "vcsn"

            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/config/pconf.hh"

                  +++ constant ----------------------------------------
                  | sym:name     - "VCSN_PACKAGE_BUGREPORT"
                  | name         - "VCSN_PACKAGE_BUGREPORT"
                  | rawval       - "\"vaucanson-bugs@lrde.epita.fr\""
                  | feature:immutable - "1"
                  | value        - "vaucanson-bugs@lrde.epita.fr"
                  | storage      - "%constant"
                  | type         - "p.char"
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"
                  | 
                  +++ constant ----------------------------------------
                  | sym:name     - "VCSN_PACKAGE_NAME"
                  | name         - "VCSN_PACKAGE_NAME"
                  | rawval       - "\"vaucanson\""
                  | feature:immutable - "1"
                  | value        - "vaucanson"
                  | storage      - "%constant"
                  | type         - "p.char"
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"
                  | 
                  +++ constant ----------------------------------------
                  | sym:name     - "VCSN_PACKAGE_STRING"
                  | name         - "VCSN_PACKAGE_STRING"
                  | rawval       - "\"vaucanson 0.7.2\""
                  | feature:immutable - "1"
                  | value        - "vaucanson 0.7.2"
                  | storage      - "%constant"
                  | type         - "p.char"
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"
                  | 
                  +++ constant ----------------------------------------
                  | sym:name     - "VCSN_PACKAGE_VERSION"
                  | name         - "VCSN_PACKAGE_VERSION"
                  | rawval       - "\"0.7.2\""
                  | feature:immutable - "1"
                  | value        - "0.7.2"
                  | storage      - "%constant"
                  | type         - "p.char"
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"
                  | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/config/math.hh"
            | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/config/system.hh"
            | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/predecls.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | csym:nextSibling - 0xb7cd8a98
                  | sym:nextSibling - 0xb7cd8a98
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"

                        +++ template ----------------------------------------
                        | sym:name     - "dynamic_traits"
                        | templatetype - "classforward"
                        | name         - "dynamic_traits"
                        | partials     - 0xb7cda4c8
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename S
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "SyntacticDecorator"
                        | templatetype - "classforward"
                        | name         - "SyntacticDecorator"
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename S,typename T
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "MetaElement"
                        | templatetype - "classforward"
                        | name         - "MetaElement"
                        | partials     - 0xb7cde788
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename S,typename T
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "Element"
                        | templatetype - "classforward"
                        | name         - "Element"
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename S,typename T
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ class ----------------------------------------
                        | name         - "vcsn::default_slot_tag"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cd61a8
                        | allows_typedef - "1"
                        | typepass:visit - "1"
                        | allocate:visit - "1"
                        | kind         - "struct"
                        | sym:name     - "default_slot_tag"
                        | allocate:default_constructor - "1"
                        | allocate:default_destructor - "1"
                        | module       - 0xb7cf9cc8
                        | sym:overname - "__SWIG_0"
                        | typescope    - 0xb7cfa108
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "SetSlot"
                        | templatetype - "classforward"
                        | name         - "SetSlot"
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename S,typename Tag
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "Structure"
                        | templatetype - "classforward"
                        | name         - "Structure"
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename Self
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | sym:name     - "virtual_types"
                        | templatetype - "classforward"
                        | name         - "virtual_types"
                        | partials     - 0xb7cda8d8
                        | kind         - "struct"
                        | sym:weak     - "1"
                        | templateparms - typename Self
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ class ----------------------------------------
                        | name         - "vcsn::undefined_type"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cd6728
                        | allows_typedef - "1"
                        | allocate:visit - "1"
                        | typepass:visit - "1"
                        | kind         - "struct"
                        | sym:name     - "undefined_type"
                        | allocate:default_constructor - "1"
                        | allocate:default_destructor - "1"
                        | module       - 0xb7cf9cc8
                        | sym:overname - "__SWIG_0"
                        | typescope    - 0xb7cfa328
                        | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/misc/selectors.hh"
            | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/misc/unique.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "utility"
                  | name         - "utility"
                  | typescope    - 0xb7cfa358
                  | symtab       - 0xb7cd6868
                  | sym:symtab   - 0xb7cd02e8
                  | sym:overname - "__SWIG_0"

                        +++ namespace ----------------------------------------
                        | sym:name     - "unique"
                        | name         - "unique"
                        | typescope    - 0xb7cfa3a8
                        | symtab       - 0xb7cd7298
                        | sym:symtab   - 0xb7cd6868
                        | sym:overname - "__SWIG_0"

                              +++ template ----------------------------------------
                              | templatetype - "class"
                              | sym:typename - "1"
                              | name         - "UniqueListBase"
                              | sym:symtab   - 0xb7cd6908
                              | symtab       - 0xb7cd6a28
                              | allows_typedef - "1"
                              | kind         - "struct"
                              | sym:name     - "UniqueListBase"
                              | sym:overname - "__SWIG_0"
                              | templateparms - class T

                                    +++ destructor ----------------------------------------
                                    | sym:name     - "~UniqueListBase"
                                    | name         - "~UniqueListBase"
                                    | decl         - "f()."
                                    | storage      - "virtual"
                                    | sym:symtab   - 0xb7cd6a28
                                    | sym:overname - "__SWIG_0"
                                    | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "uniquelist_base"
                              | name         - "utility::unique::UniqueMap::uniquelist_base"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "utility::unique::UniqueListBase<(int)>"
                              | sym:symtab   - 0xb7cd6908
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "class"
                              | sym:typename - "1"
                              | name         - "uniquelist"
                              | sym:symtab   - 0xb7cd6908
                              | symtab       - 0xb7cd7018
                              | allows_typedef - "1"
                              | kind         - "struct"
                              | sym:name     - "uniquelist"
                              | privatebaselist - 0xb7cd6f18
                              | protectedbaselist - 0xb7cd6ef8
                              | baselist     - 0xb7cd6ed8
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename T

                                    +++ destructor ----------------------------------------
                                    | sym:name     - "~uniquelist"
                                    | name         - "~uniquelist"
                                    | decl         - "f()."
                                    | sym:symtab   - 0xb7cd7018
                                    | sym:overname - "__SWIG_0"
                                    | 
                              +++ template ----------------------------------------
                              | templatetype - "class"
                              | sym:typename - "1"
                              | name         - "UniqueMap"
                              | sym:symtab   - 0xb7cd6908
                              | symtab       - 0xb7cd7298
                              | allows_typedef - "1"
                              | kind         - "struct"
                              | sym:name     - "TiSlot"
                              | sym:overname - "__SWIG_0"
                              | templateparms - class T

                                    +++ template ----------------------------------------
                                    | templatetype - "class"
                                    | sym:typename - "1"
                                    | name         - "TiSlot"
                                    | sym:symtab   - 0xb7cd7298
                                    | symtab       - 0xb7cd73f8
                                    | allows_typedef - "1"
                                    | kind         - "struct"
                                    | sym:name     - "TiSlot"
                                    | sym:overname - "__SWIG_0"
                                    | templateparms - class U

                                          +++ constructor ----------------------------------------
                                          | sym:name     - "TiSlot"
                                          | name         - "TiSlot"
                                          | parms        - std::type_info const &
                                          | decl         - "f(r.q(const).std::type_info)."
                                          | feature:new  - "1"
                                          | sym:symtab   - 0xb7cd73f8
                                          | sym:overname - "__SWIG_0"
                                          | 
                                          +++ cdecl ----------------------------------------
                                          | sym:name     - "id"
                                          | name         - "id"
                                          | decl         - "r."
                                          | type         - "q(const).std::type_info"
                                          | sym:symtab   - 0xb7cd73f8
                                          | sym:overname - "__SWIG_0"
                                          | 
                                          +++ cdecl ----------------------------------------
                                          | sym:name     - "operator =="
                                          | name         - "operator =="
                                          | decl         - "q(const).f(r.q(const).TiSlot)."
                                          | parms        - TiSlot const &
                                          | type         - "bool"
                                          | sym:symtab   - 0xb7cd73f8
                                          | sym:overname - "__SWIG_0"
                                          | 
                                          +++ cdecl ----------------------------------------
                                          | sym:name     - "operator <"
                                          | name         - "operator <"
                                          | decl         - "q(const).f(r.q(const).TiSlot)."
                                          | parms        - TiSlot const &
                                          | type         - "bool"
                                          | sym:symtab   - 0xb7cd73f8
                                          | sym:overname - "__SWIG_0"
                                          | 
                                    +++ cdecl ----------------------------------------
                                    | sym:name     - "ti_slot"
                                    | name         - "ti_slot"
                                    | decl         - ""
                                    | storage      - "typedef"
                                    | type         - "TiSlot<(int)>"
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ cdecl ----------------------------------------
                                    | sym:name     - "map_t"
                                    | name         - "map_t"
                                    | decl         - ""
                                    | storage      - "typedef"
                                    | type         - "std::map<(ti_slot,p.uniquelist_base)>"
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ cdecl ----------------------------------------
                                    | sym:name     - "instance"
                                    | name         - "instance"
                                    | decl         - "f().r."
                                    | storage      - "static"
                                    | type         - "map_t"
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ access ----------------------------------------
                                    | kind         - "protected"
                                    | 
                                    +++ cdecl ----------------------------------------
                                    | sym:name     - "map_"
                                    | name         - "map_"
                                    | decl         - ""
                                    | type         - "map_t"
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ destructor ----------------------------------------
                                    | sym:name     - "~UniqueMap"
                                    | name         - "~UniqueMap"
                                    | decl         - "f()."
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ access ----------------------------------------
                                    | kind         - "public"
                                    | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "unique_map"
                              | name         - "utility::unique::UniqueMap::unique_map"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "utility::unique::UniqueMap<(int)>"
                              | sym:symtab   - 0xb7cd7298
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ class ----------------------------------------
                              | name         - "utility::unique::UniqueMap::unifiable"
                              | sym:symtab   - 0xb7cd7298
                              | symtab       - 0xb7cd8168
                              | allows_typedef - "1"
                              | typepass:visit - "1"
                              | allocate:visit - "1"
                              | kind         - "struct"
                              | sym:name     - "unifiable"
                              | allocate:default_constructor - "1"
                              | allocate:has_constructor - "1"
                              | allocate:public_constructor - "1"
                              | allocate:default_destructor - "1"
                              | module       - 0xb7cf9cc8
                              | sym:overname - "__SWIG_0"
                              | typescope    - 0xb7cfa818

                                    +++ constructor ----------------------------------------
                                    | sym:name     - "unifiable"
                                    | name         - "utility::unique::UniqueMap::unifiable"
                                    | decl         - "f()."
                                    | feature:new  - "1"
                                    | sym:overloaded - 0xb7cd8148
                                    | csym:nextSibling - 0xb7cd8268
                                    | sym:nextSibling - 0xb7cd8268
                                    | sym:symtab   - 0xb7cd8168
                                    | sym:overname - "__SWIG_0"
                                    | 
                                    +++ constructor ----------------------------------------
                                    | sym:name     - "unifiable"
                                    | name         - "utility::unique::UniqueMap::unifiable"
                                    | parms        - utility::unique::unifiable const &
                                    | decl         - "f(r.q(const).unifiable)."
                                    | feature:new  - "1"
                                    | sym:overloaded - 0xb7cd8148
                                    | sym:previousSibling - 0xb7cd8148
                                    | sym:symtab   - 0xb7cd8168
                                    | sym:overname - "__SWIG_1"
                                    | 
                                    +++ access ----------------------------------------
                                    | kind         - "private"
                                    | 
                                    +++ template ----------------------------------------
                                    | templatetype - "cdecl"
                                    | sym:typename - "1"
                                    | name         - "get"
                                    | sym:symtab   - 0xb7cd7298
                                    | sym:name     - "get"
                                    | csym:nextSibling - 0xb7cd8788
                                    | sym:nextSibling - 0xb7cd8988
                                    | decl         - "f(r.q(const).T).r."
                                    | parms        - T const &
                                    | sym:overloaded - 0xb7cd8428
                                    | type         - "q(const).T"
                                    | sym:overname - "__SWIG_0"
                                    | storage      - "friend"
                                    | templateparms - typename T
                                    | 
                                    +++ cdecl ----------------------------------------
                                    | name         - "unique_"
                                    | decl         - ""
                                    | access       - "private"
                                    | type         - "bool"
                                    | sym:symtab   - 0xb7cd8168
                                    | 
                                    +++ access ----------------------------------------
                                    | kind         - "public"
                                    | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | name         - "get"
                              | decl         - "f(r.q(const).T).r."
                              | parms        - T const &
                              | error        - "vcsn/include/vaucanson/misc/unique.hh:121:Identifier \'get\' redefined (ignored),\nvcsn/include/vaucanson/misc/unique.hh:108:previous definition of \'get\'.\n"
                              | sym:typename - "1"
                              | type         - "q(const).T"
                              | csym:nextSibling - 0xb7cd8988
                              | templateparms - typename T
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "get"
                              | sym:symtab   - 0xb7cd7298
                              | sym:previousSibling - 0xb7cd8428
                              | sym:name     - "get"
                              | decl         - "f(p.q(const).T).p."
                              | sym:overloaded - 0xb7cd8428
                              | parms        - T const *
                              | type         - "q(const).T"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename T
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/structure.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cd5a28
                  | csym:nextSibling - 0xb7cda888
                  | sym:nextSibling - 0xb7cda888
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "Structure"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cd8c98
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "Structure"
                        | privatebaselist - 0xb7cd8c28
                        | protectedbaselist - 0xb7cd8c18
                        | baselist     - 0xb7cd8c08
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S

                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "contains"
                              | sym:symtab   - 0xb7cd8c98
                              | sym:nextSibling - 0xb7cd9238
                              | csym:nextSibling - 0xb7cd9238
                              | sym:name     - "contains"
                              | decl         - "q(const).f(r.q(const).Element<(S,T)>)."
                              | sym:overloaded - 0xb7cd8f28
                              | parms        - Element<S,T > const &
                              | type         - "bool"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename T
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "contains"
                              | sym:symtab   - 0xb7cd8c98
                              | csym:nextSibling - 0xb7cd9448
                              | sym:previousSibling - 0xb7cd8f28
                              | sym:name     - "contains"
                              | sym:nextSibling - 0xb7cd9448
                              | decl         - "q(const).f(r.q(const).Element<(OtherS,T)>)."
                              | sym:overloaded - 0xb7cd8f28
                              | parms        - Element<OtherS,T > const &
                              | type         - "bool"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename OtherS,typename T
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "contains"
                              | sym:symtab   - 0xb7cd8c98
                              | sym:previousSibling - 0xb7cd9238
                              | sym:name     - "contains"
                              | decl         - "q(const).f(r.q(const).T)."
                              | sym:overloaded - 0xb7cd8f28
                              | parms        - T const &
                              | type         - "bool"
                              | sym:overname - "__SWIG_2"
                              | templateparms - typename T
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:name     - "choose"
                              | name         - "choose"
                              | decl         - "q(const).f(r.q(const).T)."
                              | parms        - T const &
                              | sym:typename - "1"
                              | type         - "Element<(S,T)>"
                              | templateparms - class T
                              | sym:symtab   - 0xb7cd8c98
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "self_t"
                              | name         - "self_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "S"
                              | sym:symtab   - 0xb7cd8c98
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "self"
                              | name         - "self"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cd9848
                              | type         - "self_t"
                              | csym:nextSibling - 0xb7cd9918
                              | sym:nextSibling - 0xb7cd9918
                              | sym:symtab   - 0xb7cd8c98
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "self"
                              | name         - "self"
                              | decl         - "q(const).f().r."
                              | sym:overloaded - 0xb7cd9848
                              | type         - "q(const).self_t"
                              | sym:previousSibling - 0xb7cd9848
                              | sym:symtab   - 0xb7cd8c98
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ access ----------------------------------------
                              | kind         - "protected"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Structure"
                              | decl         - "f()."
                              | access       - "protected"
                              | feature:new  - "1"
                              | csym:nextSibling - 0xb7cd9af8
                              | sym:symtab   - 0xb7cd8c98
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Structure"
                              | parms        - Structure const &
                              | decl         - "f(r.q(const).Structure)."
                              | access       - "protected"
                              | feature:new  - "1"
                              | sym:symtab   - 0xb7cd8c98
                              | 
                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator !="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator !="
                        | csym:nextSibling - 0xb7ce5608
                        | sym:nextSibling - 0xb7ce5608
                        | decl         - "f(r.q(const).vcsn::Structure<(S)>,r.q(const).vcsn::Structure<(S)>)."
                        | parms        - vcsn::Structure<S > const &,vcsn::Structure<S > const &
                        | sym:overloaded - 0xb7cd9f48
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "dynamic_traits<(Structure<(S)>)>"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cda158
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "dynamic_traits<(Structure<(S)>)>"
                        | partialargs  - "dynamic_traits<(vcsn::Structure<($1)>)>"
                        | sym:overname - "__SWIG_0"
                        | templateparms - Structure<S >
                        | partialspecialization - "1"

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret"
                              | name         - "ret"
                              | decl         - ""
                              | storage      - "static"
                              | value        - "false"
                              | type         - "q(const).bool"
                              | sym:symtab   - 0xb7cda158
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "virtual_types<(Structure<(S)>)>"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cda6c8
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "virtual_types<(Structure<(S)>)>"
                        | partialargs  - "virtual_types<(vcsn::Structure<($1)>)>"
                        | sym:overname - "__SWIG_0"
                        | templateparms - Structure<S >
                        | partialspecialization - "1"
                        | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/syntactic_decorator.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cd8a98
                  | csym:nextSibling - 0xb7cdddc8
                  | sym:nextSibling - 0xb7cdddc8
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "SyntacticDecorator"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cdaa08
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "SyntacticDecorator"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T

                              +++ cdecl ----------------------------------------
                              | sym:name     - "dynamic_value"
                              | name         - "dynamic_value"
                              | decl         - ""
                              | storage      - "static"
                              | value        - "true"
                              | type         - "q(const).bool"
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "structure"
                              | name         - "structure"
                              | decl         - "q(const).f().r."
                              | type         - "q(const).S"
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "value"
                              | name         - "value"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cdace8
                              | type         - "T"
                              | csym:nextSibling - 0xb7cdadb8
                              | sym:nextSibling - 0xb7cdadb8
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "value"
                              | name         - "value"
                              | decl         - "q(const).f().r."
                              | sym:overloaded - 0xb7cdace8
                              | type         - "q(const).T"
                              | sym:previousSibling - 0xb7cdace8
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator +="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:nextSibling - 0xb7cdb3e8
                              | csym:nextSibling - 0xb7cdb3e8
                              | sym:name     - "operator +="
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cdb128
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator +="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:previousSibling - 0xb7cdb128
                              | sym:name     - "operator +="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cdb128
                              | parms        - U const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator -="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:nextSibling - 0xb7cdba68
                              | csym:nextSibling - 0xb7cdba68
                              | sym:name     - "operator -="
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cdb7a8
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator -="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:previousSibling - 0xb7cdb7a8
                              | sym:name     - "operator -="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cdb7a8
                              | parms        - U const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator /="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:nextSibling - 0xb7cdc0e8
                              | csym:nextSibling - 0xb7cdc0e8
                              | sym:name     - "operator /="
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cdbe28
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator /="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:previousSibling - 0xb7cdbe28
                              | sym:name     - "operator /="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cdbe28
                              | parms        - U const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator *="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:nextSibling - 0xb7cdc768
                              | csym:nextSibling - 0xb7cdc768
                              | sym:name     - "operator *="
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cdc4a8
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator *="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:previousSibling - 0xb7cdc4a8
                              | sym:name     - "operator *="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cdc4a8
                              | parms        - U const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator %="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:nextSibling - 0xb7cdcde8
                              | csym:nextSibling - 0xb7cdcde8
                              | sym:name     - "operator %="
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cdcb28
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_0"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator %="
                              | sym:symtab   - 0xb7cdaa08
                              | sym:previousSibling - 0xb7cdcb28
                              | sym:name     - "operator %="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cdcb28
                              | parms        - U const &
                              | type         - "Element<(S,T)>"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "operator ++"
                              | name         - "operator ++"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cdcf78
                              | type         - "Element<(S,T)>"
                              | csym:nextSibling - 0xb7cdd128
                              | sym:nextSibling - 0xb7cdd128
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "operator ++"
                              | name         - "operator ++"
                              | decl         - "f(int)."
                              | parms        - int
                              | sym:overloaded - 0xb7cdcf78
                              | type         - "Element<(S,T)>"
                              | sym:previousSibling - 0xb7cdcf78
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "operator --"
                              | name         - "operator --"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cdd268
                              | type         - "Element<(S,T)>"
                              | csym:nextSibling - 0xb7cdd418
                              | sym:nextSibling - 0xb7cdd418
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "operator --"
                              | name         - "operator --"
                              | decl         - "f(int)."
                              | parms        - int
                              | sym:overloaded - 0xb7cdd268
                              | type         - "Element<(S,T)>"
                              | sym:previousSibling - 0xb7cdd268
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:name     - "swap"
                              | name         - "swap"
                              | decl         - "f(r.Element<(S,U)>).r."
                              | parms        - Element<S,U > &
                              | sym:typename - "1"
                              | type         - "Element<(S,T)>"
                              | templateparms - typename U
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "self"
                              | name         - "self"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cdd898
                              | type         - "Element<(S,T)>"
                              | csym:nextSibling - 0xb7cdda18
                              | sym:nextSibling - 0xb7cdda18
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "self"
                              | name         - "self"
                              | decl         - "q(const).f().r."
                              | sym:overloaded - 0xb7cdd898
                              | type         - "q(const).Element<(S,T)>"
                              | sym:previousSibling - 0xb7cdd898
                              | sym:symtab   - 0xb7cdaa08
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ access ----------------------------------------
                              | kind         - "protected"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "SyntacticDecorator"
                              | decl         - "f()."
                              | access       - "protected"
                              | feature:new  - "1"
                              | csym:nextSibling - 0xb7cddbf8
                              | sym:symtab   - 0xb7cdaa08
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "SyntacticDecorator"
                              | parms        - SyntacticDecorator const &
                              | decl         - "f(r.q(const).SyntacticDecorator)."
                              | access       - "protected"
                              | feature:new  - "1"
                              | sym:symtab   - 0xb7cdaa08
                              | 
                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/meta_element_root.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cda888
                  | csym:nextSibling - 0xb7cde7b8
                  | sym:nextSibling - 0xb7cde7b8
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "MetaElement<(Structure<(S)>,T)>"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cde208
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "MetaElement<(Structure<(S)>,T)>"
                        | privatebaselist - 0xb7cde178
                        | protectedbaselist - 0xb7cde168
                        | baselist     - 0xb7cde158
                        | partialargs  - "MetaElement<(vcsn::Structure<($1)>,$2)>"
                        | sym:overname - "__SWIG_0"
                        | templateparms - Structure<S >,typename T
                        | partialspecialization - "1"

                              +++ access ----------------------------------------
                              | kind         - "protected"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "MetaElement"
                              | decl         - "f()."
                              | access       - "protected"
                              | feature:new  - "1"
                              | csym:nextSibling - 0xb7cde478
                              | sym:symtab   - 0xb7cde208
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "MetaElement"
                              | parms        - MetaElement const &
                              | decl         - "f(r.q(const).MetaElement)."
                              | access       - "protected"
                              | feature:new  - "1"
                              | sym:symtab   - 0xb7cde208
                              | 
                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/element_op_traits.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cdddc8
                  | csym:nextSibling - 0xb7ce0778
                  | sym:nextSibling - 0xb7ce0778
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "op_add_traits"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cde9f8
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "op_add_traits"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S1,typename S2,typename T1,typename T2

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret_t"
                              | name         - "ret_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Element<(S1,T1)>"
                              | sym:symtab   - 0xb7cde9f8
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "op_sub_traits"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cdf048
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "op_sub_traits"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S1,typename S2,typename T1,typename T2

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret_t"
                              | name         - "ret_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Element<(S1,T1)>"
                              | sym:symtab   - 0xb7cdf048
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "op_mul_traits"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cdf698
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "op_mul_traits"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S1,typename S2,typename T1,typename T2

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret_t"
                              | name         - "ret_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Element<(S1,T1)>"
                              | sym:symtab   - 0xb7cdf698
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "op_div_traits"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cdfce8
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "op_div_traits"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S1,typename S2,typename T1,typename T2

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret_t"
                              | name         - "ret_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Element<(S1,T1)>"
                              | sym:symtab   - 0xb7cdfce8
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "op_mod_traits"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7ce0338
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "op_mod_traits"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S1,typename S2,typename T1,typename T2

                              +++ cdecl ----------------------------------------
                              | sym:name     - "ret_t"
                              | name         - "ret_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Element<(S1,T1)>"
                              | sym:symtab   - 0xb7ce0338
                              | sym:overname - "__SWIG_0"
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/element_ops.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cde7b8
                  | csym:nextSibling - 0xb7ced2a8
                  | sym:nextSibling - 0xb7ced2a8
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator <"
                        | csym:nextSibling - 0xb7ce22b8
                        | sym:nextSibling - 0xb7ce22b8
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce0ce8
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator >"
                        | csym:nextSibling - 0xb7ce2728
                        | sym:nextSibling - 0xb7ce2728
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce12a8
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator <="
                        | csym:nextSibling - 0xb7ce3008
                        | sym:nextSibling - 0xb7ce3008
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce1868
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator >="
                        | csym:nextSibling - 0xb7ce2b98
                        | sym:nextSibling - 0xb7ce2b98
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce1e28
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce0ce8
                        | sym:name     - "operator <"
                        | csym:nextSibling - 0xb7ce3478
                        | sym:nextSibling - 0xb7ce3478
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce0ce8
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce12a8
                        | sym:name     - "operator >"
                        | csym:nextSibling - 0xb7ce38e8
                        | sym:nextSibling - 0xb7ce38e8
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce12a8
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce1e28
                        | sym:name     - "operator >="
                        | csym:nextSibling - 0xb7ce3d58
                        | sym:nextSibling - 0xb7ce3d58
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce1e28
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce1868
                        | sym:name     - "operator <="
                        | csym:nextSibling - 0xb7ce41c8
                        | sym:nextSibling - 0xb7ce41c8
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce1868
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce22b8
                        | sym:name     - "operator <"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce0ce8
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce2728
                        | sym:name     - "operator >"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce12a8
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce2b98
                        | sym:name     - "operator >="
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce1e28
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce3008
                        | sym:name     - "operator <="
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce1868
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator =="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator =="
                        | csym:nextSibling - 0xb7ce4bf8
                        | sym:nextSibling - 0xb7ce4bf8
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce4768
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator =="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce4768
                        | sym:name     - "operator =="
                        | csym:nextSibling - 0xb7ce5068
                        | sym:nextSibling - 0xb7ce5068
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce4768
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator =="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce4bf8
                        | sym:name     - "operator =="
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce4768
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator !="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cd9f48
                        | sym:name     - "operator !="
                        | csym:nextSibling - 0xb7ce5a98
                        | sym:nextSibling - 0xb7ce5a98
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | sym:overloaded - 0xb7cd9f48
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator !="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce5608
                        | sym:name     - "operator !="
                        | csym:nextSibling - 0xb7ce5f08
                        | sym:nextSibling - 0xb7ce5f08
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7cd9f48
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator !="
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce5a98
                        | sym:name     - "operator !="
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7cd9f48
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_3"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator +"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator +"
                        | csym:nextSibling - 0xb7ce6b98
                        | sym:nextSibling - 0xb7ce6b98
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce6638
                        | type         - "vcsn::op_add_traits<(S1,S2,T1,T2)>::ret_t"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator +"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce6638
                        | sym:name     - "operator +"
                        | csym:nextSibling - 0xb7ce70d8
                        | sym:nextSibling - 0xb7ce70d8
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce6638
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator +"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce6b98
                        | sym:name     - "operator +"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce6638
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator -"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator -"
                        | csym:nextSibling - 0xb7ce7d68
                        | sym:nextSibling - 0xb7ce7d68
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce7808
                        | type         - "vcsn::op_sub_traits<(S1,S2,T1,T2)>::ret_t"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator -"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce7808
                        | sym:name     - "operator -"
                        | csym:nextSibling - 0xb7ce82a8
                        | sym:nextSibling - 0xb7ce82a8
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce7808
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator -"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce7d68
                        | sym:name     - "operator -"
                        | csym:nextSibling - 0xb7cebc18
                        | sym:nextSibling - 0xb7cebc18
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce7808
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator *"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator *"
                        | csym:nextSibling - 0xb7ce8f38
                        | sym:nextSibling - 0xb7ce8f38
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce89d8
                        | type         - "vcsn::op_mul_traits<(S1,S2,T1,T2)>::ret_t"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator *"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce89d8
                        | sym:name     - "operator *"
                        | csym:nextSibling - 0xb7ce9478
                        | sym:nextSibling - 0xb7ce9478
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce89d8
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator *"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce8f38
                        | sym:name     - "operator *"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce89d8
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator /"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator /"
                        | csym:nextSibling - 0xb7cea108
                        | sym:nextSibling - 0xb7cea108
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7ce9ba8
                        | type         - "vcsn::op_div_traits<(S1,S2,T1,T2)>::ret_t"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator /"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce9ba8
                        | sym:name     - "operator /"
                        | csym:nextSibling - 0xb7cea648
                        | sym:nextSibling - 0xb7cea648
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7ce9ba8
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator /"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cea108
                        | sym:name     - "operator /"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce9ba8
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator %"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator %"
                        | csym:nextSibling - 0xb7ceb2d8
                        | sym:nextSibling - 0xb7ceb2d8
                        | decl         - "f(r.q(const).vcsn::Element<(S1,T1)>,r.q(const).vcsn::Element<(S2,T2)>)."
                        | parms        - vcsn::Element<S1,T1 > const &,vcsn::Element<S2,T2 > const &
                        | sym:overloaded - 0xb7cead78
                        | type         - "vcsn::op_mod_traits<(S1,S2,T1,T2)>::ret_t"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename S1,typename T1,typename S2,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator %"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cead78
                        | sym:name     - "operator %"
                        | csym:nextSibling - 0xb7ceb818
                        | sym:nextSibling - 0xb7ceb818
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>,r.q(const).U)."
                        | sym:overloaded - 0xb7cead78
                        | parms        - vcsn::Element<S,T > const &,U const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_1"
                        | storage      - "static"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator %"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ceb2d8
                        | sym:name     - "operator %"
                        | decl         - "f(r.q(const).U,r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7cead78
                        | parms        - U const &,vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_2"
                        | storage      - "static"
                        | templateparms - typename U,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator -"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ce82a8
                        | sym:name     - "operator -"
                        | decl         - "f(r.q(const).vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ce7808
                        | parms        - vcsn::Element<S,T > const &
                        | type         - "vcsn::Element<(S,T)>"
                        | sym:overname - "__SWIG_3"
                        | storage      - "static"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator <<"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator <<"
                        | decl         - "f(r.St,r.q(const).vcsn::Element<(S,T)>).r."
                        | parms        - St &,vcsn::Element<S,T > const &
                        | type         - "St"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename St,typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "operator >>"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "operator >>"
                        | decl         - "f(r.St,r.q(const).vcsn::Element<(S,T)>).r."
                        | parms        - St &,vcsn::Element<S,T > const &
                        | type         - "St"
                        | sym:overname - "__SWIG_0"
                        | storage      - "static"
                        | templateparms - typename St,typename S,typename T
                        | 
                  +++ namespace ----------------------------------------
                  | sym:name     - "std"
                  | name         - "std"
                  | sym:previousSibling - 0xb7cd4398
                  | typescope    - 0xb7cf9d38
                  | symtab       - 0xb7cd4338
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "swap"
                        | sym:symtab   - 0xb7cd4338
                        | sym:nextSibling - 0xb7cece18
                        | csym:nextSibling - 0xb7cece18
                        | sym:name     - "swap"
                        | decl         - "f(r.vcsn::Element<(S,T1)>,r.vcsn::Element<(S,T2)>)."
                        | parms        - vcsn::Element<S,T1 > &,vcsn::Element<S,T2 > &
                        | sym:overloaded - 0xb7ceca48
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T1,typename T2
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "swap"
                        | sym:symtab   - 0xb7cd4338
                        | sym:previousSibling - 0xb7ceca48
                        | sym:name     - "swap"
                        | csym:nextSibling - 0xb7ced1c8
                        | sym:nextSibling - 0xb7ced1c8
                        | decl         - "f(r.vcsn::Element<(S,T)>,r.T)."
                        | sym:overloaded - 0xb7ceca48
                        | parms        - vcsn::Element<S,T > &,T &
                        | type         - "void"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "swap"
                        | sym:symtab   - 0xb7cd4338
                        | sym:previousSibling - 0xb7cece18
                        | sym:name     - "swap"
                        | decl         - "f(r.T,r.vcsn::Element<(S,T)>)."
                        | sym:overloaded - 0xb7ceca48
                        | parms        - T &,vcsn::Element<S,T > &
                        | type         - "void"
                        | sym:overname - "__SWIG_2"
                        | templateparms - typename T,typename S
                        | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/default_ops.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7ce0778
                  | csym:nextSibling - 0xb7cf4b78
                  | sym:nextSibling - 0xb7cf4b78
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:name     - "op_contains"
                        | name         - "op_contains"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T)."
                        | parms        - Structure<S > const &,T const &
                        | sym:typename - "1"
                        | type         - "bool"
                        | templateparms - typename S,typename T
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_eq"
                        | sym:symtab   - 0xb7cd59e8
                        | csym:nextSibling - 0xb7cedfd8
                        | sym:name     - "op_eq"
                        | sym:nextSibling - 0xb7cedfd8
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,T const &,U const &
                        | sym:overloaded - 0xb7ceda28
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_eq"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ceda28
                        | sym:name     - "op_eq"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(V)>,r.q(const).T,r.q(const).U)."
                        | sym:overloaded - 0xb7ceda28
                        | parms        - Structure<S > const &,Structure<V > const &,T const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename S,typename V,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_lt"
                        | sym:symtab   - 0xb7cd59e8
                        | csym:nextSibling - 0xb7ceea18
                        | sym:name     - "op_lt"
                        | sym:nextSibling - 0xb7ceea18
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,T const &,U const &
                        | sym:overloaded - 0xb7cee468
                        | type         - "bool"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_lt"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cee468
                        | sym:name     - "op_lt"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(V)>,r.q(const).T,r.q(const).U)."
                        | sym:overloaded - 0xb7cee468
                        | parms        - Structure<S > const &,Structure<V > const &,T const &,U const &
                        | type         - "bool"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename S,typename V,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_convert"
                        | sym:symtab   - 0xb7cd59e8
                        | csym:nextSibling - 0xb7cef2c8
                        | sym:name     - "op_convert"
                        | sym:nextSibling - 0xb7cef2c8
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).R,r.q(const).T)."
                        | parms        - Structure<S > const &,R const &,T const &
                        | sym:overloaded - 0xb7ceeeb8
                        | type         - "R"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename R,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_convert"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7ceeeb8
                        | sym:name     - "op_convert"
                        | csym:nextSibling - 0xb7cef7c8
                        | sym:nextSibling - 0xb7cef7c8
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T,r.q(const).T).r."
                        | sym:overloaded - 0xb7ceeeb8
                        | parms        - Structure<S > const &,T const &,T const &
                        | type         - "q(const).T"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_convert"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cef2c8
                        | sym:name     - "op_convert"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T,r.q(const).Structure<(S)>,r.q(const).T).r."
                        | sym:overloaded - 0xb7ceeeb8
                        | parms        - Structure<S > const &,T const &,Structure<S > const &,T const &
                        | type         - "q(const).T"
                        | sym:overname - "__SWIG_2"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_default"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_default"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).T)."
                        | parms        - Structure<S > const &,T const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_swap"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_swap"
                        | decl         - "f(r.q(const).Structure<(S)>,r.T,r.T)."
                        | parms        - Structure<S > const &,T &,T &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_assign"
                        | sym:symtab   - 0xb7cd59e8
                        | csym:nextSibling - 0xb7cf0918
                        | sym:name     - "op_assign"
                        | sym:nextSibling - 0xb7cf0918
                        | decl         - "f(r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,T &,U const &
                        | sym:overloaded - 0xb7cf0388
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_assign"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cf0388
                        | sym:name     - "op_assign"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | sym:overloaded - 0xb7cf0388
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_in_add"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_in_add"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_in_sub"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_in_sub"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_in_mul"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_in_mul"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_in_div"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_in_div"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_in_mod"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_in_mod"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T &,U const &
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_add"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_add"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T const &,U const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_sub"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_sub"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T const &,U const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_mul"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_mul"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T const &,U const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_div"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_div"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T const &,U const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_mod"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_mod"
                        | decl         - "f(r.q(const).Structure<(S)>,r.q(const).Structure<(S)>,r.q(const).T,r.q(const).U)."
                        | parms        - Structure<S > const &,Structure<S > const &,T const &,U const &
                        | type         - "T"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T,typename U
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "op_rin"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:name     - "op_rin"
                        | decl         - "f(r.q(const).Structure<(S)>,r.St,r.q(const).T).r."
                        | parms        - Structure<S > const &,St &,T const &
                        | type         - "St"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename St,typename T
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:name     - "op_rout"
                        | name         - "op_rout"
                        | decl         - "f(r.q(const).Structure<(S)>,r.St,r.q(const).T).r."
                        | parms        - Structure<S > const &,St &,T const &
                        | sym:typename - "1"
                        | type         - "St"
                        | templateparms - typename S,typename St,typename T
                        | sym:symtab   - 0xb7cd59e8
                        | sym:overname - "__SWIG_0"
                        | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/slots.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7ced2a8
                  | csym:nextSibling - 0xb7cf6a18
                  | sym:nextSibling - 0xb7cf6a18
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "SetSlotAttribute"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cf4d38
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "SetSlotAttribute"
                        | partials     - 0xb7cf6128
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,bool

                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlotAttribute"
                              | name         - "SetSlotAttribute"
                              | decl         - "f()."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf4e28
                              | csym:nextSibling - 0xb7cf4f48
                              | sym:nextSibling - 0xb7cf4f48
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "SetSlotAttribute"
                              | sym:symtab   - 0xb7cf4d38
                              | sym:nextSibling - 0xb7cf5068
                              | csym:nextSibling - 0xb7cf5068
                              | sym:previousSibling - 0xb7cf4e28
                              | sym:name     - "SetSlotAttribute"
                              | decl         - "f(r.q(const).SetSlotAttribute)."
                              | sym:overloaded - 0xb7cf4e28
                              | parms        - SetSlotAttribute const &
                              | sym:overname - "__SWIG_1"
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlotAttribute"
                              | name         - "SetSlotAttribute"
                              | parms        - S const &
                              | decl         - "f(r.q(const).S)."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf4e28
                              | sym:previousSibling - 0xb7cf4f48
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_2"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_get"
                              | name         - "_structure_get"
                              | decl         - "q(const).f().r."
                              | type         - "q(const).S"
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_assign"
                              | name         - "_structure_assign"
                              | decl         - "f(r.q(const).SetSlotAttribute)."
                              | parms        - SetSlotAttribute const &
                              | type         - "void"
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_attach"
                              | name         - "_structure_attach"
                              | decl         - "f(r.q(const).S)."
                              | parms        - S const &
                              | type         - "void"
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_bound"
                              | name         - "_structure_bound"
                              | decl         - "q(const).f()."
                              | type         - "bool"
                              | sym:symtab   - 0xb7cf4d38
                              | sym:overname - "__SWIG_0"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "SetSlotAttribute<(S,true)>"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cf56f8
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "SetSlotAttribute<(S,true)>"
                        | partialargs  - "SetSlotAttribute<($1,true)>"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,true
                        | partialspecialization - "1"

                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlotAttribute"
                              | name         - "SetSlotAttribute"
                              | decl         - "f()."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf57b8
                              | csym:nextSibling - 0xb7cf58d8
                              | sym:nextSibling - 0xb7cf58d8
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "SetSlotAttribute"
                              | sym:symtab   - 0xb7cf56f8
                              | sym:nextSibling - 0xb7cf59f8
                              | csym:nextSibling - 0xb7cf59f8
                              | sym:previousSibling - 0xb7cf57b8
                              | sym:name     - "SetSlotAttribute"
                              | decl         - "f(r.q(const).SetSlotAttribute)."
                              | sym:overloaded - 0xb7cf57b8
                              | parms        - SetSlotAttribute const &
                              | sym:overname - "__SWIG_1"
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlotAttribute"
                              | name         - "SetSlotAttribute"
                              | parms        - S const &
                              | decl         - "f(r.q(const).S)."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf57b8
                              | sym:previousSibling - 0xb7cf58d8
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_2"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_get"
                              | name         - "_structure_get"
                              | decl         - "q(const).f().r."
                              | type         - "q(const).S"
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_assign"
                              | name         - "_structure_assign"
                              | decl         - "f(r.q(const).SetSlotAttribute)."
                              | parms        - SetSlotAttribute const &
                              | type         - "void"
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_attach"
                              | name         - "_structure_attach"
                              | decl         - "f(r.q(const).S)."
                              | parms        - S const &
                              | type         - "void"
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "_structure_bound"
                              | name         - "_structure_bound"
                              | decl         - "q(const).f()."
                              | type         - "bool"
                              | sym:symtab   - 0xb7cf56f8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ access ----------------------------------------
                              | kind         - "protected"
                              | 
                              +++ cdecl ----------------------------------------
                              | name         - "s_"
                              | decl         - "p."
                              | access       - "protected"
                              | type         - "q(const).S"
                              | sym:symtab   - 0xb7cf56f8
                              | 
                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "SetSlot"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cf64a8
                        | allows_typedef - "1"
                        | kind         - "struct"
                        | sym:name     - "SetSlot"
                        | privatebaselist - 0xb7cf63d8
                        | protectedbaselist - 0xb7cf63c8
                        | baselist     - 0xb7cf63b8
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename Tag

                              +++ cdecl ----------------------------------------
                              | sym:name     - "tag_type"
                              | name         - "tag_type"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "Tag"
                              | sym:symtab   - 0xb7cf64a8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlot"
                              | name         - "SetSlot"
                              | decl         - "f()."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf6648
                              | csym:nextSibling - 0xb7cf6768
                              | sym:nextSibling - 0xb7cf6768
                              | sym:symtab   - 0xb7cf64a8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "SetSlot"
                              | sym:symtab   - 0xb7cf64a8
                              | sym:nextSibling - 0xb7cf6888
                              | csym:nextSibling - 0xb7cf6888
                              | sym:previousSibling - 0xb7cf6648
                              | sym:name     - "SetSlot"
                              | decl         - "f(r.q(const).SetSlot)."
                              | sym:overloaded - 0xb7cf6648
                              | parms        - SetSlot const &
                              | sym:overname - "__SWIG_1"
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | sym:name     - "SetSlot"
                              | name         - "SetSlot"
                              | parms        - S const &
                              | decl         - "f(r.q(const).S)."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf6648
                              | sym:previousSibling - 0xb7cf6768
                              | sym:symtab   - 0xb7cf64a8
                              | sym:overname - "__SWIG_2"
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/element.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cf4b78
                  | csym:nextSibling - 0xb7cf9498
                  | sym:nextSibling - 0xb7cf9498
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "class"
                        | sym:typename - "1"
                        | name         - "Element"
                        | sym:symtab   - 0xb7cd59e8
                        | symtab       - 0xb7cf6de8
                        | allows_typedef - "1"
                        | kind         - "class"
                        | sym:name     - "Element"
                        | privatebaselist - 0xb7cf6cb8
                        | protectedbaselist - 0xb7cf6ca8
                        | baselist     - 0xb7cf6c98
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename S,typename T

                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "set_t"
                              | name         - "set_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "S"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "value_t"
                              | name         - "value_t"
                              | decl         - ""
                              | storage      - "typedef"
                              | type         - "T"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "dynamic"
                              | name         - "dynamic"
                              | decl         - ""
                              | storage      - "static"
                              | value        - "dynamic_traits<(S)>::ret||MetaElement<(S,T)>::dynamic_value"
                              | type         - "q(const).bool"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | sym:name     - "Element"
                              | name         - "Element"
                              | decl         - "f()."
                              | feature:new  - "1"
                              | sym:overloaded - 0xb7cf72a8
                              | csym:nextSibling - 0xb7cf73c8
                              | sym:nextSibling - 0xb7cf73c8
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | csym:nextSibling - 0xb7cf78f8
                              | sym:nextSibling - 0xb7cf7608
                              | sym:previousSibling - 0xb7cf72a8
                              | sym:name     - "Element"
                              | decl         - "f(r.q(const).Element)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - Element const &
                              | sym:overname - "__SWIG_1"
                              | feature:new  - "1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "constructor"
                              | sym:typename - "1"
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:previousSibling - 0xb7cf73c8
                              | csym:nextSibling - 0xb7cf72a8
                              | sym:name     - "Element"
                              | sym:nextSibling - 0xb7cf78f8
                              | decl         - "f(r.q(const).Element<(S,U)>)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - Element<S,U > const &
                              | sym:overname - "__SWIG_2"
                              | templateparms - typename U
                              | feature:new  - "1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "constructor"
                              | sym:typename - "1"
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | csym:nextSibling - 0xb7cf7a78
                              | sym:previousSibling - 0xb7cf7608
                              | sym:name     - "Element"
                              | sym:nextSibling - 0xb7cf7a78
                              | decl         - "f(r.q(const).Element<(OtherS,U)>)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - Element<OtherS,U > const &
                              | sym:overname - "__SWIG_3"
                              | templateparms - typename OtherS,typename U
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:nextSibling - 0xb7cf7c08
                              | csym:nextSibling - 0xb7cf7c08
                              | sym:previousSibling - 0xb7cf78f8
                              | sym:name     - "Element"
                              | decl         - "f(r.q(const).T)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - T const &
                              | sym:overname - "__SWIG_4"
                              | feature:new  - "1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "constructor"
                              | sym:typename - "1"
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | csym:nextSibling - 0xb7cf7d68
                              | sym:previousSibling - 0xb7cf7a78
                              | sym:name     - "Element"
                              | sym:nextSibling - 0xb7cf7d68
                              | decl         - "f(r.q(const).U)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - U const &
                              | sym:overname - "__SWIG_5"
                              | templateparms - typename U
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:nextSibling - 0xb7cf7f28
                              | csym:nextSibling - 0xb7cf7f28
                              | sym:previousSibling - 0xb7cf7c08
                              | sym:name     - "Element"
                              | decl         - "f(r.q(const).S)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - S const &
                              | sym:overname - "__SWIG_6"
                              | feature:new  - "1"
                              | 
                              +++ constructor ----------------------------------------
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:nextSibling - 0xb7cf8158
                              | csym:nextSibling - 0xb7cf8158
                              | sym:previousSibling - 0xb7cf7d68
                              | sym:name     - "Element"
                              | decl         - "f(r.q(const).S,r.q(const).T)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - S const &,T const &
                              | sym:overname - "__SWIG_7"
                              | feature:new  - "1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "constructor"
                              | sym:typename - "1"
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | csym:nextSibling - 0xb7cf84e8
                              | sym:previousSibling - 0xb7cf7f28
                              | sym:name     - "Element"
                              | sym:nextSibling - 0xb7cf84e8
                              | decl         - "f(r.q(const).S,r.q(const).U)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - S const &,U const &
                              | sym:overname - "__SWIG_8"
                              | templateparms - typename U
                              | feature:new  - "1"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "constructor"
                              | sym:typename - "1"
                              | name         - "Element"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:previousSibling - 0xb7cf8158
                              | sym:name     - "Element"
                              | decl         - "f(r.q(const).S,r.q(const).Element<(OtherS,U)>)."
                              | sym:overloaded - 0xb7cf72a8
                              | parms        - S const &,Element<OtherS,U > const &
                              | sym:overname - "__SWIG_9"
                              | templateparms - typename OtherS,typename U
                              | feature:new  - "1"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "operator ="
                              | name         - "operator ="
                              | decl         - "f(r.q(const).Element).r."
                              | parms        - Element const &
                              | sym:overloaded - 0xb7cf8678
                              | type         - "Element"
                              | sym:nextSibling - 0xb7cf88d8
                              | csym:nextSibling - 0xb7cf8be8
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator ="
                              | sym:symtab   - 0xb7cf6de8
                              | sym:previousSibling - 0xb7cf8678
                              | csym:nextSibling - 0xb7cf8678
                              | sym:name     - "operator ="
                              | sym:nextSibling - 0xb7cf8be8
                              | decl         - "f(r.q(const).Element<(S,U)>).r."
                              | sym:overloaded - 0xb7cf8678
                              | parms        - Element<S,U > const &
                              | type         - "Element"
                              | sym:overname - "__SWIG_1"
                              | templateparms - typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator ="
                              | sym:symtab   - 0xb7cf6de8
                              | csym:nextSibling - 0xb7cf8df8
                              | sym:previousSibling - 0xb7cf88d8
                              | sym:name     - "operator ="
                              | sym:nextSibling - 0xb7cf8df8
                              | decl         - "f(r.q(const).Element<(OtherS,U)>).r."
                              | sym:overloaded - 0xb7cf8678
                              | parms        - Element<OtherS,U > const &
                              | type         - "Element"
                              | sym:overname - "__SWIG_2"
                              | templateparms - typename OtherS,typename U
                              | 
                              +++ template ----------------------------------------
                              | templatetype - "cdecl"
                              | sym:typename - "1"
                              | name         - "operator ="
                              | sym:symtab   - 0xb7cf6de8
                              | sym:previousSibling - 0xb7cf8be8
                              | sym:name     - "operator ="
                              | decl         - "f(r.q(const).U).r."
                              | sym:overloaded - 0xb7cf8678
                              | parms        - U const &
                              | type         - "Element"
                              | sym:overname - "__SWIG_3"
                              | templateparms - typename U
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "structure"
                              | name         - "structure"
                              | decl         - "q(const).f().r."
                              | type         - "q(const).S"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "attach"
                              | name         - "attach"
                              | decl         - "f(r.q(const).S)."
                              | parms        - S const &
                              | type         - "void"
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "value"
                              | name         - "value"
                              | decl         - "f().r."
                              | sym:overloaded - 0xb7cf90c8
                              | type         - "T"
                              | csym:nextSibling - 0xb7cf9198
                              | sym:nextSibling - 0xb7cf9198
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_0"
                              | 
                              +++ cdecl ----------------------------------------
                              | sym:name     - "value"
                              | name         - "value"
                              | decl         - "q(const).f().r."
                              | sym:overloaded - 0xb7cf90c8
                              | type         - "q(const).T"
                              | sym:previousSibling - 0xb7cf90c8
                              | sym:symtab   - 0xb7cf6de8
                              | sym:overname - "__SWIG_1"
                              | 
                              +++ access ----------------------------------------
                              | kind         - "private"
                              | 
                              +++ cdecl ----------------------------------------
                              | name         - "value_"
                              | decl         - ""
                              | access       - "private"
                              | type         - "T"
                              | sym:symtab   - 0xb7cf6de8
                              | 
                              +++ access ----------------------------------------
                              | kind         - "public"
                              | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/design_pattern/design_pattern.hh"
            | 
            +++ include ----------------------------------------
            | name         - "vcsn/include/vaucanson/algorithms/standard_of.hh"

                  +++ namespace ----------------------------------------
                  | sym:name     - "vcsn"
                  | name         - "vcsn"
                  | sym:previousSibling - 0xb7cf6a18
                  | typescope    - 0xb7cf9e58
                  | symtab       - 0xb7cd59e8
                  | sym:symtab   - 0xb7cd02e8

                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "standard_of"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:nextSibling - 0xb7cf9c28
                        | csym:nextSibling - 0xb7cf9c28
                        | sym:name     - "standard_of"
                        | decl         - "f(r.Element<(A,T)>,r.q(const).Exp)."
                        | parms        - Element<A,T > &,Exp const &
                        | sym:overloaded - 0xb7cf9868
                        | type         - "void"
                        | sym:overname - "__SWIG_0"
                        | templateparms - typename A,typename T,typename Exp
                        | 
                        +++ template ----------------------------------------
                        | templatetype - "cdecl"
                        | sym:typename - "1"
                        | name         - "standard_of"
                        | sym:symtab   - 0xb7cd59e8
                        | sym:previousSibling - 0xb7cf9868
                        | sym:name     - "standard_of"
                        | decl         - "f(r.q(const).Exp)."
                        | sym:overloaded - 0xb7cf9868
                        | parms        - Exp const &
                        | type         - "Element<(A,T)>"
                        | sym:overname - "__SWIG_1"
                        | templateparms - typename A,typename T,typename Exp
                        | 
            +++ module ----------------------------------------
            | name         - "vcsn"
            | 
