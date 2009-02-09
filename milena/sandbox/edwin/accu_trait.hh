#ifndef ACCU_TRAIT_HH_
# define ACCU_TRAIT_HH_

# include <mln/trait/undef.hh>

/// Shortcut to the accumulator property about untake method disponibility
# define mln_trait_accu_has_untake(A) typename mln::trait::accu::accu_traits< A >::has_untake

/// Shortcut to the accumulator property about setvalue method disponibility
# define mln_trait_accu_has_setvalue(A) typename mln::trait::accu::accu_traits< A >::has_setvalue

/// Shortcut to the accumulator property about stop method disponibility
# define mln_trait_accu_has_stop(A) typename mln::trait::accu::accu_traits< A >::has_stop

/// Shortcut to the accumulator property about behavior when pixel is given as take() value
/// Used for instance in mln::canvas::morpho::leveling
# define mln_trait_accu_when_pix(A) typename mln::trait::accu::accu_traits< A >::when_pix


namespace mln
{
  namespace morpho {
    namespace accu
    {
      template <typename T>
      struct card;
    }
  }

  namespace trait
  {
    namespace accu
    {

      // Basic accumulators properties

      struct has_untake
      {
        struct any { protected: any() {}; };

        struct no : any { std::string name() const { return "has_untake::no"; } };

        struct yes : any { std::string name() const { return "has_untake::yes"; } };
      };

      struct has_stop
      {
        struct any { protected: any() {}; };

        struct no : any { std::string name() const { return "has_setvalue::no"; } };

        struct yes : any { std::string name() const { return "has_stop::yes"; } };
      };

      struct has_setvalue
      {
        struct any { protected: any() {}; };

        struct no : any { std::string name() const { return "has_setvalue::no"; } };

        struct yes : any { std::string name() const { return "has_setvalue::yes"; } };
      };

      // Morphological accumulators properties
      struct when_pix
      {
        struct any { protected: any() {}; };

        struct use_v : any { std::string name() const { return "when_pix::use_v"; } };

        struct use_p : any { std::string name() const { return "when_pix::use_p"; } };

        struct use_pix : any { std::string name() const { return "when_pix::use_pix"; } };

	struct use_whatever { std::string name() const { return "when_pix::use_whatever"; } };

        struct not_ok : any { std::string name() const { return "when_pix::not_ok"; } };
      };

      template <typename A>
      struct accu_traits
      {
	typedef undef has_untake;
	typedef undef has_stop;
	typedef undef has_setvalue;
	typedef undef when_pix;
      };

      template <>
      template <typename T>
      struct accu_traits< mln::morpho::accu::card <T> >
      {
	typedef has_stop::no has_stop;
	typedef has_untake::yes has_untake;
	typedef has_setvalue::yes has_setvalue;
	typedef when_pix::use_p when_pix;
      };
    } //mln::accu

  } //mln::trait

} //mln
#endif /* !ACCU_TRAIT_HH_ */
