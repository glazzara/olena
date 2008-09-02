
#include <stdio.h>
#include "Trimesh/lmsmooth.h"
#include "Trimesh/TriMesh_io.h"
#include "Trimesh/TriMesh_tstrips.h"
#include "Trimesh/TriMesh.h"
#include "Trimesh/TriMesh_algo.h"
#include "Trimesh/lineqn.h"
#include "Trimesh/TriMesh_normals.h"
#include "Trimesh/remove.h"
#include <numeric>
using namespace std;


#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif


// Quick 'n dirty portable random number generator 
static inline float tinyrnd()
{
	static unsigned trand = 0;
	trand = 1664525u * trand + 1013904223u;
	return (float) trand / 4294967296.0f;
}


// Is this argument a number?
static bool isanumber(const char *c)
{
	if (!c || !*c)
		return false;
	char *endptr;
	strtod(c, &endptr);
	return (endptr && *endptr == '\0');
}


// Transform the mesh by the given matrix
void apply_xform(TriMesh *mesh, const xform &xf)
{
	int nv = mesh->vertices.size();
	for (int i = 0; i < nv; i++)
		mesh->vertices[i] = xf * mesh->vertices[i];
	if (!mesh->normals.empty()) {
		xform nxf = norm_xf(xf);
		for (int i = 0; i < nv; i++) {
			mesh->normals[i] = nxf * mesh->normals[i];
			normalize(mesh->normals[i]);
		}
	}
}


// Transform the mesh by a matrix read from a file
void apply_xform(TriMesh *mesh, const char *xffilename)
{
	xform xf;
	if (!xf.read(xffilename))
		fprintf(stderr, "Couldn't open %s\n", xffilename);
	else
		apply_xform(mesh, xf);
}


// Clip mesh to the given bounding box
bool clip(TriMesh *mesh, const char *bboxfilename)
{
	FILE *f = fopen(bboxfilename, "r");
	if (!f) {
		fprintf(stderr, "Couldn't open %s\n", bboxfilename);
		return false;
	}

	TriMesh::BBox b;
	if (fscanf(f, "%f%f%f%f%f%f",
		&b.min[0], &b.min[1], &b.min[2],
  &b.max[0], &b.max[1], &b.max[2]) != 6) {
	  fclose(f);
	  fprintf(stderr, "Couldn't read %s\n", bboxfilename);
	  return false;
  }
  fclose(f);

  if (b.min[0] > b.max[0]) swap(b.min[0], b.max[0]);
  if (b.min[1] > b.max[1]) swap(b.min[1], b.max[1]);
  if (b.min[2] > b.max[2]) swap(b.min[2], b.max[2]);


  int nv = mesh->vertices.size();
  vector<bool> toremove(nv, false);
  for (int i = 0; i < nv; i++)
	  if (mesh->vertices[i][0] < b.min[0] ||
			 mesh->vertices[i][0] > b.max[0] ||
			 mesh->vertices[i][1] < b.min[1] ||
			 mesh->vertices[i][1] > b.max[1] ||
			 mesh->vertices[i][2] < b.min[2] ||
			 mesh->vertices[i][2] > b.max[2])
		  toremove[i] = true;

  remove_vertices(mesh, toremove);

  return true;
}


// Translate the mesh by (tx, ty, tz)
static inline void trans(TriMesh *mesh, const vec &transvec)
{
	apply_xform(mesh, xform::trans(transvec));
}


// Find center of mass
point center_of_mass(TriMesh *mesh)
{
	point com;
	float totwt = 0;

	mesh->need_faces();
	int nf = mesh->faces.size();
	for (int i = 0; i < nf; i++) {
		const point &v0 = mesh->vertices[mesh->faces[i][0]];
		const point &v1 = mesh->vertices[mesh->faces[i][1]];
		const point &v2 = mesh->vertices[mesh->faces[i][2]];

		point face_com = (v0+v1+v2) / 3.0f;
		float wt = len((v1-v0) CROSS (v2-v0));
		com += wt * face_com;
		totwt += wt;
	}
	return com / totwt;
}


// Scale the mesh so that mean squared distance from center of mass is 1
void normalize_variance(TriMesh *mesh)
{
	point com = center_of_mass(mesh);
	trans(mesh, -com);

	float var = 0;
	int nv = mesh->vertices.size();
	for (int i = 0; i < nv; i++)
		var += len2(mesh->vertices[i]);
	float s = 1.0f / sqrt(var);
	apply_xform(mesh, xform::scale(s));

	trans(mesh, com);
}


