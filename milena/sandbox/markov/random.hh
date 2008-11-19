#ifndef RANDOM_HH_
# define RANDOM_HH_



template <typename T>
class Random
{
  public:
    Random (T inf, T sup);

    T get () const;

  public:
    T inf_;
    T sup_;
};



# include "random.hxx"



#endif /* !RANDOM_HH_ */
