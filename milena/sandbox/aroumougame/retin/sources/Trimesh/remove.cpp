/*
Szymon Rusinkiewicz
Princeton University

remove.cc
Removing sets of vertices or faces from TriMeshes.
*/

#include <stdio.h>
#include "Trimesh/TriMesh.h"
#include "Trimesh/TriMesh_algo.h"
#include "Trimesh/reorder_verts.h"
#include "Trimesh/TriMesh_stats.h"

// Remove the indicated vertices from the TriMesh.
//
// Faces are renumbered to reflect the new numbering of vertices, and any
// faces that included a vertex to be removed will also be removed.
//
// Any per-vertex properties are renumbered along with the vertices.
// Any triangle strips, neighbor lists, or adjacency lists are deleted.
void remove_vertices(TriMesh *mesh, const vector<bool> &toremove)
{
	int numvertices = mesh->vertices.size();

	// Build a table that tells how the vertices will be remapped
	if (!numvertices)
		return;

	TriMesh::dprintf("Removing vertices... ");
	vector<int> remap_table(numvertices);
	int next = 0;
	int i;
	for (i = 0; i < numvertices; i++) {
		if (toremove[i])
			remap_table[i] = -1;
		else
			remap_table[i] = next++;
	}

	// Nothing to delete?
	if (next == numvertices) {
		TriMesh::dprintf("None removed.\n");
		return;
	}

	// Nuke connectivity lists, as well as anything we can re-compute
	bool have_col = !mesh->colors.empty();
	bool have_conf = !mesh->confidences.empty();
	bool have_flags = !mesh->flags.empty();
	bool have_normals = !mesh->normals.empty();
	bool have_pdir1 = !mesh->pdir1.empty();
	bool have_pdir2 = !mesh->pdir2.empty();
	bool have_curv1 = !mesh->curv1.empty();
	bool have_curv2 = !mesh->curv2.empty();
	bool have_dcurv = !mesh->dcurv.empty();
	mesh->cornerareas.clear(); mesh->pointareas.clear();
	bool had_tstrips = !mesh->tstrips.empty();
	bool had_faces = !mesh->faces.empty();
	mesh->need_faces();
	mesh->tstrips.clear();
	mesh->adjacentfaces.clear();
	mesh->neighbors.clear();
	mesh->across_edge.clear();

	// Remap the vertices and per-vertex properties
#define REMAP(property) mesh->property[remap_table[i]] = mesh->property[i]
	for (i = 0; i < numvertices; i++) {
		if (remap_table[i] == -1 || remap_table[i] == i)
			continue;
		REMAP(vertices);
		if (have_col) REMAP(colors);
		if (have_conf) REMAP(confidences);
		if (have_flags) REMAP(flags);
		if (have_normals) REMAP(normals);
		if (have_pdir1) REMAP(pdir1);
		if (have_pdir2) REMAP(pdir2);
		if (have_curv1) REMAP(curv1);
		if (have_curv2) REMAP(curv2);
		if (have_dcurv) REMAP(dcurv);
	}
	TriMesh::dprintf("%d vertices removed... Done.\n", numvertices - next);

#define ERASE(property) mesh->property.erase(mesh->property.begin() + next, \
	mesh->property.end())
	ERASE(vertices);
	if (have_col) ERASE(colors);
	if (have_conf) ERASE(confidences);
	if (have_flags) ERASE(flags);
	if (have_normals) ERASE(normals);
	if (have_pdir1) ERASE(pdir1);
	if (have_pdir2) ERASE(pdir2);
	if (have_curv1) ERASE(curv1);
	if (have_curv2) ERASE(curv2);
	if (have_dcurv) ERASE(dcurv);

	// Renumber faces
	int nextface = 0;
	for (i = 0; i < mesh->faces.size(); i++) {
		int n0 = (mesh->faces[nextface][0] = remap_table[mesh->faces[i][0]]);
		int n1 = (mesh->faces[nextface][1] = remap_table[mesh->faces[i][1]]);
		int n2 = (mesh->faces[nextface][2] = remap_table[mesh->faces[i][2]]);
		if ((n0 != -1) && (n1 != -1) && (n2 != -1))
			nextface++;
	}
	mesh->faces.erase(mesh->faces.begin() + nextface, mesh->faces.end());
	if (had_tstrips)
		mesh->need_tstrips();
	if (!had_faces)
		mesh->faces.clear();

	mesh->bsphere.valid = false;
}


// Remove vertices that aren't referenced by any face
void remove_unused_vertices(TriMesh *mesh)
{
	int numvertices = mesh->vertices.size();
	if (!numvertices)
		return;

	bool had_faces = !mesh->faces.empty();
	mesh->need_faces();
	vector<bool> unused(numvertices, true);
	for (int i = 0; i < mesh->faces.size(); i++) {
		unused[mesh->faces[i][0]] = false;
		unused[mesh->faces[i][1]] = false;
		unused[mesh->faces[i][2]] = false;
	}
	remove_vertices(mesh, unused);
	if (!had_faces)
		mesh->faces.clear();
}


// Remove faces as indicated by toremove.  Should probably be
// followed by a call to remove_unused_vertices()
void remove_faces(TriMesh *mesh, const vector<bool> &toremove)
{
	bool had_tstrips = !mesh->tstrips.empty();
	bool had_faces = !mesh->faces.empty();
	mesh->need_faces();
	int numfaces = mesh->faces.size();
	if (!numfaces)
		return;

	mesh->tstrips.clear();
	mesh->adjacentfaces.clear();
	mesh->neighbors.clear();
	mesh->across_edge.clear();
	mesh->cornerareas.clear();
	mesh->pointareas.clear();

	TriMesh::dprintf("Removing faces... ");
	int next = 0;
	for (int i = 0; i < numfaces; i++) {
		if (toremove[i])
			continue;
		mesh->faces[next++] = mesh->faces[i];
	}
	if (next == numfaces) {
		TriMesh::dprintf("None removed.\n");
		return;
	}

	mesh->faces.erase(mesh->faces.begin() + next, mesh->faces.end());
	TriMesh::dprintf("%d faces removed... Done.\n", numfaces - next);

	if (had_tstrips)
		mesh->need_tstrips();
	if (!had_faces)
		mesh->faces.clear();

	mesh->bsphere.valid = false;
}


// Remove long, skinny faces.  Should probably be followed by a
// call to remove_unused_vertices()
void remove_sliver_faces(TriMesh *mesh)
{
	bool had_faces = !mesh->faces.empty();
	mesh->need_faces();
	int numfaces = mesh->faces.size();

	const float l2thresh = sqr(4.0f * mesh->feature_size());
	const float cos2thresh = 0.85f;
	vector<bool> toremove(numfaces, false);
	for (int i = 0; i < numfaces; i++) {
		const point &v0 = mesh->vertices[mesh->faces[i][0]];
		const point &v1 = mesh->vertices[mesh->faces[i][1]];
		const point &v2 = mesh->vertices[mesh->faces[i][2]];
		float d01 = dist2(v0, v1);
		float d12 = dist2(v1, v2);
		float d20 = dist2(v2, v0);
		if (d01 < l2thresh && d12 < l2thresh && d20 < l2thresh)
			continue;
		// c2 is square of cosine of smallest angle
		float m = min(min(d01,d12),d20);
		float c2 = sqr(d01+d12+d20-2.0f*m) * m/(4.0f*d01*d12*d20);
		if (c2 < cos2thresh)
			continue;
		toremove[i] = true;
	}
	remove_faces(mesh, toremove);
	if (!had_faces)
		mesh->faces.clear();

	mesh->bsphere.valid = false;
}

