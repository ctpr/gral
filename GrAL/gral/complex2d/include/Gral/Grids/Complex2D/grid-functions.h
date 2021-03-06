#ifndef GRAL_GB_COMPLEX2D_GRIDFUNCTIONS_H
#define GRAL_GB_COMPLEX2D_GRIDFUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"

namespace GrAL {

/*! \defgroup complex2dgf Total grid functions for Complex2D

    \ingroup complex2dmodule

    We specialize the grid_function template for the element types
    of Complex2D.
 */

/*! \brief Specialization for Complex2D::Vertex
    \ingroup complex2dgf
    \see Module \ref complex2dgf
 */
template<class T>
class grid_function<Vertex2D,T> 
  : public grid_function_vector<Vertex2D,T>
{
  typedef grid_function<Vertex2D,T>        self;
  typedef grid_function_vector<Vertex2D,T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

/*! \brief Specialization for Complex2D::Vertex
    \ingroup complex2dgf
    \see Module \ref complex2dgf
 */
template<class T>
class grid_function<Cell2D,T> 
  : public grid_function_vector<Cell2D,T>
{
  typedef grid_function<Cell2D,T>         self;
  typedef grid_function_vector<Cell2D,T>  base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

/*! \brief Specialization for Complex2D::Vertex
    \ingroup complex2dgf
    \see Module \ref complex2dgf
 */
template<class T>
class grid_function<Edge2D,T> 
  : public grid_function_hash<Edge2D,T>
{
  typedef grid_function<Edge2D,T>       self;
  typedef grid_function_hash<Edge2D,T>  base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};

} // namespace GrAL 

#endif
