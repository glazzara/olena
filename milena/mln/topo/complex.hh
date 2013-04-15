// Copyright (C) 2008, 2009, 2010, 2012 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TOPO_COMPLEX_HH
# define MLN_TOPO_COMPLEX_HH

/// \file
///
/// \brief Structures for general complexes.
///
/// A complex defines a topological space which can be used as a
/// support for an image (i.e., as site sets).

# include <cstddef>

# include <iosfwd>

# include <mln/metal/bool.hh>

# include <mln/util/tracked_ptr.hh>

# include <mln/topo/face_data.hh>
# include <mln/topo/algebraic_face.hh>
# include <mln/topo/algebraic_n_face.hh>
# include <mln/topo/n_faces_set.hh>

# include <mln/topo/complex_iterators.hh>


namespace mln
{

  namespace topo
  {

    // Forward declarations (external).
    template <unsigned N, unsigned D> class n_faces_set;
    template <unsigned D> class face_fwd_iter;
    template <unsigned D> class face_bkd_iter;
// FIXME: Disabled (moved to the attic).
# if 0
    template <unsigned N, unsigned D> class faces_fwd_iter_;
    template <unsigned N, unsigned D> class faces_bkd_iter_;
#endif

    // Forward declarations (internal).
    namespace internal
    {
      template <unsigned D>
      struct complex_data;

      template <unsigned N, unsigned D>
      struct faces_set_mixin;
    }


    /*----------.
    | Complex.  |
    `----------*/

    /*!
      \brief General complex of dimension \p D.
    */
    template <unsigned D>
    class complex
    {
    public:
      /// Forward mln::Iterator type iterating on all faces.
      typedef face_fwd_iter<D> fwd_citer;
      /// Backward mln::Iterator type iterating on all faces.
      typedef face_bkd_iter<D> bkd_citer;

// FIXME: Disabled (moved to the attic).
# if 0
      /// Forward mln::Iterator type iterating on \p N-faces.
      template <unsigned N>
      struct fwd_fiter { typedef faces_fwd_iter_<N, D> ret; };
      /// Backward mln::Iterator type iterating on \p N-faces.
      template <unsigned N>
      struct bkd_fiter { typedef faces_bkd_iter_<N, D> ret; };
#endif

      /// Complex construction.
      /// \{
      /// Create a new \p D-complex.
      complex();

      /// Add a 0-face to the complex.
      n_face<0, D> add_face();

      /// Add a \p (N+1)-face to the complex (with \p N >= 0).
      ///
      /// \param adjacent_faces The (\p N-1)-faces adjacent to the new
      /// \p N-face.
      template <unsigned N>
      n_face<N + 1, D> add_face(const n_faces_set<N, D>& adjacent_faces);
      /// \}

      /// Static manipulators.
      ///
      /// These methods use statically-known input.
      /// \{
      /// Return the total number of faces, whatever their
      /// dimension.
      unsigned nfaces() const;

      /// Return the number of \p N-faces.
      template <unsigned N>
      unsigned nfaces_of_static_dim() const;
      /// \}

      /// Dynamic manipulators.
      ///
      /// These methods use input know as run time.
      /// \{
      /// Return the number of \a n-faces.
      ///
      /// Warning, this function has a complexity linear in term of N,
      /// since each n_faces_set is checked (the present implementation
      /// does not provide a direct access to n_faces_set through a
      /// dynamic value of the dimension).
      unsigned nfaces_of_dim(unsigned n) const;
      /// \}

      /// Pretty-printing.
      /// \{
      /// Print the complex.
      void print(std::ostream& ostr) const;
      /// Print the faces of dimension \p N.
      template <unsigned N>
      void print_faces(std::ostream& ostr) const;
      /// \}

      /// Get the address of the data of this complex.
      ///
      /// This address is a concise and useful information to print
      /// and track the actual content of this complex.
      const void* addr() const;

    private:
      /// The actual data of the complex.
      util::tracked_ptr< internal::complex_data<D> > data_;

      template <unsigned D_>
      friend bool operator==(const complex<D_>& lhs, const complex<D_>& rhs);

