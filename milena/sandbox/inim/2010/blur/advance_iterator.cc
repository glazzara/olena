#include "advance_iterator.hh"

template <typename Value>
mln::dpoint2d* AdvanceIterator<Value>::dpoints =
{
  mln::dpoint2d (1, 0),
  mln::dpoint2d (1, 1),
  mln::dpoint2d (1, -1),
  mln::dpoint2d (0, 1),
  mln::dpoint2d (0, -1)
};

template <typename Value>
AdvanceIterator<Value>::AdvanceIterator (mln::image2d<Value>& water,
					 mln::point2d p)
  : origin_ (p),
    water_ (water)
{
  this->reinit (p);
}

template <typename Value>
void
AdvanceIterator<Value>::reinit (mln::point2d p)
{
  points_.clear ();
  origin_ = current_ = p;
  for (int i = 0; i < 5; ++i)
    if (0 == water_ (p + dpoints[i]))
      points_.push_front (p + dpoints[i]);
}

template <typename Value>
bool
AdvanceIterator<Value>::has_point ()
{
  return current_ != points_.end ();
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
  return has_point () && water_.has (*current_);
}

template <typename Value>
void
AdvanceIterator<Value>::next ()
{
  ++current_;
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
