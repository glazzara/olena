// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

// FIXME: Split this file.
// FIXME: Address license-related issues?

#ifndef MILENA_APPS_MESH_SEGM_SKEL_MISC_HH
# define MILENA_APPS_MESH_SEGM_SKEL_MISC_HH

# include <algorithm>  // For std::swap.
# include <utility>    // For std::pair.

# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>

# include <mln/norm/l2.hh>

# include <mln/data/fill.hh>


/** See http://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Other-Builtins.html
    for a definition of __builtin_expect.

    Macros likely and unlikely are for optimization purpose only (the
    developer can use them to help the compiler improve the
    basic-block sewing pass.  */
// FIXME: We should turn `likely' and `unlikely' into functions.
#ifndef likely
#  if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#    define likely(x) (x)
#    define unlikely(x) (x)
#  else
#    define likely(x)   (__builtin_expect((x), 1))
#    define unlikely(x) (__builtin_expect((x), 0))
#  endif
#endif

namespace mln
{

  namespace algebra
  {

    // From Trimesh.
    // FIXME: Doc (input, return value, etc.).
    // FIXME: Idea: Add reference to Numerical Recipes.

    /** \brief Perform LDL^T decomposition of a symmetric positive
	definite matrix.  Like Cholesky, but no square roots.
	Overwrites lower triangle of matrix.

	From Trimesh's ldltdc routine.  */
    template <unsigned N, typename T>
    inline
    bool
    ldlt_decomp(mat<N, N, T>& A, vec<N, T>& rdiag)
    {
      vec<N - 1, T> v;
      for (unsigned i = 0; i < N; ++i)
	{
	  for (unsigned k = 0; k < i; ++k)
	    v[k] = A(i, k) * rdiag[k];
	  for (unsigned j = i; j < N; ++j)
	    {
	      T sum = A(i, j);
	      for (unsigned k = 0; k < i; k++)
		sum -= v[k] * A(j, k);
	      if (i == j)
		{
		  if (unlikely(sum <= T(0)))
		    return false;
		  rdiag[i] = T(1) / sum;
		}
	      else
		A(j, i) = sum;
	    }
	}
      return true;
    }

    // From Trimesh.
    // FIXME: Doc (input, return value, etc.).
    // FIXME: Idea: Add reference to Numerical Recipes.

    /// \brief Solve A x = B after mln::algebra::ldlt_decomp.
    template <unsigned N, typename T>
    inline
    void
    ldlt_solve(const mat<N, N, T>& A, const vec<N, T>& rdiag,
	       const vec<N, T>& B, vec<N, T>& x)
    {
      for (unsigned i = 0; i < N; ++i)
	{
	  T sum = B[i];
	  for (unsigned k = 0; k < i; ++k)
	    sum -= A(i, k) * x[k];
	  x[i] = sum * rdiag[i];
	}
      for (int i = N - 1; i >= 0; --i)
	{
	  T sum = 0;
	  for (unsigned k = i + 1; k < N; ++k)
	    sum += A(k, i) * x[k];
	  x[i] -= sum * rdiag[i];
	}
    }

  } // end of namespace mln::algebra

} // end of namespace mln


// FIXME: To be moved elsewhere (in mln/geom)?

// Inspired from Trimesh.

namespace mln
{

  namespace geom
  {

    // FIXME: More doc (see Trimesh's).

