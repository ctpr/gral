#ifndef GRAL_GB_GEOMETRY_SIMPLEX_H
#define GRAL_GB_GEOMETRY_SIMPLEX_H

// $LICENSE_NEC_2003


#include "Geometry/point-traits.h"
//#include "Container/integer-iterator.h"

#include <algorithm>

template<unsigned K, class P = int>
class simplex_base {
  typedef  simplex_base self;
public:
  typedef P                           coord_type;
  typedef point_traits<P>             pt;
  typedef typename pt::component_type scalar_type;
  typedef size_t                      size_type;
protected:
  coord_type  p[K+1];
public:
  simplex_base() {}

  template<class It>
  simplex_base(It b, It e) { std::copy(b,e,p);}

  simplex_base(P const& apex, simplex_base<K-1, P> base)
  { p[0] = apex; std::copy(base.begin(), base.end(), p+1);}

  coord_type const& operator[](int i) const { return p[i];}
  coord_type const& operator()(int i) const { return p[i];}

  int dimension() const { return K;}
  int space_dimension() const { return pt::Dim(p[0]);}
  /*
  typedef integer_iterator<int, self>     VertexIterator;
  VertexIterator FirstVertex() const { return VertexIterator(0);}
  VertexIterator EndVertex()   const { return VertexIterator(K+1);}

  typedef coord_type const*  geom_vertex_iterator;
  geom_vertex_iterator begin_vertex() const { return p;}
  geom_vertex_iterator end_vertex()   const { return p+K+1;}
  */


  typedef coord_type const*  const_iterator;
  typedef coord_type      *  iterator;
  const_iterator begin() const { return p;}
  const_iterator end  () const { return p+K+1;}
  iterator begin() { return p;}
  iterator end  () { return p+K+1;}
  size_type size() const { return K+1;}
};


template<unsigned K, class P = int>
class simplex : public simplex_base<K, P>
{
  typedef simplex_base<K, P> base;
public:
  simplex() {}
  template<class It>
  simplex(It b, It e) : simplex_base<K,P>(b,e) {}
  simplex(P const& P0, simplex<K-1,P> const& b) : base(P0,b) {}
};

template<class P>
class simplex<0,P> : public simplex_base<0,P>
{
  typedef simplex_base<0, P> base;
public:
  simplex() {}
  simplex(P const& P0) { p[0]=P0;}
};

template<class P>
class simplex<1,P> : public simplex_base<1,P>
{
  typedef simplex_base<1, P> base;
public:
  simplex() {}
  simplex(P const& P0, P const& P1) { p[0]=P0;p[1]=P1;}
  simplex(P const& P0, simplex<0,P> const& b) : base(P0, b) {}
};

template<class P>
class simplex<2,P> : public simplex_base<2,P>
{
  typedef simplex_base<2, P> base;
public:
  simplex() {}
  simplex(P const& P0, P const& P1, P const& P2) { p[0]=P0;p[1]=P1;p[2]=P2;}
  simplex(P const& P0, simplex<1,P> const& b) : base(P0, b) {}
};

template<class P>
class simplex<3,P> : public simplex_base<3,P>
{
  typedef simplex_base<3, P> base;
public:
  simplex() {}
  simplex(P const& P0, P const& P1, P const& P2, P const& P3) { p[0]=P0;p[1]=P1;p[2]=P2;p[3]=P3;}
  simplex(P const& P0, simplex<2,P> const& b) : base(P0, b) {}
};



#endif