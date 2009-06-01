#include <mln/value/int_u8.hh>
#include <mln/value/int_s.hh>
#include <mln/value/rgb8.hh>
#include <mln/trait/value/comp.hh>

#include <mln/core/alias/box1d.hh>
#include <mln/core/image/image1d.hh>
#include <mln/core/image/image3d.hh>

#include <mln/core/concept/function.hh>
#include <mln/metal/ands.hh>



namespace mln
{


  // --------------------------------------------------  fun


  namespace fun
  {

    // . . . . . . . . . . . . . . .   v2v:: value <-> index

    // This is a revamp of mln/value/internal/convert.hh.  It remains
    // to be updated with Fred's new function writting.  Then we'll be
    // able to make it better, i.e., to handle the different cases
    // properly.  Precisely a case for scalars with low quantization
    // (with a special encoding like quantized float, or not), a case
    // for vectors with low quant, etc.

    namespace v2v
    {

      // value -> index

      template <typename T>
      struct index_of_value : Function_v2v< index_of_value<T> >,
			      private metal::bool_<(mln_dim(T) == 1)>::check_t
      {
	typedef unsigned result;
	unsigned operator()(const T& v) const
	{
	  return unsigned( int(v) - int(mln_min(T)) );
	}
      };

      template <>
      struct index_of_value<bool> : Function_v2v< index_of_value<bool> >
      {
	typedef unsigned result;
	unsigned operator()(bool b) const
	{
	  return b ? 1u : 0u;
	}
      };

      template <typename T>
      unsigned
      meta_index_of_value(const T& v)
      {
	index_of_value<T> f;
	return f(v);
      }

      // index -> value

      template <typename T>
      struct value_at_index : Function_v2v< value_at_index<T> >,
			      private metal::bool_<(mln_dim(T) == 1)>::check_t
      {
	typedef T result;
	T operator()(unsigned i) const
	{
	  return T( int(mln_min(T)) + int(i) );
	}
      };

      template <>
      struct value_at_index<bool> : Function_v2v< value_at_index<bool> >
      {
	typedef bool result;
	bool operator()(unsigned i) const
	{
	  mln_precondition(i < 2);
	  return i == 1u ? true : false;
	}
      };


      // . . . . . . . . . . . . . . .   value -> point n-D


      namespace internal
      {

	// FIXME: It can be straightforwardly written using
	// fun::ith_component.  Yet there is no test file for this
	// function.

	template <unsigned dim, typename T>
	struct point_from_value;

	template <typename T>
	struct point_from_value< 1, T >
	{
	  static point1d run(const T& v)
	  {
	    return point1d(meta_index_of_value(v));
	  }
	};

	template <typename T>
	struct point_from_value< 2, T >
	{
	  static point2d run(const T& v)
	  {
	    return point2d(meta_index_of_value(v[0]),
			   meta_index_of_value(v[1]));
	  }
	};

	template <typename T>
	struct point_from_value< 3, T >
	{
	  static point3d run(const T& v)
	  {
	    return point3d(meta_index_of_value(v[0]),
			   meta_index_of_value(v[1]),
			   meta_index_of_value(v[2]));
	  }
	};

      } // mln::fun::v2v::internal


      template <typename T>
      struct point_from_value : Function_v2v< point_from_value<T> >
      {
	enum { d = mln_dim(T) };
	typedef mln_regular_grid_from_dim(d) G;
	typedef mln::point<G, def::coord> result;

	result operator()(const T& v) const
	{
	  return internal::point_from_value<d,T>::run(v);
	}

	index_of_value<T> f_;
      };

      template <typename T>
      mln_result(point_from_value<T>)
      meta_point_from_value(const T& v)
      {
	point_from_value<T> f;
	return f(v);
      }

    } // mln::fun::v2v




    // . . . . . . . . . . . . . . .   p2p::fold*

    namespace p2p
    {

      struct fold1d : Function_v2v< fold1d >
      {
	typedef point1d result;
	result operator()(const point1d& p) const
	{
	  point1d tmp(p[0] % b.len(0));
	  return tmp;
	}
	box1d b;
      };

      template <bool along_0, bool along_1>
      struct fold2d : Function_v2v< fold2d<along_0,along_1> >
      {
	typedef point2d result;
	result operator()(const point2d& p) const
	{
	  point2d tmp(along_0 ? p[0] % b.len(0) : p[0],
		      along_1 ? p[1] % b.len(1) : p[1]);
	  return tmp;
	}
	box2d b;
      };

