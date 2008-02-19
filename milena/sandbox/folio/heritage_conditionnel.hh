#ifndef HERITAGE_CONDITIONNEL_HH_
# define HERITAGE_CONDITIONNEL_HH_

# include <iostream>

class TopLeft
{
protected:
  TopLeft()
  {
    std::cout << "top left" << std::endl;
  }
};

class TopRight
{
protected:
  TopRight()
  {
    std::cout << "top right" << std::endl;
  }
};

template<bool C, typename T, typename U>
struct TraitCond
{
  typedef T type;
};

template<typename T, typename U>
struct TraitCond<false, T, U>
{
  typedef U type;
};

template<bool B>
class Bottom: public TraitCond<B, TopLeft, TopRight>::type
{
public:
  Bottom()
    : TraitCond<B, TopLeft, TopRight>::type()
  {
  }
};

#endif /* !HERITAGE_CONDITIONNEL_HH_ */
