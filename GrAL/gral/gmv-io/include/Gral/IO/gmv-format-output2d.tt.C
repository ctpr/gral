#ifndef GRAL_GB_GMV_FMT_OUTPUT2D_TT
#define GRAL_GB_GMV_FMT_OUTPUT2D_TT

#include "Gral/IO/gmv-format-output2d.h"

template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO)
{
  List<END,END> L;
  ConstructGrid(Out,G,GEO,L);
}

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO,
		   List<GF,MOREGFS> GFS)
{
  typedef OstreamGMV2DFmt GMV3D;
  ostream& out = Out.Out();
  typedef grid_types<GRID> gt;

  out << "gmvinput ascii\n";
  out << "nodev " << G.NumOfVertices() << '\n';

  for(typename gt::VertexIterator v = G.FirstVertex(); ! v.IsDone(); ++v)
    out << GEO.coord(*v) << '\n';

  // map G's vertices to GMV numbers, starting from 1.
  //  element_numbering<typename gt::Vertex> G2GMV(G,1);
  // FIXME! use element_numbering as above
  // (does not currently work for facet_grid, because
  // only typedefs instead of own element types)
  grid_function<typename gt::Vertex, int> G2GMV(G.FirstVertex().TheGrid(),-1);
  int cnt = 1;
  for(typename gt::VertexIterator v = G.FirstVertex(); !v.IsDone(); ++v)
    G2GMV[*v] = cnt++;

  // cells are 3D for GMV
  out << "cells 0\n";

  // map cells = surface polygons for GMV
  out << "surface " << G.NumOfCells() << "\n";
  for(typename gt::CellIterator c = G.FirstCell(); ! c.IsDone(); ++c) {
    out << (*c).NumOfVertices() << " ";
    for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
      out << G2GMV(*vc) << " ";
    out << '\n';
  }

  Out.copy_grid_functions(GFS);
  out << "endgmv\n";
}

#endif