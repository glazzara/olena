//
// util.hxx for inim
// Made by nicuveo <crucuny@gmail.com>
//

#ifndef UTIL_HXX_
# define UTIL_HXX_



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Includes

# include <cassert>
# include <string>
# include <vector>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image1d.hh>
# include <mln/value/all.hh>
# include <mln/accu/maj_h.hh>
# include <mln/border/resize.hh>
# include <mln/win/rectangle2d.hh>
# include "snake2d.hh"



//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
// Declarations

std::vector<mln::value::int_u8>
colors_init (const mln::image1d<size_t>& wst)
{
  std::vector<mln::value::int_u8> res;
  mln::image1d<size_t>::fwd_piter p (wst.domain ());

  for_all (p)
  {
    if (wst (p) != 0)
      continue;

    if (p[0] != 0)
    {
      res.push_back (p[0]);
    }
  }

  if (res.empty() || res.back () != 255u)
    res.push_back (255u);

  return res;
}

mln::value::int_u8
colors_match (const std::vector<mln::value::int_u8>& vec, mln::value::int_u8 x)
{
  unsigned i;

  for (i = 0; i < vec.size (); ++i)
    if (x <= vec[i])
      break;

  if (vec.size() == 1)
    return (127);
  return (i * 255) / (vec.size () - 1);
}

template <typename T>
void
print_histo (const mln::image1d<T>& hist, const std::string& f)
{
  std::ofstream fo;
  typename mln::image1d<T>::fwd_piter p (hist.domain ());

  fo.open (f.c_str ());

  for_all (p)
    fo << p[0] << "\t" << hist (p) << std::endl;

  fo.close ();
}

mln::image2d<mln::value::int_u8>
binarization(mln::image2d<mln::value::int_u8>& input, int size)
{
  mln::image2d<mln::value::int_u8> output;
  initialize(output, input);

  mln::border::resize(input, 0);
  mln::image2d<mln::value::int_u8> maj = snake2d(mln::accu::maj_h<mln::value::int_u8>(), input, mln::win::rectangle2d(size,size));
  mln::image2d<mln::value::int_u8>::fwd_piter p (input.domain());
  for_all (p)
    output(p) = (input(p) != maj(p)) * 255;
  return output;
}

#endif /* !UTIL_HH_ */