      /// Accessors.
      /// \{
      template <unsigned N, unsigned D_> friend class n_face;
      template <unsigned D_> friend class face;

      template <unsigned N>
      face_data<N, D>& face_data_(unsigned face_id);

      template <unsigned N>
      const face_data<N, D>& face_data_(unsigned face_id) const;
      /// \}

      /// Functional meta-manipulators.
      /// \{
      /* FIXME: Use something more constraining than the STL's
	 UnaryFunction/BinaryFunction.  Use Function or Function_v2v?
	 Or a new subclass of Function?  */

      /* FIXME: Replace F and ACCU by a Milena accumulator?  */

      /** Apply a kind of static fold left operator to the
	  implicit list of n_faces_set using a functor \a f and a value \a
	  accu.

	  Argument \a is called an "accumulator", but with a slightly
	  different meaning than mln:accu members.

	  We might want to use TypeLists or something similar, is they
	  provide an explicit static fold left operator.  */
      template <typename BinaryFunction, typename T>
      T fold_left_(const BinaryFunction& f, const T& accu) const;

      /// Apply a functor \a f to this list of \a n-faces.
      template <typename UnaryFunction>
      typename UnaryFunction::result_type
      apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
      /// \}

      /// Connect two algebraic faces.
      ///
      /// \param f1 An algebraic face of dimension \p N
      /// \param f2 A face of dimension \p N + 1
      ///
      /// \pre \p N must be lower or equal to \p D.
      template <unsigned N>
      void connect_(const algebraic_n_face<N, D>& f1,
		    const n_face<N + 1, D>& f2);
    };


    /// Compare two complexes for equality.
    template <unsigned D>
    bool
    operator==(const complex<D>& lhs, const complex<D>& rhs);


    /// Pretty print a complex.
    template <unsigned D>
    std::ostream&
    operator<<(std::ostream& ostr, const complex<D>& c);


    /*---------------.
    | Complex data.  |
    `---------------*/

    /** Complex data.

	Data is aggregated as follows in an mln::topo::complex<D>:

        \verbatim

                   ,-----------> higher_dim_faces_set_mixin<0, D>
                   |
        faces_set_mixin<0, D>
                 ^
                 |
                 | ,-----------> higher_dim_faces_set_mixin<1, D>
                 | | ,--------->  lower_dim_faces_set_mixin<1, D>
                 | | |
        faces_set_mixin<1, D>
                 ^
                 |
                 |

                ...

                 ^
                 |
                 | ,-----------> higher_dim_faces_set_mixin<D - 1, D>
                 | | ,--------->  lower_dim_faces_set_mixin<D - 1, D>
                 | | |
        faces_set_mixin<D - 1, D>
                 ^
                 |
                 |   ,--------->  lower_dim_faces_set_mixin<D, D>
                 |   |
        faces_set_mixin<D, D>
                 ^
                 |
                 |
           complex_data<D> ----.  (shared data)
                               |
                               |
                ,--------------'
                |
                |
                `----<> tracked_ptr< complex_data<D> > ----<> complex<D>

        \endverbatim


	An instance of mln::topo::internal::faces_set_mixin<N, D>
	stores the \p N-faces of a \p D-complex.

	Classes mln::topo::internal::lower_dim_faces_set_mixin<N, D>
	are implementation classes factoring services related to
	complex data.  */


    /*---------------------.
    | Faces of a complex.  |
    `---------------------*/

    /// The sets of n-faces of a complex are recursively aggregated as
    /// mixins.
    namespace internal
    {

      // Forward declarations.
      template <unsigned N, unsigned D> struct lower_dim_faces_set_mixin;
      template <unsigned N, unsigned D> struct higher_dim_faces_set_mixin;

      // -------------------------------------- //
      // mln::topo::internal::faces_set_mixin.  //
      // -------------------------------------- //

      /// \internal \brief Recursive mixins of set of faces.
      /// \{

      template <unsigned N, unsigned D> struct faces_set_mixin;


