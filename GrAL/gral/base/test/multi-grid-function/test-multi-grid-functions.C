

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
*/

#include "Gral/Base/multi-grid-functions.h"
#include "Gral/Base/partial-multi-grid-functions.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"

using namespace GrAL;

template class multi_grid_function<cartesian2d::CartesianGrid2D,int>;
template class multi_grid_function<Complex2D,int>;

template class partial_multi_grid_function<cartesian2d::CartesianGrid2D,int>;
template class partial_multi_grid_function<Complex2D,int>;




template<class MGF, int D>
struct mgf_tester {

  static void act(MGF & mgf) {
    typedef typename MGF::grid_type  grid_type;
    typedef grid_types<grid_type>    gt;
    typedef typename sequence_iterator_d<gt,D>::type ElementIterator;
    typedef typename element_d          <gt,D>::type Element;

    mgf_tester<MGF,D-1>::act(mgf);
    for(ElementIterator e(mgf.TheGrid()); ! e.IsDone(); ++e) {
      mgf[*e] = typename MGF::value_type(D);
      REQUIRE_ALWAYS(mgf(*e) == typename MGF::value_type(D), "mgf(*e)=" << mgf(*e), 1);
      REQUIRE_ALWAYS(mgf.template ElementFunction<Element>()(*e) == typename MGF::value_type(D), "mgf(*e)=" << mgf(*e), 1);
    }

    typedef typename MGF::template element_wise<Element>::element_iterator gf_element_iterator;
    typedef typename MGF::template element_wise<Element>::const_iterator   gf_const_iterator;
    typedef typename MGF::template element_wise<Element>::iterator         gf_iterator;
    int ei_cnt =0, ci_cnt = 0, i_cnt = 0;
    for(gf_element_iterator e = mgf.template FirstElement<Element>(); 
	e != mgf.template EndElement<Element>(); ++e) {
      ++ei_cnt;
      REQUIRE_ALWAYS(mgf(*e) == typename MGF::value_type(D), "", 1);
    }
    for(gf_const_iterator e = mgf.template begin<Element>(); e != mgf.template end<Element>(); ++e)
      ++ci_cnt;
    for(gf_iterator       e = mgf.template begin<Element>(); e != mgf.template end<Element>(); ++e)
      ++i_cnt;
    REQUIRE_ALWAYS(ei_cnt == ci_cnt && ci_cnt == i_cnt && i_cnt == (int) mgf.template size<Element>(), "",1);
  }

};


template<class MGF>
struct mgf_tester<MGF,0> 
{
  static void act(MGF &) {}
};


template<class MGF>
void test_mgf(MGF& mgf)
{
  typedef typename MGF::grid_type grid_type;
  mgf_tester<MGF, grid_types<grid_type>::dim>::act(mgf);
}

int main() {

  {
    typedef   cartesian2d::CartesianGrid2D grid_type;
    typedef grid_types<grid_type> gt;
    grid_type R(2,2);

    typedef element_d<gt,0>::type Vertex;
    
    multi_grid_function        <grid_type, int> mgf (R,1);
    partial_multi_grid_function<grid_type, int> pmgf(R,1);

    pmgf.set_default(-1);
    REQUIRE_ALWAYS(pmgf.size() == 0, "", 1);
    REQUIRE_ALWAYS(pmgf.get_default() == -1, "pmgf::get_default()=" << pmgf.get_default() ,1);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) {
      REQUIRE_ALWAYS(pmgf(*v) == -1, "pmgf(*v)=" << pmgf(*v), 1);      
      REQUIRE_ALWAYS(!pmgf.defined(*v), "", 1);
    }
    for(gt::EdgeIterator e(R); ! e.IsDone(); ++e) {
      REQUIRE_ALWAYS(pmgf(*e) == -1, "pmgf(*e)=" << pmgf(*e), 1);
      REQUIRE_ALWAYS(!pmgf.defined(*e), "", 1);
    }
    for(gt::CellIterator c(R); ! c.IsDone(); ++c) {
      REQUIRE_ALWAYS(pmgf(*c) == -1, "pmgf(*c)=" << pmgf(*c), 1);
      REQUIRE_ALWAYS(!pmgf.defined(*c), "", 1);
    }
    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) {
      mgf [*v] = v.handle();
      pmgf[*v] = v.handle();
    }
    REQUIRE_ALWAYS(pmgf.size() == R.NumOfVertices(), "", 1);


    test_mgf(mgf);
    test_mgf(pmgf);

    pmgf.clear();
    REQUIRE_ALWAYS(pmgf.empty(), "", 1);
    REQUIRE_ALWAYS(pmgf.size() == 0, "", 1);

    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) {
      REQUIRE_ALWAYS(pmgf(*v) == -1, " pmgf(v)=" << pmgf(*v),1);
      // ! pmgf.defined(*v);
    }
    for(gt::EdgeIterator e(R); ! e.IsDone(); ++e)
      REQUIRE_ALWAYS(pmgf(*e) == -1, "pmgf(*e)=" << pmgf(*e), 1);
    for(gt::CellIterator c(R); ! c.IsDone(); ++c)
      REQUIRE_ALWAYS(pmgf(*c) == -1, "pmgf(*c)=" << pmgf(*c), 1);

    for(gt::VertexIterator v(R); !v.IsDone(); ++v ) 
      pmgf[*v] = v.handle();
    for(gt::EdgeIterator e(R); ! e.IsDone(); ++e)
      pmgf[*e] = e.handle();
    REQUIRE_ALWAYS(pmgf.size() == R.NumOfVertices()+R.NumOfEdges(), "", 1);
    for(gt::CellIterator c(R); ! c.IsDone(); ++c)
      pmgf[*c] = c.handle();
    REQUIRE_ALWAYS(pmgf.size() == R.NumOfVertices()+R.NumOfEdges()+R.NumOfCells(), "", 1);
    REQUIRE_ALWAYS( mgf.size() == R.NumOfVertices()+R.NumOfEdges()+R.NumOfCells(), "", 1);
    
  }

  {
    typedef   cartesiannd::grid<2> grid_type;
    typedef grid_types<grid_type> gt;
    grid_type R(gt::index_type(2,2));
    
    multi_grid_function<grid_type, int> mgf(R,1);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      mgf[*v] = v.handle();
    
    //  test_mgf(mgf);
  }
}
