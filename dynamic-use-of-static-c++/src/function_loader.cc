/* Copyright (C) 2005, 2009 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#ifndef DYN_FUNCTION_LOADER_CC
# define DYN_FUNCTION_LOADER_CC

# include <cstdlib>

# include <map>
// FIXME: Use and improve the logger to avoid use of std::cerr.
# include <iostream>

# include <ltdl.h>

# include <boost/filesystem.hpp>
# include <boost/filesystem/fstream.hpp>
# include <boost/algorithm/string/replace.hpp>

# include "md5.hh"

# include "data.hh"
# include "function_loader.hh"
# include "ruby_stream.hh"

// For more details about this, see
// http://www.gnu.org/prep/standards/html_node/Conditional-Compilation.html
# ifdef DYN_RUBY_GENERATOR
#  define HAVE_DYN_RUBY_GENERATOR true
# else
#  define HAVE_DYN_RUBY_GENERATOR false
# endif

namespace bfs = boost::filesystem;
namespace ba = boost::algorithm;


template <typename InputIterator, typename T, typename OStream>
OStream& join(const InputIterator& begin, const InputIterator& end,
	      const T& elt, OStream& ostr)
{
  InputIterator it = begin;

  if (it != end)
    ostr << *it;

  for (++it; it != end; ++it)
    ostr << elt << *it;

  return ostr;
}


namespace dyn {

  ruby::environment ruby_environment;

  std::list<std::string> includes_;

  template <typename Fun>
  void
  foreach_path_in_paths(const std::string& header_paths, Fun& fun)
  {
    std::list<std::string>::const_iterator it;
    unsigned last = 0, pos;
    while (true)
    {
      pos = header_paths.find(":", last);
      std::string sub = header_paths.substr(last, pos - last);
      if (sub == "*")
      {
	for (it = includes_.begin(); it != includes_.end(); ++it)
	  fun(*it);
      }
      else if (sub != "")
	fun(sub);
      if (pos >= std::string::npos) break;
      last = pos + 1;
    }
  }

  template <typename OStream>
  struct gen_cxx_path
  {
    gen_cxx_path(OStream& ostr_) : ostr(ostr_) {}
    void operator() (const std::string& path)
    {
      ostr << "#include ";
      if (path.find("<") == std::string::npos
	  || path.find("\"") == std::string::npos)
	if (path.find(".hh") == path.length() - 3)
	  ostr << "\"" << path << "\"";
	else
	  ostr << "<" << path << ">";
      else
	ostr << path;
      ostr << '\n';
    }
    OStream& ostr;
  };

  template <typename OStream>
  struct gen_path
  {
    gen_path(OStream& ostr_) : first(true), ostr(ostr_) {}
    void operator() (const std::string& path)
    {
      if (first)
      {
	first = false;
	ostr << path;
      }
      else
	ostr << ':' << path;
    }
    bool first;
    OStream& ostr;
  };

  struct function_loader_t
  {

    function_loader_t()
    {
      lt_dlinit();
      if (HAVE_DYN_RUBY_GENERATOR)
	{
	  ruby << "$: << \"" << DYNDIR << "\"" << ruby::eval;
	  ruby << "require 'function_loader'" << ruby::eval;
	  ruby << "require 'md5'" << ruby::eval;
	  ruby << "Signal.trap(:SEGV, 'IGNORE')" << ruby::eval;
	}
    }

    ~function_loader_t()
    {
      lt_dlexit();
    }

    template <typename OStream>
    void
    gen_cxx(const std::string& identifier,
	    const std::string& name,
	    const std::list<std::string>& args,
	    fun_kind kind,
	    const std::string paths,
	    OStream& ostr)
    {
      typedef std::string str;
      typedef std::list<str> str_list;
      str_list call_args;
      std::ostringstream body, call;
      str nl("\n        ");
      bool first_type_is_ptr = false;
      str_list::const_iterator it;

      ostr << "#include \"dyn-light.hh\"\n";

      gen_cxx_path<OStream> fun(ostr);
      foreach_path_in_paths(paths, fun);

      ostr << "extern \"C\" {\n"
	   << "  namespace dyn {\n"
	   << "    namespace generated {\n"
	   << "      data\n"
	   << "      dyn_" << identifier << "(";

      int i = 0;
      for (it = args.begin(); it != args.end(); ++it, ++i)
      {
	std::ostringstream oarg;
	oarg << "arg" << i;
	str arg(oarg.str());

	str type(*it);
	// Remove references cause they are forbidden on lhs.
	while (*type.rbegin() == '&') type.erase(--type.end());

	if (kind == METH and i == 0)
	{
	  // Check if the first type is a pointer to choose the good
	  // operator (`.' or `->').
	  str stripped_type(type);
	  unsigned pos;
	  while ((pos = stripped_type.find(" ")) != str::npos)
	    stripped_type.erase(pos, 1);
	  unsigned len = stripped_type.length();
	  first_type_is_ptr =
	    ((stripped_type.compare(len - 7, 7, "*const>") == 0)
	     || (stripped_type.compare(len - 2, 2, "*>") == 0));
	}

	if (it != args.begin()) ostr << ", ";
	ostr << "const data& " << arg;
	call_args.push_back(arg + "_reinterpret_cast_ptr->obj()");
	body << type << "* " << arg << "_reinterpret_cast_ptr = "
	     << "reinterpret_cast<" << type << "* >(" << arg << ".proxy());"
	     << nl << "assert(" << arg << "_reinterpret_cast_ptr);" << nl;
      }

      if (kind == METH)
      {
	call << call_args.front() << ((first_type_is_ptr)? "->" : ".");
	call_args.pop_front();
      }

      call << name << "(";
      join(call_args.begin(), call_args.end(), ", ", call);
      call << ")";

      str op(name);

      switch (kind)
      {
	case OP:
	  if (op.compare(0, 8, "operator") == 0)
	    op.erase(0, 8);
	  call.str(std::string());
	  switch (call_args.size())
	  {
	    case 1:
	      call << op << "(" << *call_args.begin() << ")";
	      break;
	    case 2:
	      it = call_args.begin();
	      call << "(" << *it++ << ") ";
	      call << op
		   << " (" << *it << ")";
	      break;
	    default: assert(0);
	  }
	  // no break here
	case METH:
	case FUN:
	  body << "policy::receiver<select_dyn_policy(("
	       << call.str() << "))> receiver;" << nl
	       << "(receiver(), " << call.str() << ");" << nl
	       << "data ret(receiver.proxy(), (proxy_tag*)0);" << nl
	       << "return ret;\n";
	  break;
	case CTOR:
	  body << "typedef " << name << " T;" << nl
	       << "T* ptr = new T(";
	  join(call_args.begin(), call_args.end(), ", ", body);
	  body << ");" << nl
	       << "abstract_data* proxy = new data_proxy_by_ptr<T>(ptr);" << nl
	       << "data ret(proxy, (proxy_tag*)0);" << nl
	       << "return ret;\n";
	  break;
	default:
	  assert(!"Unknown kind");
      }

      ostr << ")\n"
	   << "      {" << nl
	   << body.str()
	   << "      }\n"
	   << "    }\n"
	   << "  }\n"
	   << "};\n";

    }

    void
    include_dir(const std::string& dir)
    {
      cflags_.push_back(std::string("-I") + dir);
    }

    void
    cflags(const std::string& elt)
    {
      cflags_.push_back(elt);
    }

    void
    ldflags(const std::string& elt)
    {
      ldflags_.push_back(elt);
    }

    // A C++ implementation of function_loader.rb's `compile'.
    void
    cxx_compile(const std::string& cxx, const std::string& identifier,
		const std::string& cflags, const std::string& ldflags)
    {
      bfs::path dyn_datadir(DYN_DATADIR);

      bfs::path repository("repository");
      if (!bfs::exists(repository))
	{
	  bfs::create_directory(repository);
	  bfs::create_symlink(dyn_datadir / "Makefile.repository",
			      repository / "Makefile");
	}

      bfs::path dir = repository / identifier;
      if (!bfs::exists(dir))
	{
	  bfs::create_directory(dir);

	  bfs::ifstream makefile_orig_str(dyn_datadir / "Makefile.template");
	  std::stringstream makefile_orig;
	  makefile_orig << makefile_orig_str.rdbuf();
	  bfs::ofstream makefile(dir / "Makefile");
	  /* FIXME: We might want to use boost::format in several
	     places here, since

	       (boost::format("libdyn_%1%.la") % identifier).str()

	     may be more elegant than

	       std::string("libdyn_") + identifier + ".la")
	  */
	  /* FIXME: It would be more elegant if we could replace
	     `libdyn_function.la' on the fly while copying the
	     Makefile (as we would do with Perl).  See what Boost
	     proposes.  */
	  makefile <<
	    ba::replace_all_copy(makefile_orig.str(),
				 "libdyn_function.la",
				 std::string("libdyn_") + identifier + ".la");
	  makefile << "CXXFLAGS += " << cflags << std::endl;
	  makefile << "LDFLAGS += " << ldflags << std::endl;

	  bfs::create_directory(dir / ".deps");
	  bfs::ofstream(dir / ".deps" / "libdyn_function_la-function.Plo");

	  bfs::path file = dir / "function.cc";
	  bfs::ofstream function(file);
	  function << cxx;
	}

      bfs::path out = dir / "make.out";
      // FIXME: Same remark wrt boost::format.
      std::string cmd =
	std::string("cd ") + dir.string() + " && make >make.out 2>&1";
      if (system(cmd.c_str()) == 0)
	{
	  if (bfs::exists(out))
	    bfs::remove(out);
	}
      else
	{
	  bfs::ifstream out_log(out);
	  std::cerr << "JIT: Error when compiling this code" << std::endl
		    << cxx << std::endl
		    << cmd << std::endl
		    << out_log.rdbuf() << std::endl;
	  // FIXME: Isn't this a bit too violent?
	  std::exit(1);
	}
    }

    // Most of the ruby-dependent code generator is in this function.
    void*
    ruby_load(fun_kind kind,
	      const std::string& name,
	      const arguments_types_t& arguments_types,
	      const std::string& paths)
    {
      std::ostringstream ostr;
      ostr << name << '(';
      arguments_types_t::const_iterator it(arguments_types.begin());
      if (it != arguments_types.end())
      {
	ostr << *it;
	for (++it; it != arguments_types.end(); ++it)
	  ostr << ", " << *it;
      }
      ostr << ')';
      if (paths != "")
      {
	ostr << ", paths: ";
	gen_path<std::ostream> fun(ostr);
	foreach_path_in_paths(paths, fun);
      }
      std::string prototype = ostr.str();

      ruby << "MD5.new(%q{" << prototype.c_str() << "}).to_s" << ruby::eval;
      std::string identifier = STR2CSTR(ruby.last_value());

      cache_type::iterator ptr_it = cache.find(identifier);

      if (ptr_it != cache.end())
      {
	// FIXME: Don't use a hard-coded error stream (std::cerr).
	// FIXME: Colors should be used only when the terminal supports them.
#if 0
	std::cerr << "\e[36mJIT: \e[32mHIT: \e[0m " << prototype << std::endl;
#endif
	std::cerr << "JIT: HIT:  " << prototype << std::endl;
	return ptr_it->second;
      }

      // FIXME: Colors should be used only when the terminal supports them.
#if 0
      std::cerr
	<< "\e[36mJIT: \e[31mMISS: compile: \e[0m " << prototype << std::endl;
#endif
      std::cerr << "JIT: MISS: compile:  " << prototype << std::endl;

      ruby << "compile %q{";
      gen_cxx(identifier, name, arguments_types, kind, paths, ruby);
      ruby << "}, %q{" << identifier << "}, %q{" << name << "}, %q{";
      join(cflags_.begin(), cflags_.end(), ' ', ruby);
      ruby << "}, %q{";
      join(ldflags_.begin(), ldflags_.end(), ' ', ruby);
      ruby << "}" << ruby::eval;

      const char* error;
      std::string lib_path = std::string("repository/") + identifier
			     + "/libdyn_" + identifier + ".la";
      std::string symb = std::string("dyn_") + identifier;

      lt_dlhandle lib = lt_dlopen(lib_path.c_str());
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      void* ptr = lt_dlsym(lib, symb.c_str());
      cache[identifier] = ptr;
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      return ptr;
    }

    // FIXME: This C++ version of ruby_load shares a lot with the latter.
    void*
    cxx_load(fun_kind kind,
	     const std::string& name,
	     const arguments_types_t& arguments_types,
	     const std::string& paths)
    {
      std::ostringstream ostr;
      ostr << name << '(';
      arguments_types_t::const_iterator it(arguments_types.begin());
      if (it != arguments_types.end())
      {
	ostr << *it;
	for (++it; it != arguments_types.end(); ++it)
	  ostr << ", " << *it;
      }
      ostr << ')';
      if (paths != "")
      {
	ostr << ", paths: ";
	gen_path<std::ostream> fun(ostr);
	foreach_path_in_paths(paths, fun);
      }
      std::string prototype = ostr.str();

      // FIXME: Careful, this cast removes a const!  We should improve
      // the interface of libmd5.
      std::string identifier =
	MD5((unsigned char*)prototype.c_str()).hex_digest();

      cache_type::iterator ptr_it = cache.find(identifier);

      // FIXME: It seems the cache doesn't work at all (we almost
      // never hit).  See why this is happening.
      if (ptr_it != cache.end())
      {
	// FIXME: Don't use a hard-coded error stream (std::cerr).
	// FIXME: Colors should be used only when the terminal supports them.
#if 0
	std::cerr << "\e[36mJIT: \e[32mHIT: \e[0m " << prototype << std::endl;
#endif
	std::cerr << "JIT: HIT:  " << prototype << std::endl;
	return ptr_it->second;
      }

      // FIXME: Colors should be used only when the terminal supports them.
#if 0
      std::cerr
	<< "\e[36mJIT: \e[31mMISS: compile: \e[0m " << prototype << std::endl;
#endif
      std::cerr << "JIT: MISS: compile:  " << prototype << std::endl;

      std::ostringstream cxx;
      gen_cxx(identifier, name, arguments_types, kind, paths, cxx);
      // FIXME: Rename cflags_ as cxxflags_.
      /* FIXME: The interface of join is not elegant.  I'd prefer to write

	   std::stringstream cflags_stream;
	   cflags_stream << join(cflags_.begin(), cflags_.end(), ' ');

	 or even

	   std::string cflags_string =
	     join(cflags_.begin(), cflags_.end(), ' ');

	 But doesn't Boost propose this? E.g. :

	   std::string cflags = ba::string::join(cflags_, ' ');

	 ?  */
      std::stringstream cflags_stream;
      join(cflags_.begin(), cflags_.end(), ' ', cflags_stream);
      std::stringstream ldflags_stream;
      join(ldflags_.begin(), ldflags_.end(), ' ', ldflags_stream);

      cxx_compile(cxx.str(), identifier,
		  cflags_stream.str(), ldflags_stream.str());

      const char* error;
      std::string lib_path = std::string("repository/") + identifier
			     + "/libdyn_" + identifier + ".la";
      std::string symb = std::string("dyn_") + identifier;

      lt_dlhandle lib = lt_dlopen(lib_path.c_str());
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      void* ptr = lt_dlsym(lib, symb.c_str());
      cache[identifier.c_str()] = ptr;
      if ((error = lt_dlerror())) std::cerr << error << std::endl;
      return ptr;
    }

  protected:
    typedef std::map<std::string, void*> cache_type;
    /* FIXME: Introduce a real cache object, with
       debugging/pretty-printing methods.  */
    /* FIXME: It seems this cache is only valid for a (single) run,
       i.e., cached values won't be preserved across several
       executions.  Of course, this is safer, but it would be great to
       benefit from a long-term cache.  */
    cache_type cache;
    // FIXME: Rename cflags as cxxflags.
    std::list<std::string> cflags_, ldflags_;
    ruby::stream ruby;
  };

  function_loader_t function_loader;

  void
  include(const std::string& file)
  {
    includes_.push_back(file);
  }

  void
  include_dir(const std::string& dir)
  {
    function_loader.include_dir(dir);
  }

  void cflags(const std::string& elt)
  {
    function_loader.cflags(elt);
  }

  void ldflags(const std::string& elt)
  {
    function_loader.ldflags(elt);
  }

  void*
  load_function(fun_kind kind,
		const std::string& name,
		const arguments_types_t& arguments_types,
		const std::string& header_path)
  {
    if (HAVE_DYN_RUBY_GENERATOR)
      return function_loader.ruby_load(kind, name, arguments_types,
				       header_path);
    else
      return function_loader.cxx_load(kind, name, arguments_types,
				      header_path);
  }

} // end of namespace dyn

#endif // ! DYN_FUNCTION_LOADER_CC
