#ifndef CHECK_SIZE_FUNCTOR_HH_
# define CHECK_SIZE_FUNCTOR_HH_

# include <mln/core/concept/function.hh>

template <typename P, typename L>
class CheckBoxSize : public mln::Function_v2b< CheckBoxSize<P, L> >
{
public:

  CheckBoxSize(mln::util::array< mln::box<P> > boxes)
    : boxes_ (boxes)
  {
  }

  bool operator() (L label) const
  {
    bool condition =
      boxes_[label].len(1) >= 200 ||
      boxes_[label].len(0) >= 200;
    return condition;
  }

private:

  mln::util::array< mln::box<P> > boxes_;

};

#endif /* !CHECK_SIZE_FUNCTOR_HH_ */