      /*!
	\internal
	\brief Faces of intermediate dimension (greater than 0, lower than \p D).
      */
      template <unsigned N, unsigned D>
      struct faces_set_mixin : public faces_set_mixin<N - 1, D>,
			       public lower_dim_faces_set_mixin<N, D>,
			       public higher_dim_faces_set_mixin<N, D>
      {
	std::vector< face_data<N, D> > faces_;

	/// Pretty-printing.
	/// \{
	/// Print the faces of dimension \p N.
	void print(std::ostream& ostr) const;
	/// Recursively print the faces of dimensions 0 to \p N
	/// (in ascending dimension).
	void print_rec_asc(std::ostream& ostr) const;
	/// \}

	/// Functional meta-manipulators.
	/// \{
	/// Fold left.
	/// \see mln::complex<D>::fold_left_.
	template <typename BinaryFunction, typename T>
	T fold_left_(const BinaryFunction& f, const T& accu) const;
	/// Apply a functor \a f to the list of faces if \a n == \p N.
	/// \see mln::complex<D>::apply_if_dim_matches_.
	template <typename UnaryFunction>
	typename UnaryFunction::result_type
	apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
	/// \}
      };

      /*!
	\internal
	\brief Faces of highest dimension (\p D).
      */
      template <unsigned D>
      struct faces_set_mixin<D, D> : public faces_set_mixin<D - 1, D>,
				     public lower_dim_faces_set_mixin<D, D>
      {
	std::vector< face_data<D, D> > faces_;

	/// Pretty-printing.
	/// \{
	/// Print the faces of dimension \p D.
	void print(std::ostream& ostr) const;
	void print_rec_asc(std::ostream& ostr) const;
	/// \}

	/// Functional meta-manipulators.
	/// \{
	/// Fold left.
	/// \see mln::complex<D>::fold_left_.
	template <typename BinaryFunction, typename T>
	T fold_left_(const BinaryFunction& f, const T& accu) const;
	/// Apply a functor \a f to the list of faces if \a n == \p D.
	/// \see mln::complex<D>::apply_if_dim_matches_.
	template <typename UnaryFunction>
	typename UnaryFunction::result_type
	apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
	/// \}
      };

      /*!
	\internal
	\brief Faces of lowest dimension (0).
      */
      template <unsigned D>
      struct faces_set_mixin<0, D> : public higher_dim_faces_set_mixin<0, D>
      {
	std::vector< face_data<0, D> > faces_;

	/// Pretty-printing.
	/// \{
	/// Print the faces of dimension 0.
	void print(std::ostream& ostr) const;
	void print_rec_asc(std::ostream& ostr) const;
	/// \}

	/// Functional meta-manipulators.
	/// \{
	/// Fold left.
	/// \see mln::complex<D>::fold_left_.
	template <typename BinaryFunction, typename T>
	T fold_left_(const BinaryFunction& f, const T& accu) const;
	/// Apply a functor \a f to the list of faces if \a n == 0.
	/// \see mln::complex<D>::apply_if_dim_matches_.
	template <typename UnaryFunction>
	typename UnaryFunction::result_type
	apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
	/// \}
      };

      /*!
	\internal
	\brief Faces of a 0-complex.
      */
      template <>
      struct faces_set_mixin<0, 0>
      {
	std::vector< face_data<0, 0> > faces_;

	/// Pretty-printing.
	/// \{
	/// Print the faces of dimension \p D.
	void print(std::ostream& ostr) const;
	void print_rec_asc(std::ostream& ostr) const;
	/// \}

	/// Functional meta-manipulators.
	/// \{
	/// Fold left.
	/// \see mln::complex<D>::fold_left_.
	template <typename BinaryFunction, typename T>
	T fold_left_(const BinaryFunction& f, const T& accu) const;
	/// Apply a functor \a f to the list of faces if \a n == 0.
	/// \see mln::complex<D>::apply_if_dim_matches_.
	template <typename UnaryFunction>
	typename UnaryFunction::result_type
	apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const;
	/// \}
      };
      /// \}


      // Tech note: g++-2.95 highly prefers to have the complex_data
      // class to be defined after the specializations of
      // 'faces_set_mixin'.

      /*!
	\internal
	\brief Complex data.
      */
      template <unsigned D>
      struct complex_data : public faces_set_mixin<D, D>
      {
	// Data is contained in super classes.
      };


