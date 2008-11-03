/*! \file src/io.hh
 *
 * Contains various method to load/save an image
 */

#ifndef SRC_IO_HH
# define SRC_IO_HH

#include <mln/core/image/image2d.hh>

#include <mln/value/int_u16.hh>
#include <mln/value/int_s16.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/load.hh>
# include <mln/io/ppm/save.hh>

namespace IO
{

  template <typename I>
  void load(I& ima, const std::string& file)
  {
    mln::io::ppm::load(ima, file);
  }

  template <>
  void load(mln::image2d<mln::value::int_u16>& ima,
            const std::string& file)
  {
    mln::io::pgm::load(ima, file);
  }

  template <typename I>
  void save(I& ima, const std::string& file)
  {
    mln::io::ppm::save(ima, file);
  }

  template <>
  void save(mln::image2d<mln::value::int_u16>& ima,
            const std::string& file)
  {
    mln::io::pgm::save(ima, file);
  }

  template <>
  void save(mln::image2d<mln::value::int_s16>& ima,
            const std::string& file)
  {
    mln::io::pgm::save(ima, file);
  }

} // !IO

#endif // !SRC_IO_HH
