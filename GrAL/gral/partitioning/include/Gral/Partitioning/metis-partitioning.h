#ifndef NMWR_GB_METIS_PARTITIONING_H
#define NMWR_GB_METIS_PARTITIONING_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

namespace GrAL {


class metis_options;

/*! \defgroup metispartitioning Converters to Metis Formats

    \ingroup partitioningmodule

   This module contains various classes and functions to interface
   with the $Metis grid partitioning library.

   The most important and convenient class in this module
   is MetisPartitioning.
 */


/*! \brief Partition a grid using the Metis library

  \ingroup metispartitioning
  \see Test in \ref test-metis-main.C

*/

class MetisPartitioning {
private:
  typedef MetisPartitioning self;

  metis_options * options;

  // FORBIDDEN
  MetisPartitioning(self const&);
  self & operator=(self const&);
public:
  MetisPartitioning();
  ~MetisPartitioning();

  /*! \brief Calculate a partitioning of the cells of G
    
     \templateparams
     - GRID:
       same as for metis_cell_graph_format()
     - PARTITIONING:
       <tt> int& operator[](GRID::Cell) </tt>
   */
  template<class GRID, class PARTITIONING>
  void calculate_cell_partitioning(GRID         const& G,   //<! IN
                                   PARTITIONING      & P,   //<! OUT
                                   int                 np); //<! IN: number of parts 
};


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/metis-partitioning-templates.C"
#endif


#endif