    /* FIXME: We should restrict attached data to vertices in return
       value.  */
    /** \brief Compute normals at vertices.

	Inspired from the method Trimesh::need_normals of the Trimesh
	library.
	\see http://www.cs.princeton.edu/gfx/proj/trimesh2/

	For simplicity purpose, and contrary to Trimesh, this routine
	only compute normals from a mesh, not from a cloud of
	points.  */
    inline
    complex_image< 2, mln::space_2complex_geometry, algebra::vec<3, float> >
    mesh_normal(const p_complex<2, space_2complex_geometry>& mesh)
    {
      // Shortcuts.
      static const unsigned D = 2;
      typedef space_2complex_geometry G;
      typedef algebra::vec<3, float> vec3f;
      typedef complex_image< D, G, vec3f > normal_t;

      normal_t normal(mesh);
      data::fill(normal, literal::zero);

      mln::p_n_faces_fwd_piter<D, G> f(mesh, 2);
      // A neighborhood where neighbors are the set of 0-faces
      // transitively adjacent to the reference point.
      typedef mln::complex_m_face_neighborhood<D, G> adj_vertices_nbh_t;
      adj_vertices_nbh_t adj_vertices_nbh;
      mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, f);
      /* FIXME: We should be able to pas this value (m) either at the
	 construction of the neighborhood or at the construction of
	 the iterator.  Alas, we can't inherit ctors (yet), so we
	 can't rely on a simple mixin approach.  */
      adj_v.iter().set_m(0);

      // Iterate on 2-faces (triangles).
      for_all(f)
      {
	/// Psites of 0-faces transitively adjacent to F.
	std::vector<mln_psite_(normal_t)> p;
	p.reserve(3);
	for_all(adj_v)
	  p.push_back(adj_v);
	/// The should be exactly three vertices adjacent to F.
	mln_assertion(p.size() == 3);

	/* FIXME: Is the explicit conversion to_site() really needed?
	   If not, we could provide a local shortcut like

	     vec(p[0])   // Instead of p[0].to_site().front().to_vec()

	   to shorten these lines.  */
	// FIXME: Factor as well?
	vec3f a =
	  p[0].to_site().front().to_vec() - p[1].to_site().front().to_vec();
	vec3f b =
	  p[1].to_site().front().to_vec() - p[2].to_site().front().to_vec();
	vec3f c =
	  p[2].to_site().front().to_vec() - p[0].to_site().front().to_vec();

	// FIXME: Factor as well?
	float l2a = norm::sqr_l2(a);
	float l2b = norm::sqr_l2(b);
	float l2c = norm::sqr_l2(c);
	vec3f face_normal = algebra::vprod(a, b);

	normal(p[0]) += face_normal * (1.0f / (l2a * l2c));
	normal(p[1]) += face_normal * (1.0f / (l2b * l2a));
	normal(p[2]) += face_normal * (1.0f / (l2c * l2b));
      }

      // Normalize normals.  We don't need to iterate on all faces, just
      // 0-faces.
      mln::p_n_faces_fwd_piter<D, G> v(mesh, 0);
      for_all(v)
	normal(v).normalize();

      return normal;
    }


    /* FIXME: We should restrict attached data to vertices in return
       value.  */
    /** \brief Compute the area ``belonging'' to normals at vertices.

	Inspired from the method Trimesh::need_pointareas of the
	Trimesh library.
	\see http://www.cs.princeton.edu/gfx/proj/trimesh2/

	From the documentation of Trimesh:

	  ``Compute the area "belonging" to each vertex or each corner
	  of a triangle (defined as Voronoi area restricted to the
	  1-ring of a vertex, or to the triangle).''
    */
    inline
    std::pair<
      complex_image< 2, mln::space_2complex_geometry, algebra::vec<3, float> >,
      complex_image< 2, mln::space_2complex_geometry, float >
      >
    mesh_corner_point_area(const p_complex<2, space_2complex_geometry>& mesh)
    {
      // Shortcuts.
      static const unsigned D = 2;
      typedef space_2complex_geometry G;
      typedef algebra::vec<3, float> vec3f;

      // Hold data for 2-faces only.
      typedef complex_image< D, G, vec3f > corner_area_t;
      // Hold data for 0-faces only.
      typedef complex_image< D, G, float > point_area_t;
      // Packed output.
      typedef std::pair<corner_area_t, point_area_t> output_t;

      // Initialize output.
      output_t output(mesh, mesh);
      corner_area_t& corner_area = output.first;
      point_area_t& point_area = output.second;
      data::fill(corner_area, literal::zero);
      data::fill(point_area, literal::zero);

      mln::p_n_faces_fwd_piter<D, G> f(mesh, 2);
      // A neighborhood where neighbors are the set of 0-faces
      // transitively adjacent to the reference point.
      typedef mln::complex_m_face_neighborhood<D, G> adj_vertices_nbh_t;
      adj_vertices_nbh_t adj_vertices_nbh;
      mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, f);
      /* FIXME: We should be able to pas this value (m) either at the
	 construction of the neighborhood or at the construction of
	 the iterator.  Alas, we can't inherit ctors (yet), so we
	 can't rely on a simple mixin approach.  */
      adj_v.iter().set_m(0);

