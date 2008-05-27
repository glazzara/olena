/*
Szymon Rusinkiewicz
Princeton University

reorder_verts.cc
Reorder and remap vertices.
*/

#include <stdio.h>
#include "Trimesh/TriMesh.h"
#include "Trimesh/TriMesh_algo.h"
#include "Trimesh/TriMesh_pointareas.h"
#include "Trimesh/TriMesh_bounding.h"
#include <vector>


void reorder_verts(TriMesh *mesh)
{
	TriMesh::dprintf("Reordering vertices... ");

	int nv = mesh->vertices.size();
	vector<int> remap(nv, -1);
	int next = 0;
	if (!mesh->tstrips.empty()) {
		mesh->faces.clear();
		mesh->convert_strips(TriMesh::TSTRIP_TERM);
		for (int i = 0; i < mesh->tstrips.size(); i++) {
			int v = mesh->tstrips[i];
			if (v == -1)
				continue;
			if (remap[v] == -1)
				remap[v] = next++;
		}
	} else {
		for (int i = 0; i < mesh->faces.size(); i++) {
			for (int j = 0; j < 3; j++) {
				int v = mesh->faces[i][j];
				if (remap[v] == -1)
					remap[v] = next++;
			}
		}
	}

	if (next != nv) {
		// Unreferenced vertices...  Just stick them at the end.
		for (int i = 0; i < nv; i++)
			if (remap[i] == -1)
				remap[i] = next++;
	}

	TriMesh *oldmesh = new TriMesh;
	*oldmesh = *mesh;

	for (int i = 0; i < nv; i++)
		mesh->vertices[remap[i]] = oldmesh->vertices[i];
	if (!mesh->colors.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->colors[remap[i]] = oldmesh->colors[i];
	}
	if (!mesh->confidences.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->confidences[remap[i]] = oldmesh->confidences[i];
	}
	if (!mesh->flags.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->flags[remap[i]] = oldmesh->flags[i];
	}
	if (!mesh->normals.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->normals[remap[i]] = oldmesh->normals[i];
	}
	if (!mesh->pdir1.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->pdir1[remap[i]] = oldmesh->pdir1[i];
	}
	if (!mesh->pdir2.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->pdir2[remap[i]] = oldmesh->pdir2[i];
	}
	if (!mesh->curv1.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->curv1[remap[i]] = oldmesh->curv1[i];
	}
	if (!mesh->curv2.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->curv2[remap[i]] = oldmesh->curv2[i];
	}
	if (!mesh->dcurv.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->dcurv[remap[i]] = oldmesh->dcurv[i];
	}
	if (!mesh->cornerareas.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->cornerareas[remap[i]] = oldmesh->cornerareas[i];
	}
	if (!mesh->pointareas.empty()) {
		for (int i = 0; i < nv; i++)
			mesh->pointareas[remap[i]] = oldmesh->pointareas[i];
	}

	delete oldmesh;

	mesh->neighbors.clear();
	mesh->adjacentfaces.clear();
	mesh->across_edge.clear();

	if (!mesh->tstrips.empty()) {
		for (int i = 0; i < mesh->tstrips.size(); i++) {
			int &v = mesh->tstrips[i];
			if (v == -1)
				continue;
			v = remap[v];
		}
		mesh->convert_strips(TriMesh::TSTRIP_LENGTH);
	} else {
		for (int i = 0; i < mesh->faces.size(); i++) {
			for (int j = 0; j < 3; j++) {
				int &v = mesh->faces[i][j];
				v = remap[v];
			}
		}
	}

	TriMesh::dprintf("Done.\n");
}

