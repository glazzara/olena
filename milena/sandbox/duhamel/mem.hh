#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>


#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>

#include <mln/data/fill.hh>

#include <mln/debug/println_with_border.hh>

# include <mln/core/image/sparse_image.hh>
# include <mln/core/image/sparse_encode.hh>

# include <mln/core/image/rle_image.hh>
# include <mln/core/image/rle_encode.hh>

namespace mln
{
  namespace level
  {

    template <typename I>
    void imaset_(I& ima, const mln_value(I)& value, std::size_t n)
    {
      // FIXME Preconditions
      mln_value(I)* ptr = &ima[0];
      
      for (std::size_t i = 0; i < n; ++i)
	*ptr++ = value;
    }

    template <typename I, typename J>
    void imaset_(sparse_image<I, J>& ima_,
		 const mln_value(J)& value_,
		 std::size_t n)
    {
      // FIXME
      const mln_value(I)& v;
      std::size_t i = n;

      i = n;
      v = value_;
      ima_.domain ();
      std::cerr << "imaset_ sparse_image\n";
    }
    
    template <typename I, typename J>
    void imaset_(rle_image<I, J>& ima_,
		 const mln_value(J)& value_,
		 std::size_t n)
    {
      // FIXME
//       mln::internal::run_pset_<I> my_set;
      const mln_value(J)& v;

      for (std::size_t i = 0; i < n; ++i)
 	;

      v = value_;
      ima_.domain ();
      std::cerr << "imaset_ sparse_image\n";
    }
    
    template <typename I>
    void imacpy_(I& dst, I& src, std::size_t n)
    {
      // FIXME Preconditions
      mln_value(I)* u = &dst[0];
      mln_value(I)* v = &src[0];
      
      for (std::size_t i = 0; i < n; ++i)
	*u++ = *v++;
    }

    template <typename I>
    void imacpy_(I& dst, const I& src, std::size_t n)
    {
      // FIXME Preconditions
      mln_value(I)* u = &dst[0];
      const mln_value(I)* v = &src[0];
      
      for (std::size_t i = 0; i < n; ++i)
	*u++ = *v++;
    }

    template <typename I, typename J>
    void imacpy_(sparse_image<I, J>& dst,
		 const sparse_image<I, J>& src,
		 std::size_t n)
    {
      //      mln::internal::run_pset_<I> my_set;

      for (std::size_t i = 0; i < n; ++i)
	;

      dst.domain ();
      src.domain ();
      std::cout << "imacpy_ sparse_image\n";

    }

    template <typename I, typename J>
    void imacpy_(rle_image<I, J>& dst,
		 const rle_image<I, J>& src,
		 std::size_t n)
    {
      std::size_t i = n;

      i = n;
      dst.domain ();
      src.domain ();
      std::cout << "imacpy_ rle_image\n";
    }
    
  }
}
