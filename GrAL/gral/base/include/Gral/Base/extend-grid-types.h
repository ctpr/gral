#ifndef GRAL_BASE_GB_EXTEND_GRID_TYPES_H
#define GRAL_BASE_GB_EXTEND_GRID_TYPES_H

// $LICENSE_NEC
#include "Gral/Base/common-grid-basics.h"


#define DEFINE_OVERRIDE(T) template<class V> struct  override_##T { typedef V T;};

DEFINE_OVERRIDE(vertex_handle);
DEFINE_OVERRIDE(edge_handle);
DEFINE_OVERRIDE(face_handle);
DEFINE_OVERRIDE(facet_handle);
DEFINE_OVERRIDE(cell_handle);

DEFINE_OVERRIDE(Vertex);
DEFINE_OVERRIDE(Edge);
DEFINE_OVERRIDE(Face);
DEFINE_OVERRIDE(Facet);
DEFINE_OVERRIDE(Cell);

DEFINE_OVERRIDE(VertexIterator);
DEFINE_OVERRIDE(EdgeIterator);
DEFINE_OVERRIDE(FaceIterator);
DEFINE_OVERRIDE(FacetIterator);
DEFINE_OVERRIDE(CellIterator);

DEFINE_OVERRIDE(VertexOnVertexIterator);
DEFINE_OVERRIDE(EdgeOnVertexIterator);
DEFINE_OVERRIDE(FaceOnVertexIterator);
DEFINE_OVERRIDE(FacetOnVertexIterator);
DEFINE_OVERRIDE(CellOnVertexIterator);

DEFINE_OVERRIDE(VertexOnEdgeIterator);
DEFINE_OVERRIDE(FaceOnEdgeIterator);
DEFINE_OVERRIDE(FacetOnEdgeIterator);
DEFINE_OVERRIDE(CellOnEdgeIterator);

DEFINE_OVERRIDE(VertexOnFaceIterator);
DEFINE_OVERRIDE(EdgeOnFaceIterator);
DEFINE_OVERRIDE(CellOnFaceIterator);

DEFINE_OVERRIDE(VertexOnFacetIterator);
DEFINE_OVERRIDE(EdgeOnFacetIterator);
DEFINE_OVERRIDE(CellOnFacetIterator);

DEFINE_OVERRIDE(VertexOnCellIterator);
DEFINE_OVERRIDE(EdgeOnCellIterator);
DEFINE_OVERRIDE(FaceOnCellIterator);
DEFINE_OVERRIDE(FacetOnCellIterator);
DEFINE_OVERRIDE(CellOnCellIterator);

#undef DEFINE_OVERRIDE


/*! \brief factory for type-wise changes to a grid_types<> specialization.

   Usage:
   \code
   // grid types lacking VertexOnEdgeIterator
   grid_types<myGrid> gt;

   // Add  VertexOnEdgeIterator
   typedef vertex_on_edge_iterator<myGrid> myVertexOnEdgeIterator;
   typedef xgt<gt, override_VertexOnEdgeIterator<myVertexOnEdgeIterator> > mygt;

   // call some component needing  VertexOnEdgeIterator
   someAlgoNeedingVertexOnEdgeIter<mygt>(aGrid, ...);
   \endcode
 */
template<class ORIGGT, class CHANGE> 
struct xgt : public ORIGGT, public CHANGE {};


// specialize for a type possibly occuring in grid_types

#define SPEC_XGT(T)  \
    template<class ORIGGT, class X> \
    struct xgt<ORIGGT, override_##T<X> > : public ORIGGT { \
      typedef typename override_##T<X>:: T T; \
    };


  SPEC_XGT(vertex_handle);
  SPEC_XGT(edge_handle);
  SPEC_XGT(face_handle);
  SPEC_XGT(facet_handle);
  SPEC_XGT(cell_handle);

  SPEC_XGT(Vertex);
  SPEC_XGT(Edge);
  SPEC_XGT(Face);
  SPEC_XGT(Facet);
  SPEC_XGT(Cell);

  SPEC_XGT(VertexIterator);
  SPEC_XGT(EdgeIterator);
  SPEC_XGT(FaceIterator);
  SPEC_XGT(FacetIterator);
  SPEC_XGT(CellIterator);

  SPEC_XGT(VertexOnVertexIterator);
  SPEC_XGT(EdgeOnVertexIterator);
  SPEC_XGT(FaceOnVertexIterator);
  SPEC_XGT(FacetOnVertexIterator);
  SPEC_XGT(CellOnVertexIterator);

  SPEC_XGT(VertexOnEdgeIterator);
  SPEC_XGT(FaceOnEdgeIterator);
  SPEC_XGT(FacetOnEdgeIterator);
  SPEC_XGT(CellOnEdgeIterator);

  SPEC_XGT(VertexOnFaceIterator);
  SPEC_XGT(EdgeOnFaceIterator);
  SPEC_XGT(CellOnFaceIterator);

  SPEC_XGT(VertexOnFacetIterator);
  SPEC_XGT(EdgeOnFacetIterator);
  SPEC_XGT(CellOnFacetIterator);

  SPEC_XGT(VertexOnCellIterator);
  SPEC_XGT(EdgeOnCellIterator);
  SPEC_XGT(FaceOnCellIterator);
  SPEC_XGT(FacetOnCellIterator);
  SPEC_XGT(CellOnCellIterator);

#undef SPEC_XGT

#endif
