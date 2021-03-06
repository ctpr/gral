#ifndef GRAL_GB_SEQUENCE_ASSIGNMENT_OPERATORS_H
#define GRAL_GB_SEQUENCE_ASSIGNMENT_OPERATORS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*!  \defgroup operators Operator classes
     \ingroup accessors
*/

/*! \brief Assignment operators for parameterizing generic functions
    \ingroup operators
*/
namespace assignment_operators {

//! \ingroup operators 

struct assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls = rs;}
};

//! \ingroup operators 


struct add_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls += rs;}
};

//! \ingroup operators 

struct sub_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls -= rs;}
};

//! \ingroup operators 

struct mult_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls *= rs;}
};

//! \ingroup operators 

struct div_assign {
  template<class T, class U>
  void operator()(T& ls, const U& rs) { ls /= rs;}
};


} // namespace assigment_operators

} // namespace GrAL 

#endif
