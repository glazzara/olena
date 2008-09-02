/*
Szymon Rusinkiewicz
Princeton University

TriMesh_normals.cc
Compute per-vertex normals for TriMeshes

For meshes, uses average of per-face normals, weighted according to:
  Max, N.
  "Weights for Computing Vertex Normals from Facet Normals,"
  Journal of Graphics Tools, Vol. 4, No. 2, 1999.

For raw point clouds, fits plane to k nearest neighbors.
*/

#include <stdio.h>
#include "Trimesh/TriMesh.h"


// Compute per-vertex normals
void TriMesh::need_normals()
{
	if (normals.size() == vertices.size())
		return;
	need_faces();

	dprintf("Computing normals... ");

	normals.clear();
	normals.resize(vertices.size());

	int nf = faces.size(), nv = vertices.size();
	for (int i = 0; i < nf; i++) {
		const point &p0 = vertices[faces[i][0]];
		const point &p1 = vertices[faces[i][1]];
		const point &p2 = vertices[faces[i][2]];
		vec a = p0-p1, b = p1-p2, c = p2-p0;
		float l2a = len2(a), l2b = len2(b), l2c = len2(c);
		vec facenormal = a CROSS b;
		normals[faces[i][0]] += facenormal * (1.0f / (l2a * l2c));
		normals[faces[i][1]] += facenormal * (1.0f / (l2b * l2a));
		normals[faces[i][2]] += facenormal * (1.0f / (l2c * l2b));
	}

	for (int i = 0; i < nv; i++)
		normalize(normals[i]);

	dprintf("Done.\n");
}

