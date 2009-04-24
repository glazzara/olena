#ifndef VECT_IT_HH
# define VECT_IT_HH

# define MLN_INCLUDE_ONLY
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/image/image2d.hh>
# undef MLN_INCLUDE_ONLY
# include <list>
# include "vector.hh"

template <typename Value>
class VectorIterator
{
public:
  VectorIterator (mln::image2d<Value>& water,
                  mln::point2d p,
                  e_orient orient);
  void start ();
  bool is_valid ();
  bool has_point ();
  void next ();
  void reinit (mln::point2d p,
               e_orient orient);
  mln::point2d operator*();
  mln::point2d operator->();
private:
  mln::point2d origin_;
  std::list<mln::point2d> points_;
  std::list<mln::point2d>::const_iterator current_;
  mln::image2d<Value>& water_;

  static mln::dpoint2d* dpoints;
};

# include "vect_it.hxx"

#endif /* !VECT_IT_HH */
