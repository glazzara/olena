
#include "Trimesh/TriMesh.h"
#include "Trimesh/TriMesh_algo.h"
#include "Trimesh/TriMesh_io.h"
#include "Trimesh/TriMesh_curvature.h"
#include "Trimesh/diffuse.h"
#include <cmath>
#include <float.h>
#include <algorithm>
using namespace std;

// Quick 'n dirty portable random number generator 
static inline float tinyrnd()
{
	static unsigned trand = 0;
	trand = 1664525u * trand + 1013904223u;
	return (float) trand / 4294967296.0f;
}

// Compute a "feature size" for the mesh: computed as 1% of 
// the reciprocal of the 10-th percentile curvature 
float feature_size(TriMesh *mesh)
{
	mesh->need_curvatures();
	int nv = mesh->curv1.size();
	int nsamp = min(nv, 500);
                
	vector<float> samples;
	samples.reserve(nsamp * 2);
        
	for (int i = 0; i < nsamp; i++) {
		// Quick 'n dirty portable random number generator  
		static unsigned randq = 0;
		randq = unsigned(1664525) * randq + unsigned(1013904223);

		int ind = int(tinyrnd() * nv);
		samples.push_back(fabs(mesh->curv1[ind]));
		samples.push_back(fabs(mesh->curv2[ind]));
	}
	const float frac = 0.1f;
	const float mult = 0.01f;
	int which = int(frac * samples.size());
	nth_element(samples.begin(), samples.begin() + which, samples.end());
	float f = mult / samples[which];
	if (!isfinite(f)) {
		mesh->need_bsphere();
		f = mesh->bsphere.r;
	}
		
	fprintf(stderr, "Feature size = %f\n", f);
	return f;
}

// Color based on curvature.  Similar to above, but uses a grayscale mapping.
void gcolorbycurv(TriMesh *mesh,float scale, float smooth)
{
	mesh->need_curvatures();
	if (smooth > 0.0f) {
		smooth *= mesh->feature_size();
		diffuse_curv(mesh, smooth);
	}
	float cscale = 10.0f * scale * feature_size(mesh);
	TriMesh::dprintf("Using scale = %f\n", cscale);

	int nv = mesh->vertices.size();
	for (int i = 0; i < nv; i++) {
		float H = 0.5f * (mesh->curv1[i] + mesh->curv2[i]);
		int c = int(min(max(256.0 / M_PI * (atan(H*cscale) + M_PI_2),
					0.0), 255.99));
		mesh->colors[i] = Color(c,c,c);
	}
}


int shade (const char *filenameIn, float scale, float smooth, const char *filenameOut)
{
	TriMesh *themesh = TriMesh::read(filenameIn);
	if (!themesh)
	{
		printf("Mesh not readable\n");
		return 0;
	}
	themesh->colors.resize(themesh->vertices.size());
	
	gcolorbycurv(themesh,scale,smooth);

	themesh->write(filenameOut);
	delete themesh;
	return 1;
}
