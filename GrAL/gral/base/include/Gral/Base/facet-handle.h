#ifndef GRAL_BASE_GB_GRID_FACET_HANDLE_H
#define GRAL_BASE_GB_GRID_FACET_HANDLE_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"

#include <iostream>
#include <stdlib.h>

namespace GrAL {

/*! \defgroup facethandle Generic facet handle

   \ingroup elements
 
*/

/*! \brief A generic facet handle

     \ingroup facethandle

  A generic facet handle defined by a (cell handle, local facet) pair.

 In the pair <tt>(c,lf)</tt>  \c c is a cell handle, 
 \c lf is an integer giving the local
 number of the facet on the cell \c c: \c lf \f$\in\f$  <tt>[0, Cell(c).NumOfFacets()) </tt>

 On construction, one has to take care that only one of two 
 possible handles for an interior facet is ever created.
 Else two handle denoting the same facet could compare not equal.
 This generally requires access to cell neighbors, i.e. a grid 
 with CellOnCellIterator defined.
 */
template<class CHandle>
struct facet_handle {
  CHandle c;  // cell
  int     lf; // 0 <= lf < NumOfFacets(c) : local position in cell

  facet_handle() {}
  facet_handle(CHandle cc, int llf) : c(cc), lf(llf) {}

  int local_facet() const { return lf;}
};

/*! \brief Equality test
    \ingroup facethandle
 */
template<class CHandle>
inline
bool operator== (facet_handle<CHandle> const& ls, facet_handle<CHandle> const& rs)
   { return ((ls.c == rs.c) && (ls.lf == rs.lf)); }

/*! \ingroup facethandle
 */
template<class CHandle>
inline
bool operator!= (facet_handle<CHandle> const& ls, facet_handle<CHandle> const& rs)
   { return ! (ls == rs) }

/*! \brief Ordering test

    The order defined is a total ordering, but arbitrary otherwise.

    \ingroup facethandle
 */
template<class CHandle>
inline
bool operator<  (facet_handle<CHandle> const& ls, facet_handle<CHandle> const& rs)
  { return ((ls.c < rs.c)  || ((ls.c == rs.c) &&  (ls.lf < rs.lf))); }

/*! \ingroup facethandle
 */
template<class CHandle>
inline
std::ostream& operator<< ( ::std::ostream& out, facet_handle<CHandle> const& e)
 { return (out << e.c << ' ' << e.lf);}

/*! \ingroup facethandle
 */
template<class CHandle>
inline
std::istream& operator>> ( ::std::istream& in,  facet_handle<CHandle>     & e)
 { return (in >> e.c >> e.lf);}

/*! \ingroup facethandle
 */
inline size_t hash_facet_handle(facet_handle<int> const& h)
{ return (8*h.c + h.lf);}

/*! \brief Specialization of  hash<> template.

   \ingroup  facet_handle

   The hash template is not yet a part of the standard, 
   and may live in different namespaces
   for different compilers.
 */
template<class T> class hash;
struct hash<facet_handle<int> > {
public:
  typedef facet_handle<int>     argument_type;
  typedef size_t                result_type;

  // hash function -- the factor 8 is heuristic, but it makes at
  // least sure that for the common cell types in 2D and 3D no two
  // different handles map to the same value 
  size_t operator()(const facet_handle<int>& h) const
    { return (8*h.c + h.lf);}
};

} // namespace GrAL 

#endif
