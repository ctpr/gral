#ifndef GRAL_GB_TEST_PRIMITIVES3D_H
#define GRAL_GB_TEST_PRIMITIVES3D_H

// $LICENSE

#include <iostream>

namespace GrAL {

template<class POINT>
struct test_algebraic_primitives3d
{
  void do_tests(::std::ostream & out);
};

} // namespace GrAL 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/internal/test-primitives3d.C"
#endif


#endif
