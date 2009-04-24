#ifndef ADVANCE_ITERATOR_HXX
# define ADVANCE_ITERATOR_HXX

#include "advance_iterator.hh"

template <typename Value>
mln::dpoint2d AdvanceIterator<Value>::dpoints[] =
{
  mln::dpoint2d (0, 1),
  mln::dpoint2d (-1, 1),
  mln::dpoint2d (1, 1),
  mln::dpoint2d (1, 0),
  mln::dpoint2d (-1, 0)
};

template <typename Value>
AdvanceIterator<Value>::AdvanceIterator (mln::image2d<Value>& water,
					 mln::image2d<Value>& result,
					 mln::point2d p)
  : origin_ (p),
    water_ (water),
    result_ (result)
{
  points_.reserve (5);
  this->recenter (p);
}

template <typename Value>
void
AdvanceIterator<Value>::recenter (mln::point2d p)
{
  points_.clear ();
  origin_ = p;
  for (int i = 0; i < 3; ++i)
    if (result_.has(p + dpoints[i])
	&& water_ (p + dpoints[i]) == 0u
// 	&& result_(p + dpoints[i]) != 0u
      )
    {
//       std::cout << water_ (p + dpoints[i]) << std::endl;
      points_.push_back (p + dpoints[i]);
    }
  current_ = points_.begin ();
}

template <typename Value>
unsigned
AdvanceIterator<Value>::count ()
{
  return points_.size ();
}

template <typename Value>
void
AdvanceIterator<Value>::start ()
{
  current_ = points_.begin ();
}

template <typename Value>
bool
AdvanceIterator<Value>::is_valid ()
{
  return current_ != points_.end ();
}

template <typename Value>
void
AdvanceIterator<Value>::next ()
{
  ++current_;
}

template <typename Value>
mln::point2d
AdvanceIterator<Value>::center ()
{
  return origin_;
}

template <typename Value>
mln::point2d
AdvanceIterator<Value>::operator* ()
{
  return *current_;
}

template <typename Value>
mln::point2d
AdvanceIterator<Value>::operator-> ()
{
  return *current_;
}

#endif /* !ADVANCE_ITERATOR_HXX */