      // ------------------------------------------------- //
      // mln::topo::internal::lower_dim_faces_set_mixin.   //
      // mln::topo::internal::higher_dim_faces_set_mixin.  //
      // ------------------------------------------------- //

      /// \internal \brief Mixins of mixin mln::faces_set_mixin.
      /// \{
      template <unsigned N, unsigned D>
      struct lower_dim_faces_set_mixin
      {
	void print(std::ostream& ostr, const face_data<N, D>& f) const;
      };

      template <unsigned N, unsigned D>
      struct higher_dim_faces_set_mixin
      {
	void print(std::ostream& ostr, const face_data<N, D>& f) const;
      };
      /// \}

    } // end of namespace mln::topo::internal



# ifndef MLN_INCLUDE_ONLY

    /*-----------------------.
    | Complex construction.  |
    `-----------------------*/

    template <unsigned D>
    inline
    complex<D>::complex()
      // Allocate data for this complex.
      : data_(new internal::complex_data<D>())
    {
    }

    template <unsigned D>
    inline
    n_face<0, D>
    complex<D>::add_face()
    {
      // Upcast `data_' to get access to the field `faces_' of its
      // base class `internal::faces_set_mixin<0, D>'.
      std::vector< face_data<0, D> >& faces_0 =
	static_cast< internal::faces_set_mixin<0, D>& >(*data_).faces_;
      /* FIXME: This is not thread-proof (these two lines should
	 form an atomic section).  */
      faces_0.push_back(face_data<0, D>());
      unsigned id = nfaces_of_static_dim<0>() - 1;
      return n_face<0, D>(*this, id);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    n_face<N + 1, D>
    complex<D>::add_face(const n_faces_set<N, D>& adjacent_faces)
    {
      typedef typename std::vector< algebraic_n_face<N, D> >::const_iterator
	iter_t;

      // Ensure ADJACENT_FACES are already part of the complex.
      if (!HAS_NDEBUG)
	for (iter_t a = adjacent_faces.faces().begin();
	     a != adjacent_faces.faces().end(); ++a)
	  {
	    mln_precondition(a->cplx() == *this);
	    mln_precondition(a->is_valid());
	  }

      face_data<N + 1, D> f;
      // Upcast `data_' to get access to the field `faces_' of its
      // base class `internal::faces_set_mixin<N + 1, D>'.
      std::vector< face_data<N + 1, D> >& faces_n_plus_1 =
	static_cast< internal::faces_set_mixin<N + 1, D>& >(*data_).faces_;
      /* FIXME: This is not thread-proof (these two lines should
	 form an atomic section).  */
      faces_n_plus_1.push_back(f);
      unsigned id = nfaces_of_static_dim<N + 1>() - 1;

      n_face<N + 1, D> fh(*this, id);
      // Connect F and its ADJACENT_FACES.
      for (iter_t a = adjacent_faces.faces().begin();
	   a != adjacent_faces.faces().end(); ++a)
	/* Connect
	   - algebraic n-face *A,
	   - and an (n+1)-algebraic face based on FH and having the
             sign of *A.  */
	connect_(*a, fh);
      return fh;
    }


    /*-------.
    | Misc.  |
    `-------*/

    namespace internal
    {

      /*!
	\internal
	\brief A binary meta-functor defined by:

	\code
	add_size : x, c -> x + c.size()
	\endcode

	\see mln::complex<D>::nfaces_of_static_dim<N> (static version).
	\see mln::complex<D>::fold_left_.
      */
      struct add_size
      {
	template <typename T, typename Container>
	T operator()(const T& x, const Container& c) const
	{
	  return x + c.size();
	}
      };

      /*!
	\internal
	\brief An unary meta-functor defined by:

	\code
	add_size : c -> c.size()
	\endcode

	\see mln::complex<D>::nfaces_of_dim (dynamic version).
	\see mln::complex<D>::apply_if_dim_matches_.
      */
      struct get_size
      {
	typedef std::size_t result_type;

	template <typename Container>
	typename Container::size_type operator()(const Container& c) const
	{
	  return c.size();
	}
      };

    } // end of namespace mln::topo::internal


    /*----------------------.
    | Static manipulators.  |
    `----------------------*/

    template <unsigned D>
    inline
    unsigned
    complex<D>::nfaces() const
    {
      return fold_left_(internal::add_size(), 0);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    unsigned
    complex<D>::nfaces_of_static_dim() const
    {
      // Upcast `data_' to get access to the field `faces_' of its
      // base class `internal::faces_set_mixin<N, D>'.
      const std::vector< face_data<N, D> >& faces_n =
	static_cast< const internal::faces_set_mixin<N, D>& >(*data_).faces_;
      return faces_n.size();
    }


    /*-----------------------.
    | Dynamic manipulators.  |
    `-----------------------*/

    template <unsigned D>
    inline
    unsigned
    complex<D>::nfaces_of_dim(unsigned n) const
    {
      // Ensure N is compatible with D.
      mln_precondition(n <= D);
      return apply_if_dim_matches_(n, internal::get_size());
    }


    /*-------------------.
    | Internal methods.  |
    `-------------------*/

    template <unsigned D>
    template <unsigned N>
    inline
    face_data<N, D>&
    complex<D>::face_data_(unsigned face_id)
    {
      // Upcast `data_' to get access to the field `faces_' of its
      // base class `internal::faces_set_mixin<N, D>'.
      std::vector< face_data<N, D> >& faces_n =
	static_cast< internal::faces_set_mixin<N, D>& >(*data_).faces_;
      return faces_n[face_id];
    }

    template <unsigned D>
    template <unsigned N>
    inline
    const face_data<N, D>&
    complex<D>::face_data_(unsigned face_id) const
    {
      // Upcast `data_' to get access to the field `faces_' of its
      // base class `internal::faces_set_mixin<N, D>'.
      const std::vector< face_data<N, D> >& faces_n =
	static_cast< const internal::faces_set_mixin<N, D>& >(*data_).faces_;
      return faces_n[face_id];
    }

    template <unsigned D>
    template <unsigned N>
    inline
    void
    complex<D>::connect_(const algebraic_n_face<N, D>& f1,
			 const n_face<N + 1, D>& f2)
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();

      /* Connect
	 - F1, an algebraic n-face,
	 - and AF2, an algebraic (n+1)-face based on F2 and having the
	   sign of F1.  */
      f1.data().connect_higher_dim_face(make_algebraic_n_face(f2, f1.sign()));
      f2.data().connect_lower_dim_face(f1);
    }


    /*-------------.
    | Comparison.  |
    `-------------*/

    template <unsigned D>
    inline
    bool
    operator==(const complex<D>& lhs, const complex<D>& rhs)
    {
      return lhs.data_.ptr_ == rhs.data_.ptr_;
    }


    /*------------------.
    | Pretty-printing.  |
    `------------------*/

    template <unsigned D>
    inline
    std::ostream&
    operator<<(std::ostream& ostr, const complex<D>& c)
    {
      c.print(ostr);
      return ostr;
    }

    template <unsigned D>
    inline
    void
    complex<D>::print(std::ostream& ostr) const
    {
      // Upcast `data_' to get access to the method `print_rec_asc' of
      // its base class `internal::faces_set_mixin<D, D>'.
      static_cast< const internal::faces_set_mixin<D, D>& >(*data_).print_rec_asc(ostr);
    }

    template <unsigned D>
    template <unsigned N>
    inline
    void
    complex<D>::print_faces(std::ostream& ostr) const
    {
      // Ensure N is compatible with D.
      metal::bool_< N <= D >::check();

      // Upcast `data_' to get access to the method `print' of its
      // base class `internal::faces_set_mixin<N, D>'.
      static_cast< const internal::faces_set_mixin<N, D>& >(*data_).print(ostr);
    }

    template <unsigned D>
    inline
    const void*
    complex<D>::addr() const
    {
      return data_.ptr_;
    }


    namespace internal
    {

      template <unsigned N, unsigned D>
      inline
      void
      faces_set_mixin<N, D>::print_rec_asc(std::ostream& ostr) const
      {
	faces_set_mixin<N - 1, D>::print_rec_asc(ostr);
	print(ostr);
      }

      template <unsigned D>
      inline
      void
      faces_set_mixin<0, D>::print_rec_asc(std::ostream& ostr) const
      {
	print(ostr);
      }

      template <unsigned D>
      inline
      void
      faces_set_mixin<D, D>::print_rec_asc(std::ostream& ostr) const
      {
	faces_set_mixin<D - 1, D>::print_rec_asc(ostr);
	print(ostr);
      }

      inline
      void
      faces_set_mixin<0, 0>::print_rec_asc(std::ostream& ostr) const
      {
	print(ostr);
      }


      template <unsigned N, unsigned D>
      inline
      void
      faces_set_mixin<N, D>::print(std::ostream& ostr) const
      {
	ostr << "Faces of dimension " << N
	     << " and their ajacent faces of dimension "
	     << N - 1 << " and "
	     << N + 1 << std::endl;
	for (unsigned f = 0; f < faces_.size(); ++f)
	  {
	    ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	    lower_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	    ostr << "},  dim " << N + 1 << ": { ";
	    higher_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	    ostr << "}" << std::endl;
	  }
      }

      template <unsigned D>
      inline
      void
      faces_set_mixin<0, D>::print(std::ostream& ostr) const
      {
	const unsigned N = 0;
	ostr << "Faces of dimension " << N
	     << " and their ajacent faces of dimension "
	     << N + 1 << std::endl;
	for (unsigned f = 0; f < faces_.size(); ++f)
	  {
	    ostr << "  " << f << ":  dim " << N + 1 << ": { ";
	    higher_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	    ostr << "}" << std::endl;
	  }
      }

      template <unsigned D>
      inline
      void
      faces_set_mixin<D, D>::print(std::ostream& ostr) const
      {
	const unsigned N = D;
	ostr << "Faces of dimension " << N
	     << " and their ajacent faces of dimension "
	     << N - 1 << std::endl;
	for (unsigned f = 0; f < faces_.size(); ++f)
	  {
	    ostr << "  " << f << ":  dim " << N - 1 << ": { ";
	    lower_dim_faces_set_mixin<N, D>::print(ostr, faces_[f]);
	    ostr << "}" << std::endl;
	  }
      }

      inline
      void
      faces_set_mixin<0, 0>::print(std::ostream& ostr) const
      {
	const unsigned N = 0;
	ostr << "Faces of dimension " << N << std::endl;
	for (unsigned f = 0; f < faces_.size(); ++f)
	  ostr << "  " << f << std::endl;
      }


      template <unsigned N, unsigned D>
      inline
      void
      lower_dim_faces_set_mixin<N, D>::print(std::ostream& ostr,
					     const face_data<N, D>& f) const
      {
	for (typename std::vector< algebraic_n_face<N - 1, D> >::const_iterator l =
	       f.lower_dim_faces_.begin(); l != f.lower_dim_faces_.end(); ++l)
	  ostr << l->face_id() << " ";
      }

      template <unsigned N, unsigned D>
      inline
      void
      higher_dim_faces_set_mixin<N, D>::print(std::ostream& ostr,
					      const face_data<N, D>& f) const
      {
	for (typename std::vector< algebraic_n_face<N + 1, D> >::const_iterator h =
	       f.higher_dim_faces_.begin(); h != f.higher_dim_faces_.end(); ++h)
	  ostr << h->face_id() << " ";
      }

    } // end of namespace mln::topo::internal


    /*-------------------------------.
    | Functional meta-manipulators.  |
    `-------------------------------*/

    /* ------------------------------- */
    /* ``Static Fold Left'' Operator.  */
    /* ------------------------------- */

    template <unsigned D>
    template <typename BinaryFunction, typename T>
    inline
    T
    complex<D>::fold_left_(const BinaryFunction& f, const T& accu) const
    {
      // Upcast `data_' to get access to the method `fold_left_' of
      // its base class `internal::faces_set_mixin<D, D>'.
      return static_cast< const internal::faces_set_mixin<D, D>& >(*data_).fold_left_(f, accu);
    }

    namespace internal
    {

      // FIXME: Try to factor.

      template <unsigned D>
      template <typename BinaryFunction, typename T>
      inline
      T
      faces_set_mixin<D, D>::fold_left_(const BinaryFunction& f,
					const T& accu) const
      {
	// Upcast `data_' to get access to the method `fold_left_' of
	// its base class `internal::faces_set_mixin<D - 1, D>'.
	return static_cast< const faces_set_mixin<D - 1, D>& >(*this).fold_left_(f, f(accu, faces_));
      }

      template <unsigned N, unsigned D>
      template <typename BinaryFunction, typename T>
      inline
      T
      faces_set_mixin<N, D>::fold_left_(const BinaryFunction& f,
					const T& accu) const
      {
	// Upcast `data_' to get access to the method `fold_left_' of
	// its base class `internal::faces_set_mixin<N - 1, D>'.
	return static_cast< const faces_set_mixin<N - 1, D>& >(*this).fold_left_(f, f(accu, faces_));
      }

      template <unsigned D>
      template <typename BinaryFunction, typename T>
      inline
      T
      faces_set_mixin<0, D>::fold_left_(const BinaryFunction& f,
					 const T& accu) const
      {
	return f(accu, faces_);
      }

      template <typename BinaryFunction, typename T>
      inline
      T
      faces_set_mixin<0, 0>::fold_left_(const BinaryFunction& f,
					  const T& accu) const
      {
	return f(accu, faces_);
      }

    } // end of namespace mln::topo::internal


    /* ------------------------------------------------ */
    /* ``Static Apply-If-Dimension-Matches'' Operator.  */
    /* ------------------------------------------------ */

    template <unsigned D>
    template <typename UnaryFunction>
    inline
    typename UnaryFunction::result_type
    complex<D>::apply_if_dim_matches_(unsigned n, const UnaryFunction& f) const
    {
      // Ensure N is compatible with D.
      mln_precondition(n <= D);
      // Upcast `data_' to get access to the method `apply_if_dim_matches_' of
      // its base class `internal::faces_set_mixin<D, D>'.
      return static_cast< const internal::faces_set_mixin<D, D>& >(*data_).apply_if_dim_matches_(n, f);
    }

    namespace internal
    {

      // FIXME: Try to factor.

      template <unsigned D>
      template <typename UnaryFunction>
      inline
      typename UnaryFunction::result_type
      faces_set_mixin<D, D>::apply_if_dim_matches_(unsigned n,
						   const UnaryFunction& f) const
      {
	// Ensure N and D are compatible.
	mln_precondition(n <= D);
	return n == D ?
	  f(faces_) :
	  faces_set_mixin<D - 1, D>::apply_if_dim_matches_(n, f);
      }

      template <unsigned N, unsigned D>
      template <typename UnaryFunction>
      inline
      typename UnaryFunction::result_type
      faces_set_mixin<N, D>::apply_if_dim_matches_(unsigned n,
						   const UnaryFunction& f) const
      {
	// Ensure N and D are compatible.
	mln_precondition(n <= D);
	return n == N ?
	  f(faces_) :
	  faces_set_mixin<N - 1, D>::apply_if_dim_matches_(n, f);
      }

      template <unsigned D>
      template <typename UnaryFunction>
      inline
      typename UnaryFunction::result_type
      faces_set_mixin<0, D>::apply_if_dim_matches_(unsigned n,
						    const UnaryFunction& f) const
      {
	// If we reached this method, then N should be 0.
	mln_precondition(n == 0);
	// Prevent ``unused variable'' warnings when NDEBUG is defined.
	(void) n;
	return f(faces_);
      }

      template <typename UnaryFunction>
      inline
      typename UnaryFunction::result_type
      faces_set_mixin<0, 0>::apply_if_dim_matches_(unsigned n,
						     const UnaryFunction& f) const
      {
	// If we reached this method, then N should be 0.
	mln_precondition(n == 0);
	// Prevent ``unused variable'' warnings when NDEBUG is defined.
	(void) n;
	return f(faces_);
      }

    } // end of namespace mln::topo::internal

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::topo

} // end of namespace mln

#endif // ! MLN_TOPO_COMPLEX_HH
