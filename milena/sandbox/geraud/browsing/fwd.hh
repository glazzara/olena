

namespace mln
{

  namespace canvas
  {

    namespace browsing
    {

      template <typename F>
      void fwd(F& f)
      {
 	trace::entering("canvas::browsing::fwd");
	mln_precondition(f.input.has_data());
	typedef typename F::I I;
	mln_fwd_piter(I) p(f.input.domain());
	f.init();
	for_all(p)
	  {
	    f.p = p;
	    f.next();
	  }
	f.final();
 	trace::exiting("canvas::browsing::fwd");
      }


      template <typename F>
      void row_fwd(F& f)
      {
 	trace::entering("canvas::browsing::row_fwd");
	mln_precondition(f.input.has_data());
	box2d b = f.input.domain();
	unsigned nrows = b.nrows(), ncols = b.ncols();
	def::coord& row = f.p.row();
	def::coord& col = f.p.col();

	f.init();
	for (row = 0; row < nrows; ++row)
	  {
	    col = 0;
	    f.begin();
	    while (col < ncols)
	      {
		f.next();
		++col;
	      }
	    f.end();
	  }
	f.final();
 	trace::exiting("canvas::browsing::row_fwd");
      }


      template <typename F>
      void col_fwd(F& f)
      {
 	trace::entering("canvas::browsing::col_fwd");
	mln_precondition(f.input.has_data());
	box2d b = f.input.domain();
	unsigned nrows = b.nrows(), ncols = b.ncols();
	def::coord& row = f.p.row();
	def::coord& col = f.p.col();

	f.init();
	for (col = 0; col < ncols; ++col)
	  {
	    row = 0;
	    f.begin();
	    while (row < nrows)
	      {
		f.next();
		++row;
	      }
	    f.end(); // FIXME: p invalid...
	  }
	f.final();
 	trace::exiting("canvas::browsing::col_fwd");
      }


    } // end of namespace mln::canvas::browsing

  } // end of namespace mln::canvas

} // end of namespace mln