      template <bool along_0, bool along_1, bool along_2>
      struct fold3d : Function_v2v< fold3d<along_0,along_1,along_2> >
      {
	typedef point3d result;
	result operator()(const point3d& p) const
	{
	  point3d tmp(along_0 ? p[0] % b.len(0) : p[0],
		      along_1 ? p[1] % b.len(1) : p[1],
		      along_2 ? p[2] % b.len(2) : p[2]);
	  return tmp;
	}
	box3d b;
      };

    } // mln::fun::p2p
    

  } // mln::fun


  // --------------------------------------------------  value


  namespace value
  {

    template <unsigned n, int inf, int sup>
    struct circular
    {
    };


    namespace internal
    {
      template <typename T>
      struct is_circular_helper : metal::false_
      {};
      
      template <unsigned n, int inf, int sup>
      struct is_circular_helper< circular<n,inf,sup> > : metal::true_
      {};

      template <typename T>
      struct is_circular : is_circular_helper<T>::eval
      {
      };
      
    } // mln::value::internal

  } // mln::value


  namespace trait
  {

    template <unsigned n, int inf, int sup>
    struct value_< mln::value::circular<n, inf, sup> >
    {
      enum {
	dim = 1,
	nbits = n,
	card = mln_value_card_from_(n)
      };
      // ...
    };

  } // end of namespace trait


  // --------------------------------------------------  histo


  namespace histo
  {

    namespace internal
    {
      
      template <int dim, typename T>
      struct image_from_value_helper
      {
      };

      // dim = 1

      template <typename T, bool is_c>
      struct image_from_value_helper_1;

      template <typename T>
      struct image_from_value_helper_1< T, false >
      {
	typedef image1d<unsigned> ret;

	static ret get_image()
	{
	  ret tmp(mln_card(T));
	  return tmp;
	}
      };

      template <typename T>
      struct image_from_value_helper_1< T, true >
      {
	typedef void ret;
      };

      template <typename T>
      struct image_from_value_helper< 1, T >
      {
	enum { is_c = value::internal::is_circular<T>::value };
	typedef image_from_value_helper_1< T, is_c > helper;
      };

      // dim = 3

      template <typename C0, bool is_c0,
		typename C1, bool is_c1,
		typename C2, bool is_c2,
		typename T>
      struct image_from_value_helper_3;

      template <typename C0, typename C1, typename C2,
		typename T>
      struct image_from_value_helper_3< C0, false,
					C1, false,
					C2, false,
					T >
      {
	typedef image3d<unsigned> ret;
	static ret get_image()
	{
	  ret tmp(mln_card(C0), mln_card(C1), mln_card(C2));
	  return tmp;
	}
      };

      template <typename C0, typename C1, typename C2,
		typename T>
      struct image_from_value_helper_3< C0, true,
					C1, false,
					C2, false,
					T >
      {
	typedef void ret;
      };
      
      template <typename T>
      struct image_from_value_helper< 3, T >
      {
	typedef mln_trait_value_comp(T, 0) C0;
	typedef mln_trait_value_comp(T, 1) C1;
	typedef mln_trait_value_comp(T, 2) C2;
	typedef image_from_value_helper_3< C0, value::internal::is_circular<C0>::value,
					   C1, value::internal::is_circular<C1>::value,
					   C2, value::internal::is_circular<C2>::value,
					   T > helper;
      };
    
      template <typename T>
      struct image_from_value_ // Entry point.
      {
	typedef typename image_from_value_helper<mln_dim(T), T>::helper helper;
      };
    

    } // mln::histo::internal
    
    template <typename T>
    struct image_from_value : internal::image_from_value_<T>::helper
    {
    };



    // First code: the result of this function shall be replaced by
    // an histo::image<T> where T is mln_value(I).  As a consequence,
    // we will not have to call meta_point_from_value...

    template <typename I>
    typename image_from_value<mln_value(I)>::ret
    compute_image(const Image<I>& input_)
    {
      const I& input = exact(input_);
      typedef image_from_value<mln_value(I)> helper;
      typename helper::ret h = helper::get_image();

      mln_piter(I) p(input.domain());
      for_all(p)
	++h( fun::v2v::meta_point_from_value(input(p)) );

      return h;
    }

  } // mln::histo

} // mln


int main()
{
  using namespace mln;

  {
    typedef bool T;
    std::cout << histo::image_from_value<T>::get_image().domain() << std::endl;
  }

  {
    typedef unsigned char T;
    std::cout << histo::image_from_value<T>::get_image().domain() << std::endl;
  }

  {
    typedef value::int_s<3> T;
    std::cout << histo::image_from_value<T>::get_image().domain() << std::endl;
  }

  {
    typedef value::rgb8 T;
    std::cout << histo::image_from_value<T>::get_image().domain() << std::endl;
  }

  {
    //   typedef value::circular<8,0,9> T;
  }
}
