#ifndef RANDOM_HH_
# define RANDOM_HH_



template <typename T>
class Random
{
public:
  Random (T inf, T sup);

  T get ();
  T gen () const;

public:
  static const unsigned size_ = 10000;
  T inf_;
  T sup_;
  int i_;
  T values_[size_];
};



# include "random.hxx"



#endif /* !RANDOM_HH_ */
