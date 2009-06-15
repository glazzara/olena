
#include <mln/pw/image.hh>
#include <mln/convert/to_fun.hh>
#include <mln/accu/stat/max.hh>


namespace mln
{

  // To be added in mln/pw/image.hh

  template <typename V, typename P, typename S>
  pw::image< fun::C<V(*)(P)>, S >
  operator | (V (*f)(P), const S& s)
  {
    return convert::to_fun(f) | s;
  }


//   namespace accu
//   {
//     template <typename T> struct sup : max<T> {};
//   }


  namespace accu
  {
    template <typename T>
    struct sup : public accu::internal::base< T, sup<T> >
    {
      typedef T argument;
      T t_;

      void init()             { t_ = mln_min(T); }
      void take(const T& t)   { if (t > t_) t_ = t; }
      void take(const sup& o) { if (o.t_ > t_) t_ = o.t_; }
      T to_result() const     { return t_; }
      bool is_valid() const   { return true; }
    };

  } // mln::accu


  namespace morpho
  {

    template <typename I, typename W>
    mln_concrete(I) dilation(const I& ima, const W& win)
    {
      mln_concrete(I) out;
      initialize(out, ima);

      mln_piter(I) p(ima.domain());
      mln_qiter(W) q(win, p);
      accu::math::sup<mln_value(I)> sup;

      for_all(p)
	{
	  sup.init();
	  for_all(q) if (ima.has(q))
	    sup.take(ima(q));
	  out(p) = sup;
	}

      return out;
    }

  } // mln::morpho

} // mln
