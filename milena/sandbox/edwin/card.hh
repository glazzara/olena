#ifndef CARD_HH_
# define CARD_HH_

# include <mln/accu/all.hh>
# include <mln/util/pix.hh>

namespace mln
{
  namespace morpho
  {
    namespace accu
    {
      template <typename I>
      struct card : public mln::accu::internal::base< unsigned, card<I> >
      {
	typedef mln_psite(I) argument;

	card () { init(); };
	void init () { c_ = 0; };

	void take (const card<I>& accu) { c_ += accu.c_; };

	void take () { ++c_; };
	void take (const mln_psite(I)& elt) { ++c_; };
	void take (const mln_value(I)& elt) { ++c_; };
	void take (const util::pix<I>& pix) { ++c_; };

	unsigned to_result() const { return c_; };

	bool is_valid () const { return true; };

      private:
	unsigned c_;
      };
    } // mln::morpho::accu
  } // mln::morpho
} // mln

#endif /* !CARD_HH_ */