      for_all(f)
      {
	/// Psites of 0-faces transitively adjacent to F.
	std::vector<mln_psite_(corner_area_t)> p;
	p.reserve(3);
	for_all(adj_v)
	  p.push_back(adj_v);
	/// The should be exactly three vertices adjacent to F.
	mln_assertion(p.size() == 3);

	// (Opposite) edge vectors.
	algebra::vec<3, vec3f> e;
	// FIXME: See above.
	e.set
	  (p[2].to_site().front().to_vec() - p[1].to_site().front().to_vec(),
	   p[0].to_site().front().to_vec() - p[2].to_site().front().to_vec(),
	   p[1].to_site().front().to_vec() - p[0].to_site().front().to_vec());

	// Compute corner weights.
	float area = 0.5f * norm::l2(algebra::vprod(e[0], e[1]));
	vec3f sqr_norm;
	sqr_norm.set(norm::sqr_l2(e[0]),
		     norm::sqr_l2(e[1]),
		     norm::sqr_l2(e[2]));
	vec3f edge_weight;
	edge_weight.set
	  (sqr_norm[0] * (sqr_norm[1] + sqr_norm[2] - sqr_norm[0]),
	   sqr_norm[1] * (sqr_norm[2] + sqr_norm[0] - sqr_norm[1]),
	   sqr_norm[2] * (sqr_norm[0] + sqr_norm[1] - sqr_norm[2]));

	if (edge_weight[0] <= 0.0f)
	  {
	    corner_area(f)[1] = -0.25f * sqr_norm[2] * area / (e[0] * e[2]);
	    corner_area(f)[2] = -0.25f * sqr_norm[1] * area / (e[0] * e[1]);
	    corner_area(f)[0] = area - corner_area(f)[1] - corner_area(f)[2];
	  }
	else if (edge_weight[1] <= 0.0f)
	  {
	    corner_area(f)[2] = -0.25f * sqr_norm[0] * area / (e[1] * e[0]);
	    corner_area(f)[0] = -0.25f * sqr_norm[2] * area / (e[1] * e[2]);
	    corner_area(f)[1] = area - corner_area(f)[2] - corner_area(f)[0];
	  }
	else if (edge_weight[2] <= 0.0f)
	  {
	    corner_area(f)[0] = -0.25f * sqr_norm[1] * area / (e[2] * e[1]);
	    corner_area(f)[1] = -0.25f * sqr_norm[0] * area / (e[2] * e[0]);
	    corner_area(f)[2] = area - corner_area(f)[0] - corner_area(f)[1];
	  }
	else
	  {
	    float ewscale =
	      0.5f * area / (edge_weight[0] + edge_weight[1] + edge_weight[2]);
	    for (int i = 0; i < 3; ++i)
	      corner_area(f)[i] =
		ewscale * (edge_weight[(i+1)%3] + edge_weight[(i+2)%3]);
	  }

	for (int i = 0; i < 3; ++i)
	  point_area(p[i]) += corner_area(f)[i];
      }

