#ifndef NMWR_GB_CELL_ON_CELL2D_ITERATOR_H
#define NMWR_GB_CELL_ON_CELL2D_ITERATOR_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/internal/edge-on-cell2d-it.h"

class CellOnCell2D_Iterator : public complex2d_types {
  typedef CellOnCell2D_Iterator   self;
private:
  //----- DATA --------
  EdgeOnCell2D_Iterator _f;
public:
  friend class Complex2D;
  friend class Cell2D;
  friend class EdgeOnCell2D_Iterator;

  typedef Cell value_type;
  typedef Cell element_type;
  typedef Cell anchor_type;

  //--------- constructors -----------------------------------

  CellOnCell2D_Iterator() {}
  CellOnCell2D_Iterator(Cell const& C);
  CellOnCell2D_Iterator(int cnt, const Cell& C)  : _f(cnt,C) 
    { make_valid(); }
  CellOnCell2D_Iterator(const EdgeOnCell2D_Iterator f) : _f(f)
    { make_valid(); }


  ~CellOnCell2D_Iterator() {}

  //----------- iterator operations --------------------------

  Cell operator*() const { return (_f.OtherCell()); }

  self& operator++()   { 
    ++_f;
    make_valid();
    return (*this);
  }
  
  inline self& operator--();

  
  inline self  CyclicSucc() const;
  inline self  CyclicPred() const;

  bool IsDone()      const { return _f.IsDone();}
  operator    bool() const { return !IsDone();}
  int  LocalNumber() const { return _f.LocalNumber(); }

  Cell             TheCell()  const { return(_f.TheCell());}
  Complex2D const& TheGrid()  const { return(_f.TheGrid());}

  friend bool operator==(const self& ls, const self& rs) 
  { return (ls._f == rs._f); }

private:
  inline void make_valid();
};



#endif
