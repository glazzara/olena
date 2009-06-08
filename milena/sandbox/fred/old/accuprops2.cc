# include <iostream>
# include <string>

# include <mln/trait/undef.hh>
# include <mln/trait/image/props.hh>
# include <mln/trait/value_.hh>

# include <mln/metal/bexpr.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_const.hh>

# include <mln/core/concept/image.hh>
# include <mln/accu/all.hh>
# include <mln/util/pix.hh>
# include <mln/make/pix.hh>

/// Shortcut to the accumulator property about untake method disponibility
# define mln_trait_accu_has_untake(A) typename mln::trait::accu_< A >::has_untake

/// Shortcut to the accumulator property about setvalue method disponibility
# define mln_trait_accu_has_setvalue(A) typename mln::trait::accu_< A >::has_setvalue

/// Shortcut to the accumulator property about stop method disponibility
# define mln_trait_accu_has_stop(A) typename mln::trait::accu_< A >::has_stop

/// Shortcut to the accumulator property about behavior when pixel is given as take() value
/// Used for instance in mln::canvas::morpho::dataing
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

        struct use_v : any { std::string name() const { return "when_pix::use_v"; } };

        struct use_p : any { std::string name() const { return "when_pix::use_p"; } };

        struct use_pix : any { std::string name() const { return "when_pix::use_pix"; } };

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
      typedef accu::when_pix::not_ok when_pix;
      // FIXME: should default be undef ?
    };

    template <typename A>
    struct accu_ : undefined_accu_<A>
    {
    };

  }  // end of namespace mln::trait

  namespace canvas {

    namespace morpho {

      namespace impl {

        namespace generic {

          template <typename I, template <typename P> class A>
          inline
          void
          leveling(const Image<I>& input,
              Accumulator< A< util::pix<I> > >& acc)
          {
            const I& ima = exact(input);
            A< util::pix<I> >& a = exact(acc);

            mln_piter(I) p(ima.domain());

            for_all(p)
              a.take(mln::make::pix(ima, p));
          }

        } // end of namespace mln::canvas::morpho::impl::generic

        // fast implementation (only on values from pixter)
        template <typename I, template <typename P> class A>
        inline
        void
        leveling_fastest(const Image<I>& input,
            Accumulator< A< util::pix<I> > >& acc)
        {
          const I& ima = exact(input);
          A< util::pix<I> >& a = exact(acc);

          mln_pixter(const I) px(ima);

          for_all(px)
            a.take(px.val());
        }

      }  // end of namespace mln::canvas::morpho::impl

      namespace internal {

        template <typename I, typename A>
        inline
        void
        leveling_dispatch(metal::false_,
            const Image<I>& input,
            Accumulator<A>& acc)
        {
          impl::generic::dataing(input, acc);
        }

        template <typename I, typename A>
        inline
        void
        leveling_dispatch(metal::true_,
            const Image<I>& input,
            Accumulator<A>& acc)
        {
          impl::dataing_fastest(input, acc);
        }

        template <typename I, typename A>
        inline
        void
        leveling_dispatch(const Image<I>& input,
            Accumulator<A>& acc)
        {
          enum {
            test = mlc_equal(mln_trait_image_speed(I),
                trait::image::speed::fastest)::value
                &&
                mlc_equal(mln_trait_accu_when_pix(A),
                    trait::accu::when_pix::use_v)::value
          };
          internal::dataing_dispatch(metal::bool_<test>(),
              input, acc);
        }

      }  // end of namespace mln::canvas::morpho::internal

      // Facade.
      template <typename I, template <typename P> class A>
      inline
      void
      leveling(const Image<I>& input,
          Accumulator< A< util::pix<I> > >& acc)
      {
        internal::dataing_dispatch(input, acc);
      }

    }  // end of namespace mln::canvas::morpho

  }  // end of namespace mln::canvas

  namespace morpho {

    namespace accu {

      // MEAN ACCUMULATOR
      namespace internal {

        template <typename V>
        struct sum_handling_pixels
        { typedef mln_sum(V) ret; };

        template <typename I>
        struct sum_handling_pixels< util::pix<I> >
        { typedef mln_sum(mln_value(I)) ret; };

      } // end of namespace mln::morpho::accu::internal

      template <typename T>
      /// Morphological (i.e. for pixel and pixel values) accumulator calculating mean.
      /// FIXME: is inclusion polyphormism really appliable ?
      struct mean : public mln::accu::mean<T, typename internal::sum_handling_pixels<T>::ret>,
                    public Accumulator< mean<T> >
      {
        typedef mln::accu::mean<T, typename internal::sum_handling_pixels<T>::ret> super;

        using super::operator typename super::result;
        using super::take;

        void take(const mean<T>& m);
      };

      template <typename I>
      struct mean< util::pix<I> > : public mean< mln_value(I) >,
                                    public Accumulator< mean< util::pix<I> > >
      {
        typedef mean< mln_value(I) > super;
        typedef util::pix<I> argument;

        using super::operator typename super::result;
        using super::take;

        void take(const argument& t);
        void take(const mean< util::pix<I> >& m);
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename T>
      inline
      void mean<T>::take(const mean<T>& m)
      {
        this->super::take(m);
      }

      template <typename I>
      inline
      void mean< util::pix<I> >::take(const mean< util::pix<I> >::argument& t)
      {
        this->mean< mln_value(I) >::take(t.v());
      }

      template <typename I>
      inline
      void mean< util::pix<I> >::take(const mean< util::pix<I> >& m)
      {
        this->super::take(m);
      }

# endif // ! MLN_INCLUDE_ONLY

      // FROM mln::accu:: TO mln::morpho::acu::

      template <template <typename T> class A>
      struct from_mln_accu
      {
        template <typename T>
        struct ret;

        template <typename I>
        struct ret< util::pix<I> > : public A< mln_value(I) >,
                                     public Accumulator< ret< util::pix<I> > >
        {
          typedef A< mln_value(I) > super;
          typedef typename super::result result;

          typedef util::pix<I> argument;

          using super::take;

          void take(const argument& t);
          void take(const ret< util::pix<I> >& m);
        };
      };

# ifndef MLN_INCLUDE_ONLY

      template <template <typename T> class A>
      template <typename I>
      inline
      void from_mln_accu<A>::ret< util::pix<I> >::take(const from_mln_accu<A>::ret< util::pix<I> >& m)
      {
        this->super::take(m);
      }

      template <template <typename T> class A>
      template <typename I>
      inline
      void from_mln_accu<A>::ret< util::pix<I> >::take(const from_mln_accu<A>::ret< util::pix<I> >::argument& t)
      {
        this->A< mln_value(I) >::take(t.v());
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::accu

  } // end of namespace mln::morpho

  namespace trait {

    template <typename I>
    struct accu_< morpho::accu::mean< util::pix<I> > > :
      public undefined_accu_ < morpho::accu::mean< util::pix<I> > >
    {
      typedef accu::when_pix::use_v when_pix;
    };

    template <template <typename T> class A, typename I>
    struct accu_< morpho::accu::from_mln_accu < A > :: ret < I > >
    {
      typedef accu::when_pix::use_v when_pix;
    };
  } // end of namespace mln::trait

} // end of namespace mln

# include <mln/accu/all.hh>
# include <mln/core/image/image2d.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/core/var.hh>
# include <mln/util/timer.hh>
int main()
{
  using namespace mln;
  typedef image2d<int> I;

  I ima(1000, 1000);
  morpho::accu::from_mln_accu<accu::stat::min>::ret<util::pix<I> > acc;

  float elapsed;
  mln::util::timer chrono;

  debug::iota(ima);
  std::cout << "50 mean of a 1000x1000 image2d<int>" << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::canvas::morpho::dataing(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(auto) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::canvas::morpho::impl::generic::dataing(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(generic) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::canvas::morpho::impl::dataing_fastest(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(fast) " << elapsed << "s : " << acc.to_result() << std::endl;
}