      return output;
    }


    namespace internal
    {

      /** \brief \a i + 1 and \a i - 1 modulo 3

	  Defined as macros in Trimesh.

	    #define NEXT(i) ((i)<2 ? (i)+1 : (i)-2)
	    #define PREV(i) ((i)>0 ? (i)-1 : (i)+2)

	  According to Trimesh doc,

	    ``This way of computing it tends to be faster than using %''

	  but I (Roland) just can't believe it...  We should profile
	  some code first.

	  \{ */
      static inline unsigned next(unsigned i) { return (i + 1) % 3; }
      static inline unsigned prev(unsigned i) { return (i - 1) % 3; }
      /** \} */


      /// Rotate a coordinate system to be perpendicular to the given normal
      inline
      void
      rot_coord_sys(const algebra::vec<3, float> &old_u,
		    const algebra::vec<3, float> &old_v,
		    const algebra::vec<3, float> &new_norm,
		    algebra::vec<3, float> &new_u,
		    algebra::vec<3, float> &new_v)
      {
	new_u = old_u;
	new_v = old_v;
	algebra::vec<3, float> old_norm = vprod(old_u, old_v);
	float ndot = old_norm * new_norm;
	if (unlikely(ndot <= -1.0f))
	  {
	    new_u = -new_u;
	    new_v = -new_v;
	    return;
	  }
	algebra::vec<3, float> perp_old = new_norm - ndot * old_norm;
	algebra::vec<3, float> dperp =
	  1.0f / (1 + ndot) * (old_norm + new_norm);
	new_u -= dperp * (new_u * perp_old);
	new_v -= dperp * (new_v * perp_old);
      }


      // FIXME: Add const to formals whenever we can.

      // Reproject a curvature tensor from the basis spanned by old_u and old_v
      // (which are assumed to be unit-length and perpendicular) to the
      // new_u, new_v basis.
      inline
      void
      proj_curv(const algebra::vec<3, float>& old_u,
		const algebra::vec<3, float>& old_v,
		float old_ku, float old_kuv, float old_kv,
		const algebra::vec<3, float>& new_u,
		const algebra::vec<3, float>& new_v,
		float& new_ku, float& new_kuv, float& new_kv)
      {
	algebra::vec<3, float> r_new_u, r_new_v;
	rot_coord_sys(new_u, new_v, vprod(old_u, old_v), r_new_u, r_new_v);

	float u1 = r_new_u * old_u;
	float v1 = r_new_u * old_v;
	float u2 = r_new_v * old_u;
	float v2 = r_new_v * old_v;
	new_ku  = old_ku * u1*u1 + old_kuv * (2.0f  * u1*v1) + old_kv * v1*v1;
	new_kuv = old_ku * u1*u2 + old_kuv * (u1*v2 + u2*v1) + old_kv * v1*v2;
	new_kv  = old_ku * u2*u2 + old_kuv * (2.0f  * u2*v2) + old_kv * v2*v2;
      }

      /** Given a curvature tensor, find principal directions and
	  curvatures.  Makes sure that pdir1 and pdir2 are
	  perpendicular to normal.  */
      inline
      void
      diagonalize_curv(const algebra::vec<3, float>& old_u,
		       const algebra::vec<3, float>& old_v,
		       float ku, float kuv, float kv,
		       const algebra::vec<3, float>& new_norm,
		       algebra::vec<3, float>& pdir1,
		       algebra::vec<3, float>& pdir2,
		       float& k1, float& k2)
      {
	algebra::vec<3, float> r_old_u, r_old_v;
	rot_coord_sys(old_u, old_v, new_norm, r_old_u, r_old_v);

	float c = 1, s = 0, tt = 0;
	if (likely(kuv != 0.0f))
	  {
	    // Jacobi rotation to diagonalize.
	    float h = 0.5f * (kv - ku) / kuv;
	    tt = (h < 0.0f) ?
	      1.0f / (h - sqrt(1.0f + h*h)) :
	      1.0f / (h + sqrt(1.0f + h*h));
	    c = 1.0f / sqrt(1.0f + tt*tt);
	    s = tt * c;
	  }

	k1 = ku - tt * kuv;
	k2 = kv + tt * kuv;

	if (fabs(k1) >= fabs(k2))
	  pdir1 = c*r_old_u - s*r_old_v;
	else
	  {
	    std::swap(k1, k2);
	    pdir1 = s*r_old_u + c*r_old_v;
	  }
	pdir2 = vprod(new_norm, pdir1);
      }

    } // end of namespace mln::geom::internal


    /** \brief Compute the principal curvatures of a surface at
	vertices.

	These princpal curvatures are names kappa_1 and kappa_2 in

	  Sylvie Philipp-Foliguet, Michel Jordan Laurent Najman and
	  Jean Cousty.  Artwork 3D Model Database Indexing and
	  Classification.

	\param[in] mesh The surface (triangle mesh) on which the
	curvature is to be computed.  */
    /* FIXME: We should restrict attached data to vertices in return
       value.  */
    /* FIXME: Have a typedef for this return type?  */
    inline
    std::pair<
      complex_image< 2, mln::space_2complex_geometry, float >,
      complex_image< 2, mln::space_2complex_geometry, float >
      >
    mesh_curvature(const p_complex<2, space_2complex_geometry>& mesh)
    {
      // Shortcuts.
      static const unsigned D = 2;
      typedef space_2complex_geometry G;
      typedef algebra::vec<3, float> vec3f;
      typedef algebra::mat<3, 3, float> mat3f;

      typedef complex_image< D, G, vec3f > corner_area_t;
      typedef complex_image< D, G, float > point_area_t;

      // Normals at vertices.
      typedef complex_image< 2, mln::space_2complex_geometry, vec3f > normal_t;
      normal_t normal = mesh_normal(mesh);

      // Areas ``belonging'' to a normal.
      typedef complex_image< D, G, vec3f > corner_area_t;
      typedef complex_image< D, G, float > point_area_t;
      typedef std::pair<corner_area_t, point_area_t> corner_point_area_t;
      corner_point_area_t corner_point_area = mesh_corner_point_area(mesh);
      // Shortcuts.
      corner_area_t& corner_area = corner_point_area.first;
      point_area_t& point_area = corner_point_area.second;

      // Curvatures at each vertex.
      typedef complex_image< 2, mln::space_2complex_geometry, float > curv_t;
      typedef std::pair<curv_t, curv_t> output_t;
      output_t output(mesh, mesh);
      curv_t& curv1 = output.first;
      curv_t& curv2 = output.second;
      // ??
      complex_image< 2, mln::space_2complex_geometry, float > curv12(mesh);
      // Principal directions at each vertex.
      complex_image< 2, mln::space_2complex_geometry, vec3f > pdir1(mesh);
      complex_image< 2, mln::space_2complex_geometry, vec3f > pdir2(mesh);

      /*-------------------------------------------------.
      | Set up an initial coordinate system per vertex.  |
      `-------------------------------------------------*/

      mln::p_n_faces_fwd_piter<D, G> f(mesh, 2);
      // A neighborhood where neighbors are the set of 0-faces
      // transitively adjacent to the reference point.
      typedef mln::complex_m_face_neighborhood<D, G> adj_vertices_nbh_t;
      adj_vertices_nbh_t adj_vertices_nbh;
      mln_niter_(adj_vertices_nbh_t) adj_v(adj_vertices_nbh, f);
      /* FIXME: We should be able to pas this value (m) either at the
	 construction of the neighborhood or at the construction of
	 the iterator.  Alas, we can't inherit ctors (yet), so we
	 can't rely on a simple mixin approach.  */
      adj_v.iter().set_m(0);

      for_all(f)
      {
	/// Psites of 0-faces transitively adjacent to F.
	std::vector<mln_psite_(curv_t)> p;
	p.reserve(3);
	for_all(adj_v)
	  p.push_back(adj_v);
	/// The should be exactly three vertices adjacent to F.
	mln_assertion(p.size() == 3);

	// FIXME: See above.
	pdir1(p[0]) =
	  p[1].to_site().front().to_vec() - p[0].to_site().front().to_vec();
	pdir1(p[1]) =
	  p[2].to_site().front().to_vec() - p[1].to_site().front().to_vec();
	pdir1(p[2]) =
	  p[0].to_site().front().to_vec() - p[2].to_site().front().to_vec();
      }

      mln::p_n_faces_fwd_piter<D, G> v(mesh, 0);
      for_all(v)
      {
	pdir1(v) = algebra::vprod(pdir1(v), normal(v));
	pdir1(v).normalize();
	pdir2(v) = algebra::vprod(normal(v), pdir1(v));
      }

      /*-----------------------------.
      | Compute curvature per-face.  |
      `-----------------------------*/

      for_all(f)
      {
	std::vector<mln_psite_(curv_t)> p;
	p.reserve(3);
	for_all(adj_v)
	  p.push_back(adj_v);
	mln_assertion(p.size() == 3);

	// (Opposite) edge vectors.
	algebra::vec<3, vec3f> e;
	// FIXME: See above.
	e.set
	  (p[2].to_site().front().to_vec() - p[1].to_site().front().to_vec(),
	   p[0].to_site().front().to_vec() - p[2].to_site().front().to_vec(),
	   p[1].to_site().front().to_vec() - p[0].to_site().front().to_vec());

	// N-T-B coordinate system per face.
	vec3f t = e[0];
	t.normalize();
	vec3f n = algebra::vprod(e[0], e[1]);
	/* FIXME: Why don't we normalize N?  Is it a normal vector by
	   construction?  Or maybe we don't need it to be normal?  */
	vec3f b = algebra::vprod(n, t);
	b.normalize();

	// Estimate curvature based on variation of normals along edges.
	vec3f m = literal::zero;
	mat3f w = literal::zero;
	for (int j = 0; j < 3; ++j)
	  {
	    float u = e[j] * t;
	    float v = e[j] * b;
	    w(0, 0) += u * u;
	    w(0, 1) += u * v;
	    /* FIXME: Those two lines were commented in Trimesh's
	       original code.

	         //w(1, 1) += v*v + u*u;
	         //w(1, 2) += u*v;

	    */
	    w(2, 2) += v * v; 
	    vec3f dn =
	      normal(p[internal::prev(j)]) - normal(p[internal::next(j)]);
	    float dnu = dn * t;
	    float dnv = dn * b;
	    m[0] += dnu*u;
	    m[1] += dnu*v + dnv*u;
	    m[2] += dnv*v;
	  }
	  w(1, 1) = w(0, 0) + w(2, 2);
	  w(1, 2) = w(0, 1);

	  // Least squares solution.
	  vec3f diag;
#ifndef NDEBUG
	  bool ldlt_decomp_sucess_p = algebra::ldlt_decomp(w, diag);
#endif // ! NDEBUG
	  mln_assertion(ldlt_decomp_sucess_p);
	  algebra::ldlt_solve(w, diag, m, m);

	  // Push it back out to the vertices
	  for (int j = 0; j < 3; ++j)
	    {
	      float c1, c12, c2;
	      internal::proj_curv(t, b, m[0], m[1], m[2],
				  pdir1(p[j]), pdir2(p[j]), c1, c12, c2);
	      float wt = corner_area(f)[j] / point_area(p[j]);
	      curv1(p[j])  += wt * c1;
	      curv12(p[j]) += wt * c12;
	      curv2(p[j])  += wt * c2;
	    }
      }

      /*-------------------------------------------------------------.
      | Compute principal directions and curvatures at each vertex.  |
      `-------------------------------------------------------------*/

      for_all(v)
	internal::diagonalize_curv(pdir1(v), pdir2(v),
				   curv1(v), curv12(v), curv2(v),
				   normal(v), pdir1(v), pdir2(v),
				   curv1(v), curv2(v));

      return output;
    }

  } // end of namespace mln::geom

} // end of namespace mln

#endif // ! MILENA_APPS_MESH_SEGM_SKEL_MISC_HH
