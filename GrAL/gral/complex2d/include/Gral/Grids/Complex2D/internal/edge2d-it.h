#ifndef GRAL_GB_COMPLEX2D_EDGE2D_ITERATOR_H
#define GRAL_GB_COMPLEX2D_EDGE2D_ITERATOR_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Grids/Complex2D/complex2d.h"


namespace GrAL {

class Edge2D_Iterator  : public complex2d_types {
  typedef Edge2D_Iterator       self;
private:
  //----- DATA --------
  GridCellIterator      _c;  // current cell
  FacetOnCellIterator   _fc; // current edge == current facet of cell
  // invariant: (*(_c) == _fc.TheCell()) || IsDone()
public:
  typedef grid_sequence_iterator_category_d<1>  category;
  typedef edge_handle   handle_type;
  typedef Edge          element_type;
  typedef Edge          value_type;
  typedef grid_type     anchor_type;
  
  //--------- constructors -------------------------

  Edge2D_Iterator() {}
  Edge2D_Iterator(GridCellIterator    const& c, 
		  FacetOnCellIterator const& nb);
  explicit Edge2D_Iterator(Complex2D const&);
  Edge2D_Iterator(Complex2D const& g, edge_handle const& h);

  Edge2D_Iterator(const self& e) : _c(e._c), _fc(e._fc) {}
  self& operator=(const self& e) 
    { if (this != &e) { _c = e._c; _fc = e._fc; } return (*this);}

  ~Edge2D_Iterator() {}

  
  //---------- iterator operations ------------------
  inline self& operator++();
  Edge operator*() const {return (Edge(_fc));} 
  bool  IsDone() const {return (_c.IsDone());}
  operator bool() const { return !IsDone();}

  handle_type    handle()  const { return TheGrid().handle(this->operator*());}
  Complex const& TheGrid  () const {return (_c.TheGrid());}
  Complex const& TheAnchor() const {return (_c.TheGrid());}

  vertex_handle v1() const { return _fc.v1();}
  vertex_handle v2() const { return _fc.v2();}

  friend bool operator==(const self& ls, const self& rs) {
    // relies on shortcircuit to prevent (ls._fc == rs._fc) from
    // being executed if (ls._c != rs._c), thus violating the precondition
    // of the first comparision.
    // Also, this relies on Edge2D_Iterator visiting edges always from
    // one and the same side -- the `inverse' iterator won't compare equal.
    return (    (ls.IsDone() && rs.IsDone()) 
	     || ((ls._c == rs._c) && (ls._fc == rs._fc)));
  }
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  bool bound() const { return _c.bound();}
  bool valid() const { return _c.valid() && _fc.valid();}
private:
  //---------- helper functions for iteration -------
  inline void _advance_till_valid();
  inline void _advance_local();
  inline bool _is_new_edge();
};


inline Edge2D_Iterator::Edge2D_Iterator(Complex2D const& CC)
  : _c(CC)
{
  if(! _c.IsDone())
    _fc = (*_c).FirstFacet();
  _advance_till_valid();
}

inline Edge2D_Iterator::Edge2D_Iterator(Complex2D const&              CC,
					Complex2D::edge_handle const& h)
  : _c(CC,h.c), _fc(EdgeOnCellIterator(*_c,h.le))
{}



inline Edge2D_Iterator::Edge2D_Iterator(const Cell2D_Iterator& c,
					const EdgeOnCell2D_Iterator& nb)
 : _c(c), _fc(nb) 
{
  REQUIRE( ( (_c.IsDone()) || (*_c) == _fc.TheCell()), 
	   "Edge2D_Iterator: Constructor called with nb not on c!",1); 

  _advance_till_valid();
}

inline void Edge2D_Iterator::_advance_till_valid()
{
  while( ! IsDone() && ! _is_new_edge())
    ++(*this);
}

inline bool Edge2D_Iterator::_is_new_edge()
  // check, if the current edge (facet) is not
  // already visited from the other side.
{ 
  return (   TheGrid().IsOnBoundary(_fc)
	     || TheGrid().handle(this->operator*().C2()) <
	        TheGrid().handle(_c));
}

inline void Edge2D_Iterator::_advance_local()
{
 while(! _fc.IsDone()
       && ! _is_new_edge()) 
   ++_fc;
}

inline  Edge2D_Iterator& Edge2D_Iterator::operator++()
{ 
  ++_fc;
  _advance_local();
  while(_fc.IsDone() && ! _c.IsDone()) {
    ++_c;
    if(! _c.IsDone()){
      _fc = (*_c).FirstFacet();
      _advance_local();
    }
  }
    
  return (*this); 
}

} // namespace GrAL 


#endif