// Rotate model so that first principal axis is along +X (using
// forward weighting), and the second is along +Y
void pca_rotate(TriMesh *mesh)
{
	point com = center_of_mass(mesh);
	trans(mesh, -com);

	float C[3][3] = { {0,0,0}, {0,0,0}, {0,0,0} };
	int nv = mesh->vertices.size();
	for (int i = 0; i < nv; i++) {
		const vec &p = mesh->vertices[i];
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				C[j][k] += p[j] * p[k];
	}
	float e[3];
	eigdc(C, e);

	// Sorted in order from smallest to largest, so grab third column
	vec first(C[0][2], C[1][2], C[2][2]);
	int npos = 0;
	for (int i = 0; i < nv; i++)
		if ((mesh->vertices[i] DOT first) > 0.0f)
			npos++;
	if (npos < nv/2)
		first = -first;

	vec second(C[0][1], C[1][1], C[2][1]);
	npos = 0;
	for (int i = 0; i < nv; i++)
		if ((mesh->vertices[i] DOT second) > 0.0f)
			npos++;
	if (npos < nv/2)
		second = -second;

	vec third = first CROSS second;

	xform xf;
	xf[0] = first[0];  xf[1] = first[1];  xf[2] = first[2];
	xf[4] = second[0]; xf[5] = second[1]; xf[6] = second[2];
	xf[8] = third[0];  xf[9] = third[1];  xf[10] = third[2];

	invert(xf);
	for (int i = 0; i < nv; i++)
		mesh->vertices[i] = xf * mesh->vertices[i];

	trans(mesh, com);
}


// As above, but only rotate by 90/180/etc. degrees w.r.t. original
void pca_snap(TriMesh *mesh)
{
	point com = center_of_mass(mesh);
	trans(mesh, -com);

	float C[3][3] = { {0,0,0}, {0,0,0}, {0,0,0} };
	int nv = mesh->vertices.size();
	for (int i = 0; i < nv; i++) {
		const vec &p = mesh->vertices[i];
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				C[j][k] += p[j] * p[k];
	}
	float e[3];
	eigdc(C, e);

	// Sorted in order from smallest to largest, so grab third column
	vec first(C[0][2], C[1][2], C[2][2]);
	int npos = 0;
	for (int i = 0; i < nv; i++)
		if ((mesh->vertices[i] DOT first) > 0.0f)
			npos++;
	if (npos < nv/2)
		first = -first;
	if (fabs(first[0]) > fabs(first[1])) {
		if (fabs(first[0]) > fabs(first[2])) {
			first[1] = first[2] = 0;
			first[0] /= fabs(first[0]);
		} else {
			first[0] = first[1] = 0;
			first[2] /= fabs(first[2]);
		}
	} else {
		if (fabs(first[1]) > fabs(first[2])) {
			first[0] = first[2] = 0;
			first[1] /= fabs(first[1]);
		} else {
			first[0] = first[1] = 0;
			first[2] /= fabs(first[2]);
		}
	}

	vec second(C[0][1], C[1][1], C[2][1]);
	npos = 0;
	for (int i = 0; i < nv; i++)
		if ((mesh->vertices[i] DOT second) > 0.0f)
			npos++;
	if (npos < nv/2)
		second = -second;
	second -= first * (first DOT second);
	if (fabs(second[0]) > fabs(second[1])) {
		if (fabs(second[0]) > fabs(second[2])) {
			second[1] = second[2] = 0;
			second[0] /= fabs(second[0]);
		} else {
			second[0] = second[1] = 0;
			second[2] /= fabs(second[2]);
		}
	} else {
		if (fabs(second[1]) > fabs(second[2])) {
			second[0] = second[2] = 0;
			second[1] /= fabs(second[1]);
		} else {
			second[0] = second[1] = 0;
			second[2] /= fabs(second[2]);
		}
	}

	vec third = first CROSS second;

	xform xf;
	xf[0] = first[0];  xf[1] = first[1];  xf[2] = first[2];
	xf[4] = second[0]; xf[5] = second[1]; xf[6] = second[2];
	xf[8] = third[0];  xf[9] = third[1];  xf[10] = third[2];

	invert(xf);
	for (int i = 0; i < nv; i++)
		mesh->vertices[i] = xf * mesh->vertices[i];

	trans(mesh, com);
}


// Helper function: return the largest X coord for this face
static float max_x(const TriMesh *mesh, int i)
{
	return max(max(mesh->vertices[mesh->faces[i][0]][0],
			   mesh->vertices[mesh->faces[i][1]][0]),
	  mesh->vertices[mesh->faces[i][2]][0]);
}


