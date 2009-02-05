#ifndef CARD_HH_
# define CARD_HH_

# include <mln/accu/all.hh>

namespace mln
{
  namespace morpho
  {
    namespace accu
    {
      template <typename T>
      struct card : public mln::accu::internal::base< unsigned, card<T> >
      {
	typedef T argument;

	card () { init(); };
	void init () { c_ = 0; };
	void take (const card<T>& accu) { c_ += accu.c_; };
	void take (const T& elt) { ++c_; };
	unsigned to_result() const { return c_; };
	bool is_valid () const { return true; };

      private:
	unsigned c_;
      };
    } // mln::morpho::accu
  } // mln::morpho
} // mln

#endif /* !CARD_HH_ */


