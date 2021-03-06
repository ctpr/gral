#ifndef GRAL_GB_COMPLEX2D_CELL_ON_EDGE2D_IT_H
#define GRAL_GB_COMPLEX2D_CELL_ON_EDGE2D_IT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL {

class CellOnEdge2D_Iterator : public complex2d_types {
  typedef CellOnEdge2D_Iterator self;
private:
  Edge   e;
  int    ce;

public:
  //------------------- types ---------------------

  typedef Complex2D grid_type;
  typedef Cell      element_type;
  typedef Cell      value_type;
  typedef Edge2D    anchor_type;
  typedef grid_incidence_iterator_category_d<2,1>  category;
  //----------------- construction -----------------

  CellOnEdge2D_Iterator() : e(), ce(0) {}
  CellOnEdge2D_Iterator(const Edge& ee) : e(ee), ce(1) 
  { make_valid();} 
  CellOnEdge2D_Iterator(const Edge& ee, int vv) : e(ee), ce(vv) {}

  //----------------- iteration --------------------

  Cell operator*() const { return cell();}
  self& operator++()       { ++ce; make_valid(); return *this;}
  bool  IsDone()     const { return (ce > 2);}
  operator bool()    const { return !IsDone();}

  //--------------- anchor access -------------------

  const Complex2D& TheGrid()  const { return e.TheGrid();}
  const Edge&      TheEdge()    const { return e;}
  const Edge&      TheAnchor()  const { return e;}
  const Edge&      TheFacet()   const { return e;}

  //--------------- comparisom ----------------------

  friend bool operator==(const self& ls, const self& rs) 
   { return ((ls.ce == rs.ce) && (ls.e == rs.e)); }
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}
private:
  Cell cell() const { return ( ce == 1 ? e.C1() : ( ce == 2 ? e.C2() : Cell()));}

  void make_valid() {
    while(! IsDone() && ! TheGrid().IsInside(cell()))
      ++ce;
  }
};

} // namespace GrAL {

#endif
