#ifndef ADVANCE_ITERATOR_HH_
# define ADVANCE_ITERATOR_HH_

# define MLN_INCLUDE_ONLY
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/image/image2d.hh>
# undef MLN_INCLUDE_ONLY
# include <list>

template <typename Value>
class AdvanceIterator
{
public:
  AdvanceIterator (mln::image2d<Value>& water, mln::point2d p);
  void start ();
  bool is_valid ();
  bool has_point ();
  void next ();
  void reinit (mln::point2d p);
  mln::point2d operator*();
  mln::point2d operator->();
private:
  mln::point2d origin_;
  std::list<mln::point2d> points_;
  std::list<mln::point2d>::const_iterator current_;
  mln::image2d<Value>& water_;

  static mln::dpoint2d* dpoints;
};

#endif /* !ADVANCE_ITERATOR_HH_ */
