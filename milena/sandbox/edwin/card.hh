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

	card ();
	void init ();

	void take (const card<I>& accu);

	void take ();
	void take (const mln_psite(I)& elt);
	void take (const mln_value(I)& elt);
	void take (const util::pix<I>& pix);

	unsigned to_result() const;

	bool is_valid () const;

      private:
	unsigned c_;
      };

# ifndef MLN_INCLUDE_ONLY
      template <typename I>
      inline
      card<I>::card () :
	c_ (0)
      {
      }

      template <typename I>
      inline
      void
      card<I>::init ()
      {
	c_ = 0;
      }

      template <typename I>
      inline
      void
      card<I>::take (const card<I>& accu)
      {
	c_ += accu.c_;
      }


      template <typename I>
      inline
      void
      card<I>::take ()
      {
	++c_;
      };

      template <typename I>
      inline
      void
      card<I>::take (const mln_psite(I)& elt)
      {
	++c_;
      };


      template <typename I>
      inline
      void
      card<I>::take (const mln_value(I)& elt)
      {
	++c_;
      };


      template <typename I>
      inline
      void
      card<I>::take (const util::pix<I>& pix)
      {
	++c_;
      };

      template <typename I>
      inline
      unsigned
      card<I>::to_result() const
      {
	return c_;
      };

      template <typename I>
      inline
      bool
      card<I>::is_valid () const
      {
	return true;
      };
# endif


    } // mln::morpho::accu
  } // mln::morpho
} // mln

#endif /* !CARD_HH_ */


