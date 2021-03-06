#ifndef NMWR_GB_OVERLAPPING_GRID_FUNCTION_ADAPTER_H
#define NMWR_GB_OVERLAPPING_GRID_FUNCTION_ADAPTER_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <vector>

#include "Config/compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \brief A grid function that knows which parts are owned by other parts.
    \ingroup overlappinggrids
    \partof  overlappinggrids

    \templateparams
    - GF: sequential grid function
    - DGrid: overlapping grid, for example overlapping_grid

   \see module gridfunctions
 */
//----------------------------------------------------------------


template<class GF, class DGrid>
class overlapping_grid_function_adapter {
  typedef overlapping_grid_function_adapter<GF,DGrid> self;
public:
  typedef GF local_grid_function;
  typedef typename local_grid_function::element_type   element_type;
  typedef typename local_grid_function::value_type     value_type;
  typedef typename local_grid_function::size_type      size_type;
  typedef typename local_grid_function::const_iterator const_iterator;
  
  typedef element_type E;
  typedef value_type   T;
  typedef element_traits<element_type> et;
  typedef typename et::grid_type       base_grid_type;
  typedef DGrid                        ovrlp_grid_type;
  typedef typename DGrid::overlap_type overlap_type;
 

private:
  //------- DATA  -------
  ovrlp_grid_type const* the_ovrlp_grid; // referenced
  overlap_type    const* the_overlap;    // referenced
  local_grid_function    data;           // owned

  /*
  std::vector<combine_type_tag>  combine_types;

  combine_type_tag combine_type(const comm_range& cr) const 
    { return combine_types[cr.layer_type];}
  */

public:
  //------------------ construction -----------------------

  overlapping_grid_function_adapter() :  the_ovrlp_grid(0), the_overlap(0) {}

  // constructor for case that local_grid_function has value semantics.
  overlapping_grid_function_adapter(ovrlp_grid_type     const& og)
    : the_ovrlp_grid(& og), 
      the_overlap   (& og.TheOverlap()),
      data          (og.TheGrid())
    {}
  // constructor for case that local_grid_function has value semantics.
  overlapping_grid_function_adapter(ovrlp_grid_type     const& og,
				    T                   const& t)
    : the_ovrlp_grid(& og), 
      the_overlap   (& og.TheOverlap()),
      data          (og.TheGrid(),t)
    {}

  // constructor for case that local_grid_function has reference semantics.
  overlapping_grid_function_adapter(local_grid_function const& lgf, 
                                    ovrlp_grid_type     const& og)
    : the_ovrlp_grid(& og), 
      the_overlap   (& og.TheOverlap()),
      data          (lgf)
    {}

  //-----------------  data access  ------------------------

  T      & operator[](const E& e)       { return data[e];  }
  T const& operator()(const E& e) const { return data(e);  }

  //---------------- component access ----------------------

  overlap_type const& TheOverlap() const { return (*the_overlap);}

  local_grid_function const& TheBaseGridFunction() const { return data;}
  local_grid_function      & TheBaseGridFunction()       { return data;}

  //-------------------  ranges -----------------------------

  //  range_type_ref TheCalculationRange() const;

  size_type       size() const  { return data.size();}
  const_iterator  begin() const { return data.begin();}
  const_iterator  end()   const { return data.end  ();}
};

} // namespace GrAL 

#endif
