#ifndef FILL_HH
# define FILL_HH

# include <cstring>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/level/memset_.hh>


#include <mln/core/image2d_b.hh>
#include <mln/value/int_u8.hh>
#include <mln/debug/println.hh>


#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>

#include <mln/level/fill.hh>

#include <mln/debug/println_with_border.hh>

namespace mln
{

  namespace level
  {

    template <typename I>
    void fill_opt1(I& ima, const mln_value(I)& value)
    {
      mln_pixter(I) pix(ima);
      mln_pixter(I) s(ima);
      
      pix.start();
      s.start ();
      
      mln_coord(I) min_row = geom::min_row(exact(ima));
      mln_coord(I) min_col = geom::min_col(exact(ima));
      mln_coord(I) max_row = geom::max_row(exact(ima));
      mln_coord(I) max_col = geom::max_col(exact(ima));
    
      mln_coord(I) nb_cols = max_col - min_col;
      mln_coord(I) nb_rows = max_row - min_row;
      
      std::cerr << "min_row : "
		<< min_row
		<< std::endl
		<< "max_row : "
		<< max_row
		<< std::endl
		<< "min_col : "
		<< min_col
		<< std::endl
		<< "max_col : "
		<< max_col
		<< std::endl;
      
      // fill row
      for (std::size_t i = 0; i < nb_cols; ++i)
	{
	  pix.val () = value;
	  pix.next ();
	}
      
      for (std::size_t i = 1; i < nb_rows; ++i)
	{
	  // copy row into current row
	  std::memcpy (&(pix.val ()),
		       &(s.val ()),
		       sizeof(mln_value(I)) * nb_cols);
	  
	  // Jump to next row
	  for (std::size_t j = 0; j < nb_cols; ++j)
	    pix.next ();
	}
      
      std::memcpy (&(pix.val ()),
		   &(s.val ()),
		   sizeof(mln_value(I)) * nb_cols);
    }
    
    template <typename I>
    void fill_opt2(I& ima, const mln_value(I)& value)
    {
      mln_value(I)* ptr = & ima[0];
      
      mln_pixter(I) pix(ima);
      mln_pixter(I) s(ima);
      point2d min = ima.bbox ().pmin ();
      point2d u = min;
      point2d v = min + down;
      std::size_t k = ima.offset (down);
      std::size_t max = ima.ncells() / k;
      
      for (std::size_t i = 0; i < k; ++i)
	*ptr++ = value;
      
      ptr = & ima[0];
      
      for (std::size_t i = 0;
	   i < max - 1;
	   ++i)
	{
	  u = u + down;
	  std::memcpy (&ima[ima.offset (u - min)],
		       ptr,
		       sizeof (mln_value(I)) * k);
	}
    }

    template <typename I, typename J>
    void fill(sparse_image<I, J> & ima, const mln_value(J)& value)
    {
      mln::internal::run_pset_<I> my_set;

      for (std::size_t i = 0; i < 0; ++i)
	{

	}
      imaset_(ima, value, 42);
    }

    template <typename I, typename J>
    void fill(rle_image<I, J> & ima, const mln_value(J)& value)
    {
      imaset_(ima, value, 42);
    }
  }
}
#endif
