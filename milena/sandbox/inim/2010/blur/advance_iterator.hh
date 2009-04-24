#ifndef ADVANCE_ITERATOR_HH_
# define ADVANCE_ITERATOR_HH_

# define MLN_INCLUDE_ONLY
# include <mln/core/alias/point2d.hh>
# include <mln/core/alias/dpoint2d.hh>
# include <mln/core/image/image2d.hh>
# undef MLN_INCLUDE_ONLY
# include <vector>

// This iterator browses the neighbors of a given point, in a
// particular order, and only when the corresponding pixel in the
// water image is black.
template <typename Value>
class AdvanceIterator
{
public:
  AdvanceIterator (mln::image2d<Value>& water,
		   mln::image2d<Value>& result,
		   mln::point2d p);

  // Initialize the neighbor iterator
  void start ();

  // Return the number of neighbors that would be iterated on
  unsigned count ();

  // Return whether the neighbor iteration has finished
  bool is_valid ();

  // Advance the neighbor iterator
  void next ();

  // Retrieve the point around which we iterate
  mln::point2d center ();

  // Reset to iterate on the neighborhood of the given point
  void recenter (mln::point2d p);

  // Get the current neighbor
  mln::point2d operator*();
  mln::point2d operator->();
private:
  mln::point2d origin_;
  std::vector<mln::point2d> points_;
  std::vector<mln::point2d>::const_iterator current_;
  mln::image2d<Value>& water_;
  mln::image2d<Value>& result_;

  static mln::dpoint2d dpoints[];
};

# include "advance_iterator.hxx"

#endif /* !ADVANCE_ITERATOR_HH_ */
