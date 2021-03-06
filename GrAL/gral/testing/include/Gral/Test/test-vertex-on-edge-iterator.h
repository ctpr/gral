#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of vertex-on-edge iterators (models of $GrAL VertexOnEdgeIterator).
*/


#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-on-edge-iterator.C"
#else

namespace GrAL {

/*! \brief Test a vertex-on-edge iterator
 
    Test a model of $GrAL VertexOnEdgeIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_vertex_on_edge_iterator(G const& g, ::std::ostream & out);

/*! \brief Test a vertex-on-edge iterator
 
    Test a model of $GrAL VertexOnEdgeIterator

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
bool test_vertex_on_edge_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
