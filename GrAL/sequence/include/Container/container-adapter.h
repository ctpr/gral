#ifndef NMWR_GB_CONTAINER_ADAPTER_H
#define NMWR_GB_CONTAINER_ADAPTER_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/mapped-value-iterator.h"

namespace GrAL {

//----------------------------------------------------------------
/*! \brief This class gives a const view to the values sequences
     of an associative container.
    \ingroup containers

  <b> Template parameters: </b>
  - Cont:
    This is a sequence (STL) of pair<key,value> items.
    - typedef pair<key,value> value_type;
    - typedef const_iterator; (which derefences to value_type)

  \todo 
   Could be extended to KeyMask<Cont>.
*/

template<class Cont>
class ValueMask {
private:
  const Cont* the_container;
public:
  typedef typename Cont::value_type          base_value_type;
  typedef typename Cont::const_iterator      base_iter;
  typedef get_second<base_value_type>        functor_type;

  typedef mapped_value_const_iterator<base_iter,functor_type> const_iterator;
  typedef typename const_iterator::value_type                 value_type;

  ValueMask(const Cont& c) : the_container(&c) {}
  
  const_iterator begin() const 
    { return const_iterator(the_container->begin(),functor_type()); }
  const_iterator end() const 
    { return const_iterator(the_container->end(),functor_type()); }

  unsigned size() const { return the_container->size();}
};


template<class Cont>
inline ValueMask<Cont>
values_of(const Cont& c) { return ValueMask<Cont>(c);}

} // namespace GrAL 
#endif
