
// LICENSE_NEC_2003

/*! \file Test harness for stored_geometry_triang3d

*/

#include "Gral/Grids/Triang3D/stored-geometry.h"
#include "Gral/Grids/Triang3D/grid-functions.h"

#include <iostream>

int main() {
  using namespace std;
  typedef Triang3D                 grid_type;
  typedef stored_geometry_triang3d geom_type;
  typedef grid_types<grid_type>    gt;

  typedef int tet[4];
  tet tri[] = { {0, 1, 2, 8}, // bottom 
		{0, 2, 3, 8},
		{4, 5, 6, 8}, // top
		{4, 6, 7, 8},
		{1, 2, 6, 8}, // right
		{1, 6, 5, 8},
		{0, 3, 7, 8}, // left
		{0, 7, 4, 8},
		{0, 1, 5, 8}, // front
		{0, 5, 4, 8},
		{3, 2, 6, 8}, // back
		{3, 6, 7, 8}};

  typedef double ct[3];
  ct coords[] = { {0.0, 0.0, 0.0}, // 0
		  {1.0, 0.0, 0.0}, // 1
		  {1.0, 1.0, 0.0}, // 2
		  {0.0, 1.0, 0.0}, // 3
		  {0.0, 0.0, 1.0}, // 4
		  {1.0, 0.0, 1.0}, // 5
		  {1.0, 1.0, 1.0}, // 6
		  {0.0, 1.0, 1.0}, // 7
		  {0.5, 0.5, 0.5}};// 8

  int nc = sizeof(tri)/sizeof(tet);
  int nv = sizeof(coords)/sizeof(ct);

  grid_type T((int *)tri, nc, nv);
  geom_type GeomT1(T, (double *)coords);

  grid_function<gt::Vertex, double> sum_ratios(T,0.0);
  for(gt::VertexIterator v(T); ! v.IsDone(); ++v)
    cout << v.handle() << " @ " << GeomT1.coord(*v) << endl;
  for(gt::CellIterator c(T); ! c.IsDone(); ++c) {
    cout << c.handle() 
	 << " [" << (*c).v(0) << "," << (*c).v(1) << "," << (*c).v(2) << "," << (*c).v(3) << "] "
	 <<  " center=" << GeomT1.center(*c) << "  ";
    for(gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc) {
      cout << "; v=" << vc.handle()
	   << ": a=" << GeomT1.solid_angle(vc)
	   << "  r=" << GeomT1.solid_angle_ratio(vc);
      sum_ratios[*vc] += GeomT1.solid_angle_ratio(vc);
    }
    cout << endl;
  }
  for(gt::VertexIterator v(T); ! v.IsDone(); ++v)
    cout << v.handle() << ":  sum of ratios=" << sum_ratios[*v] << endl;

}