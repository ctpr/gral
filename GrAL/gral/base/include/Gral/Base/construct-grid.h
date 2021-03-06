#ifndef GRAL_BASE_GB_CONSTRUCT_GRID_H
#define GRAL_BASE_GB_CONSTRUCT_GRID_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Container/dummy-mapping.h"
#include "Container/partial-mapping.h"
#include "Geometry/point-traits.h"

#include "Gral/Base/grid-morphism.h"



namespace GrAL {

/*! \defgroup copyoperations Generic grid copy operations
    \brief copy-construct a grid \c destG from another grid \c srcG
    \ingroup mutatingoperations

    The generic versions implemented in this module
    are based on an overloaded master version for each destination
    grid type \c G1.

  \templateparams
   - G2: model of $GrAL Cell-VertexInputGridRange and $GrAL ArchetypedGrid, i.e. must support
     - grid_types<>, 
     - $GrAL CellIterator, $GrAL VertexIterator, $GrAL Vertex-On-CellIterator,
     - vertex_handle
     - archetype related functionality
   - Geom2 : model of VertexGridGeometry
     - <tt> Geom2::coord(Vertex2) </tt>
   - VertexMap:  map \f$  V(G_2) \mapsto V(G_1) \f$ 
      - <tt> vertex_handle1  operator()(vertex_handle2) </tt>(read)
      - <tt> vertex_handle1 operator[](vertex_handle2)  </tt> (write)
   - CellMap: map \f$ C(G_2) \mapsto C(G_1) \f$
     - <tt> cell_handle1 operator()(cell_handle2) </tt> (read)
     - <tt> cell_handle1 operator[](cell_handle2) </tt> (write)
 
  The following template function has to be implemented
  separately for each special type of G1:

  \code

  // construct without geometry
  template<class G1, class G2, class VertexMap, class CellMap>
  extern void 
  ConstructGrid0(G1            & destG, 
	         G2       const& srcG,
	         VertexMap     & VCorrG2_G1,
	         CellMap       & CCorrG2_G1); 

  \endcode
*/



/*!  Construct combinatorial grid, without mappings
     \ingroup copyoperations
 */
template<class G_DEST, class G_SRC>
inline void 
ConstructGrid0(G_DEST     & destG, 
	       G_SRC const&  srcG)
{
  typedef grid_types<G_SRC>                          gtsrc;
  typedef typename gtsrc::cell_handle                cell_handle_src;
  typedef typename gtsrc::vertex_handle              vertex_handle_src;
  typedef typename grid_types<G_DEST>::cell_handle   cell_handle_dest;
  typedef typename grid_types<G_DEST>::vertex_handle vertex_handle_dest;

  // here we are not interested in a cell correspondance
  dummy_mapping<cell_handle_src,cell_handle_dest>       c_corr;

  // VertexCorr must be a functional type, because it is used.
  //  vertex_morphism<G_SRC, G_DEST> v_corr(srcG, destG);
  partial_mapping<vertex_handle_src, vertex_handle_dest> v_corr;
  ConstructGrid0(destG,srcG,v_corr,c_corr);
}


/*!  Construct combinatorial grid, return vertex mapping
   \ingroup copyoperations
 */
template<class G_DEST, class G_SRC, class V_CORR>
inline void 
ConstructGrid0(G_DEST     & destG, 
	       G_SRC const& srcG,
	       V_CORR     & VertexCorr)
{
  typedef grid_types<G_SRC>                          gtsrc;
  typedef typename gtsrc::cell_handle                cell_handle_src;
  typedef typename grid_types<G_DEST>::cell_handle   cell_handle_dest;

  // here we are not interested in a cell correspondance
  dummy_mapping<cell_handle_src,cell_handle_dest>       CellCorr;

  // provided by the implementation of  G_DEST
  ConstructGrid0(destG,srcG,VertexCorr,CellCorr);
}


/*!  Construct combinatorial & generic grid, with vertex and cell  mappings
     \ingroup copyoperations

     This is a default version; it must be enabled explicitly by
     saying
     \code
     template<class GEOM_DEST,
             class G_SRC,  class GEOM_SRC,
             class VCORR,  class CCORR>
      void ConstructGridVC (MyGrid        & G_dest,
			    GEOM_DEST     & Geom_dest,
			    G_SRC    const& G_src,
			    GEOM_SRC const& Geom_src,
			    VCORR         & v_corr,
			    CCORR         & c_corr)
      { ConstructGridVC_generic(G_dest, Geom_dest, ...);}
     \endcode
     for each type \c MyGrid.

     This version just copies vertex coordinates from \c Geom_src
     to \c Geom_dest. For non-linear geometries, it has to be specialized.
 */

template<class G_DEST, class GEOM_DEST,
         class G_SRC,  class GEOM_SRC,
         class VCORR,  class CCORR>
inline
void ConstructGridVC_generic (G_DEST        & G_dest,
			      GEOM_DEST     & Geom_dest,
			      G_SRC    const& G_src,
			      GEOM_SRC const& Geom_src,
			      VCORR         & v_corr,
			      CCORR         & c_corr)
{ 
  // provided by the implementation of G_DEST
  ConstructGrid0(G_dest, G_src, v_corr, c_corr); 
  Geom_dest.rebind(G_dest);
  typedef grid_types<G_SRC>  srcgt;
  typedef grid_types<G_DEST> destgt;
  typedef typename GEOM_DEST::coord_type dest_coord_type;
  for(typename srcgt::VertexIterator vs=G_src.FirstVertex(); ! vs.IsDone(); ++vs) {
    Geom_dest.coord(typename destgt::Vertex(G_dest, v_corr[vs.handle()]))
      = convert_point<dest_coord_type>(Geom_src .coord(*vs));
  }
}

template<class G_DEST, class GEOM_DEST,
         class G_SRC,  class GEOM_SRC,
         class VCORR>
inline
void ConstructGridV(G_DEST        & G_dest,
		    GEOM_DEST     & Geom_dest,
		    G_SRC    const& G_src,
		    GEOM_SRC const& Geom_src,
		    VCORR         & v_corr)
{ 
  typedef grid_types<G_SRC>  srcgt;
  typedef grid_types<G_DEST> destgt;
  dummy_mapping<typename srcgt ::cell_handle,
                typename destgt::cell_handle> c_corr;
  ConstructGridVC_generic(G_dest,Geom_dest, G_src, Geom_src, v_corr, c_corr);
}


template<class G_DEST, class GEOM_DEST,
         class G_SRC,  class GEOM_SRC>
inline
void ConstructGrid(G_DEST        & G_dest,
		   GEOM_DEST     & Geom_dest,
		   G_SRC    const& G_src,
		   GEOM_SRC const& Geom_src)
{ 
  vertex_morphism<G_SRC, G_DEST> v_corr(G_src, G_dest);
  ConstructGridV(G_dest, Geom_dest, G_src, Geom_src, v_corr);
}

} // namespace GrAL 

#endif

