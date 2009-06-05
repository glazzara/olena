#include <mln/core/concept/function.hh>
#include <mln/core/concept/value.hh>
#include <mln/fun/internal/resolve.hh>


namespace mln
{


  // Forward declaration.
  namespace fun {
    namespace vv2v {
      template <typename T> struct inf;
    }
  }


  namespace trait
  {

    // Default (whatever the category):
    // "inf" performs a "min"!
    template <template <class> class Category, typename T>
    struct set_unary_< fun::vv2v::inf, Category, T >
    {
      typedef set_unary_< fun::vv2v::inf, Category, T > ret;

      static T exec(const T& t1, const T& t2)
      {
	return t1 < t2 ? t1 : t2;
      }

      // In the case of a binary function with two different argument
      // types, we shall instrument this definition with the result
      // type...
    };

  } // mln::trait



  namespace fun
  {

    namespace vv2v
    {

      // fun::vv2v::inf<T>

      template <typename T>
      struct inf : Function_vv2v< inf<T> >
      {
	typedef T result;

	typedef mln_fun_internal_resolve(inf) impl;

	T operator()(const T& t1, const T& t2) const
	{
	  return impl::exec(t1, t2);
	}
      };


      // fun::vv2v::meta::inf

      namespace meta
      {
      
	struct inf
	{

	  // A meta-fun can act as a function :-)

	  template <typename T>
	  T operator()(const T& t1, const T& t2) const
	  // Here, we know the result type of vv2v::inf<T> so
	  // we explictly write it.
	  {
	    fun::vv2v::inf<T> f;
	    return f(t1, t2);
	  }

	  // The "meta-fun -> fun" code is similar to the one in
	  // mln/accu/stat/min.hh
	  template <typename T>
	  struct with
	  {
	    typedef fun::vv2v::inf<T> ret;
	  };
	};
      
      } // mln::fun::vv2v::meta

    } // mln::fun::vv2v

  } // mln::fun



  // Yay! A special type equipped with a particular 'inf'.

  struct rgb : Value<rgb>
  {
    typedef rgb enc;
    typedef rgb equiv; // Those couple of typedefs are required by the concept.
    rgb() {}
    rgb(int r, int g, int b) : r(r), g(g), b(b) {}
    int r, g, b;
  };

  std::ostream& operator<<(std::ostream& ostr, const rgb& c)
  {
    ostr << c.r << ' ' << c.g << ' ' << c.b;
  }

  namespace trait
  {

    template <>
    struct set_precise_unary_< fun::vv2v::inf, rgb >
    {
      typedef set_precise_unary_< fun::vv2v::inf, rgb > ret;

      static rgb exec(const rgb& c1, const rgb& c2)
      {
	// "Inf" is component-wise "min".
	return rgb(c1.r < c2.r ? c1.r : c2.r,
		   c1.g < c2.g ? c1.g : c2.g,
		   c1.b < c2.b ? c1.b : c2.b);
      }
    };
    
  } // mln::trait

} // mln



int main()
{
  using namespace mln;

  fun::vv2v::meta::inf inf;
  std::cout << inf(3, 5) << std::endl;

  rgb c1(1, 2, 3), c2(2, 1, 2);
  std::cout << inf(c1, c2) << std::endl;
}
