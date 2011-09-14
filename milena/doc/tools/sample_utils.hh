// Copyright (C) 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef DOC_TOOLS_SAMPLE_UTILS_HH
# define DOC_TOOLS_SAMPLE_UTILS_HH

# include <mln/core/image/image2d.hh>
# include <mln/io/pbm/all.hh>
# include <mln/io/pgm/all.hh>
# include <mln/io/ppm/all.hh>

# include "doc/tools/data.hh"

namespace doc
{

  inline
  std::string
  file(const std::string& name, const std::string& extension)
  {
    static int file_id = 1;

    std::ostringstream os;
    os << MLN_DOC_DIR "/figures/"
      << name
      << "-"
      << file_id++
      << "."
      << extension;
    return os.str();
  }

  template <typename I>
  inline
  void
  ppmsave(const mln::Image<I>& ima, const std::string& name)
  {
    mln::io::ppm::save(ima, file(name, "ppm"));
  }

  template <typename I>
  inline
  void
  pbmsave(const mln::Image<I>& ima, const std::string& name)
  {
    mln::io::pbm::save(ima, file(name, "pbm"));
  }

  template <typename I>
  inline
  void
  pgmsave(const mln::Image<I>& ima, const std::string& name)
  {
    mln::io::pgm::save(ima, file(name, "pgm"));
  }

  inline
  void
  begin_output()
  {
    std::cout << "// \\{" << std::endl;
  }

  inline
  void
  end_output()
  {
    std::cout << "// \\}" << std::endl;
  }

} // end of namespace doc

#endif // ! DOC_TOOLS_SAMPLE_UTILS_HH