// Flip faces so that orientation among touching faces is consistent
void orient(TriMesh *mesh)
{
	mesh->need_faces();
	mesh->tstrips.clear();
	mesh->need_adjacentfaces();

	mesh->flags.clear();
	const unsigned NONE = ~0u;
	mesh->flags.resize(mesh->faces.size(), NONE);

	TriMesh::dprintf("Auto-orienting mesh... ");
	unsigned cc = 0;
	vector<int> cc_farthest;
	for (int i = 0; i < mesh->faces.size(); i++) {
		if (mesh->flags[i] != NONE)
			continue;
		mesh->flags[i] = cc;
		cc_farthest.push_back(i);
		float farthest_val = max_x(mesh, i);

		vector<int> q;
		q.push_back(i);
		while (!q.empty()) {
			int f = q.back();
			q.pop_back();
			for (int j = 0; j < 3; j++) {
				int v0 = mesh->faces[f][j];
				int v1 = mesh->faces[f][(j+1)%3];
				const vector<int> &a = mesh->adjacentfaces[v0];
				for (int k = 0; k < a.size(); k++) {
					int f1 = a[k];
					if (mesh->flags[f1] != NONE)
						continue;
					int i0 = mesh->faces[f1].indexof(v0);
					int i1 = mesh->faces[f1].indexof(v1);
					if (i0 < 0 || i1 < 0)
						continue;
					if (i1 == (i0 + 1) % 3)
						swap(mesh->faces[f1][1],
							 mesh->faces[f1][2]);
					mesh->flags[f1] = cc;
					if (max_x(mesh, f1) > farthest_val) {
						farthest_val = max_x(mesh, f1);
						cc_farthest[cc] = f1;
					}
					q.push_back(f1);
				}
			}
		}
		cc++;
	}

	vector<bool> cc_flip(cc, false);
	for (int i = 0; i < cc; i++) {
		int f = cc_farthest[i];
		const point &v0 = mesh->vertices[mesh->faces[f][0]];
		const point &v1 = mesh->vertices[mesh->faces[f][1]];
		const point &v2 = mesh->vertices[mesh->faces[f][2]];
		int j = 0;
		if (v1[0] > v0[0])
			if (v2[0] > v1[0]) j = 2; else j = 1;
		else
			if (v2[0] > v0[0]) j = 2;
		int v = mesh->faces[f][j];
		const vector<int> &a = mesh->adjacentfaces[v];
		vec n;
		for (int k = 0; k < a.size(); k++) {
			int f1 = a[k];
			const point &v0 = mesh->vertices[mesh->faces[f1][0]];
			const point &v1 = mesh->vertices[mesh->faces[f1][1]];
			const point &v2 = mesh->vertices[mesh->faces[f1][2]];
			n += (v1 - v0) CROSS (v2 - v0);
		}
		if (n[0] < 0.0f)
			cc_flip[i] = true;
	}

	for (int i = 0; i < mesh->faces.size(); i++) {
		if (cc_flip[mesh->flags[i]])
			swap(mesh->faces[i][1], mesh->faces[i][2]);
	}
	TriMesh::dprintf("Done.\n");
}


// Remove boundary vertices (and faces that touch them)
void erode(TriMesh *mesh)
{
	int nv = mesh->vertices.size();
	vector<bool> bdy(nv);
	for (int i = 0; i < nv; i++)
		bdy[i] = mesh->is_bdy(i);
	remove_vertices(mesh, bdy);
}


// Add a bit of noise to the mesh
void noisify(TriMesh *mesh, float amount)
{
	mesh->need_normals();
	mesh->need_neighbors();
	int nv = mesh->vertices.size();
	vector<vec> disp(nv);

	for (int i = 0; i < nv; i++) {
		point &v = mesh->vertices[i];
		// Tangential
		for (int j = 0; j < mesh->neighbors[i].size(); j++) {
			const point &n = mesh->vertices[mesh->neighbors[i][j]];
			float scale = amount / (amount + len(n-v));
			disp[i] += (float) tinyrnd() * scale * (n-v);
		}
		if (mesh->neighbors[i].size())
			disp[i] /= (float) mesh->neighbors[i].size();
		// Normal
		disp[i] += (2.0f * (float) tinyrnd() - 1.0f) *
				amount * mesh->normals[i];
	}
	for (int i = 0; i < nv; i++)
		mesh->vertices[i] += disp[i];
}


int orient_smooth (const char *filenameIn, int niters,const char *filenameOut)
{
	TriMesh *themesh = TriMesh::read(filenameIn);
	if (!themesh)
	{
		printf("Mesh not readable\n");
		return 0;
	}
	
	bool have_tstrips = !themesh->tstrips.empty();
	orient(themesh);
	lmsmooth(themesh, niters);
	if (have_tstrips && themesh->tstrips.empty())
		themesh->need_tstrips();
	themesh->write(filenameOut);
	delete themesh;
}


int centre_bb_orient(const char *filenameIn, const char *filenameOut)
{
	TriMesh *themesh = TriMesh::read(filenameIn);
	if (!themesh)
	{
		printf("Mesh not readable\n");
		return 0;
	}
	
	bool have_tstrips = !themesh->tstrips.empty();
	
	trans(themesh, -center_of_mass(themesh));
	
	themesh->need_bbox();
	trans(themesh, -themesh->bbox.center());
	
	themesh->need_bbox();
	vec l = themesh->bbox.size();
	float ll = max(max(l[0], l[1]), l[2]);
	trans(themesh, -themesh->bbox.center());
	float s = 1.0f / ll;
	apply_xform(themesh, xform::scale(s));
	trans(themesh, themesh->bbox.center());
	
	orient(themesh);
	
	if (have_tstrips && themesh->tstrips.empty())
		themesh->need_tstrips();
	themesh->write(filenameOut);
	delete themesh;
}
