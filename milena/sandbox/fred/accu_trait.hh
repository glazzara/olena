#ifndef MLN_TRAIT_ACCU_HH
# define MLN_TRAIT_ACCU_HH

# include <string>
# include <mln/trait/undef.hh>

/// Shortcut to the accumulator property about untake method disponibility
# define mln_trait_accu_has_untake(A) typename mln::trait::accu_< A >::has_untake

/// Shortcut to the accumulator property about setvalue method disponibility
# define mln_trait_accu_has_setvalue(A) typename mln::trait::accu_< A >::has_setvalue

/// Shortcut to the accumulator property about stop method disponibility
# define mln_trait_accu_has_stop(A) typename mln::trait::accu_< A >::has_stop

/// Shortcut to the accumulator property about behavior when pixel is given as take() value
/// Used for instance in mln::canvas::morpho::leveling
# define mln_trait_accu_when_pix(A) typename mln::trait::accu_< A >::when_pix

namespace mln {

  namespace trait {

    namespace accu {

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

        struct use_only_v : any { std::string name() const { return "when_pix::use_only_v"; } };

        struct use_only_p : any { std::string name() const { return "when_pix::use_only_p"; } };

        struct use_pix : any { std::string name() const { return "when_pix::use_pix"; } };

        struct use_whatever : any { std::string name() const { return "when_pix::use_whatever"; } };

        struct not_ok : any { std::string name() const { return "when_pix::not_ok"; } };
      };

    }  // end of namespace mln::trait::accu

    template <typename A>
    struct undefined_accu_
    {
      // general
      typedef undef has_untake;
      typedef undef has_stop;
      typedef undef has_setvalue;

      // morpho
      typedef accu::when_pix::use_pix when_pix;
    };

    template <typename A>
    struct accu_ : public undefined_accu_<A>
    {
    };

  }  // end of namespace mln::trait

} // end of namespace mln

#endif /* !MLN_TRAIT_ACCU_HH */
