#ifndef NMWR_GB_BOUNDARY_REG2D_H
#define NMWR_GB_BOUNDARY_REG2D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Base/boundary.h"
#include "Gral/Base/grid-functors.h"

#include "Gral/Iterators/boundary-iterator.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

namespace GrAL {

/*! \defgroup cartesian2dboundary Boundary Range for Cartesian2D
     \ingroup cartesian2dmodule
*/

/*! \brief Specialization of the BoundaryRange primary template for RegGrid2D
    \ingroup cartesian2dboundary
   
    \todo This uses generic boundary iteration. It could  make do with \f$ O(1) \f$
     memory when using a specialized implementation.
    \todo \c NumOfVertices() etc. is missing
 */
template<>
class BoundaryRange<cartesian2d::RegGrid2D> {
public:

  typedef cartesian2d::RegGrid2D   grid_type;
  typedef grid_types<grid_type>    gt;
  typedef gt::Edge                 Edge;
  typedef gt::Vertex               Vertex;

  typedef iscellinside_pred<grid_type>                          inside_p;
  typedef BoundaryComponentVertexIterator2D<grid_type,inside_p> VertexIterator;
  typedef BoundaryComponentEdgeIterator2D  <grid_type,inside_p> EdgeIterator;
  typedef BoundaryComponentEdgeIterator2D <grid_type,inside_p>  FacetIterator;
private:
  grid_type const* g;
  Edge             seed_edge;
public:
  BoundaryRange() : g(0) {}
  BoundaryRange(grid_type const& gg) : g(&gg) 
    {
      // get edge on boundary (should be the first edge)
      gt::EdgeIterator e = TheGrid().FirstEdge();
     while(! TheGrid().IsOnBoundary(*e))
       ++e;
     seed_edge = *e;
    }


  //  VertexIterator FirstVertex() const { return VertexIterator(seed_edge,IsEdgeOnBoundary(*g));}
  //  EdgeIterator   FirstEdge()   const { return EdgeIterator  (seed_edge,IsEdgeOnBoundary(*g));}
  //  FacetIterator  FirstFacet()  const { return FacetIterator (seed_edge,IsEdgeOnBoundary(*g));}
  VertexIterator FirstVertex() const { return VertexIterator(seed_edge,IsCellInside(*g));}
  EdgeIterator   FirstEdge()   const { return EdgeIterator  (seed_edge,IsCellInside(*g));}
  FacetIterator  FirstFacet()  const { return FacetIterator (seed_edge,IsCellInside(*g));}

  //  unsigned NumOfVertices() const;
  //  unsigned NumOfEdges()    const;
  //  unsigned NumOfFacets()   const;
  grid_type const& TheGrid() const { return *g;}

};


/*! \brief Specialization of the grid_types primary template for BoundaryRange<RegGrid2D>
  
    \ingroup cartesian2dboundary
*/
template<>
struct grid_types<BoundaryRange<cartesian2d::RegGrid2D> > 
{
  typedef grid_types<cartesian2d::RegGrid2D>     gt;
  typedef BoundaryRange<cartesian2d::RegGrid2D>  BrC;

  typedef  gt::Vertex Vertex;
  typedef  gt::Edge   Edge;
  typedef  gt::Facet  Facet;
 // typedef  gt::Cell   Cell;

  typedef  BrC::VertexIterator VertexIterator;
  typedef  BrC::EdgeIterator   EdgeIterator;
  typedef  BrC::FacetIterator  FacetIterator;
  //  typedef  BrC::CellIterator   CellIterator;

 // typedef  gt::VertexOnCellIterator VertexOnCellIterator;
 // typedef  gt::EdgeOnCellIterator   EdgeOnCellIterator;
 // typedef  gt::FacetOnCellIterator  FacetOnCellIterator;
 // typedef  gt::CellOnCellIterator   CellOnCellIterator;
};

} // namespace GrAL 

#endif
