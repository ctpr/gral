/*! \file 
 */

/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/enclosed-volume.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format.h"

#include "Container/tuple-point-traits.h"

#include <iostream>

void test_volume(std::string file)
{

  typedef GrAL::tuple<double,3>                        coord_type;
  typedef GrAL::Complex2D                              grid_type;
  typedef GrAL::simple_geometry<grid_type, coord_type> geom_type;
  
  {
    grid_type G;
    geom_type GeomG(G);
    GrAL::IstreamComplex2DFmt IG(file);
    ConstructGrid(G, GeomG, IG, IG);
    
    double v  = GrAL::enclosed_volume_oriented(G,GeomG);
    double vo = GrAL::enclosed_volume(G,GeomG);
    std::cout << "Volume of grid \"" << file << "\" is " << v << " (assumed it's oriented)" << std::endl;
    std::cout << "Volume of grid \"" << file << "\" is " << vo << " (forced orientation)" << std::endl;
  }
  
  int sign[] = {-1,1};
  std::string signstr[] = { "neg", "pos"};

  for(int i = 0; i <= 1; i++)
  {
    grid_type G;
    geom_type GeomG(G);
    GrAL::IstreamComplex2DFmt IG(file);
    ConstructGrid(G, GeomG, IG, IG);
    
    bool res = GrAL::fix_geometric_orientation(G,GeomG,sign[i]);
    std::string orfile = "output/" + file + "-" + signstr[i] + "-oriented.complex2d";
    GrAL::OstreamComplex2DFmt Out(orfile);
    ConstructGrid0(Out, G);
    double v = GrAL::enclosed_volume_oriented(G,GeomG);
    std::cout << "Volume of " << signstr[i] <<  ". oriented grid \"" << orfile << "\" is " << v << std::endl;
  }
}

int main() 
{
  
  test_volume("unit-tet.complex2d");
  test_volume("unit-tet-neg.complex2d");
  test_volume("unit-cube.complex2d");
  test_volume("unit-cube-centered.complex2d");
  test_volume("unit-cube-1st-octant.complex2d");
  test_volume("unit-cube-not-oriented.complex2d");
}


