#ifndef GRAL_GB_TRIANG2D_STORED_GEOMETRY_H
#define GRAL_GB_TRIANG2D_STORED_GEOMETRY_H

// $LICENSE_NEC

#include "Gral/Grids/Triang2D/triang2d.h"
#include "Geometry/point-traits.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>

/*! \brief Geometry wrapper class for Triang2D

   This class assumes vertex coordinates in an array
   \f$ (x_0,y_0, x_1, y_1, \ldots) \f$.

   It is a model of $GrAL VertexGridGeometry.
 */
class stored_geometry_triang2d {
public:
  typedef Triang2D              grid_type; 
  typedef grid_types<grid_type> gt;
  typedef gt::Vertex            Vertex;
private:
  grid_type const* g;
  double         * xy;
  bool             owned;
public:
  stored_geometry_triang2d() : g(0), xy(0), owned(false) {}
  //! initialize with value semantics (coords are owned)
  stored_geometry_triang2d(grid_type const& g_)
    : g(&g_)
    { 
      init_xy();
    }
  //! initialize with reference semantics (coords are referenced)
  stored_geometry_triang2d(grid_type const& g_, double* xy_)
    : g(&g_), xy(xy_), owned(false) {}

  ~stored_geometry_triang2d() { clear();}

  //! change to value semantics
  void set_grid(grid_type const& g_) {
    clear();
    g = & g_;
    init_xy();
  }

private:
  void clear() { if (owned) delete [] xy; xy = 0;}
  void init_xy() {
    if(g->NumOfVertices() > 0) {
      xy = new double[2*g->NumOfVertices()];
      owned = true;
    }
    else {
      xy = 0;
      owned = false;
    }
  }

public:
  grid_type const& TheGrid() const { return *g;}

  class coord_type;
  class coord_proxy {
    double* xy_v; // start address of xyz values of a vertex
    friend class coord_type;
  public:
    coord_proxy(double* xy_v_) : xy_v(xy_v_) {}
    inline void operator=(coord_type const& coo);
    
    double  operator[](int i) const { cr(i); return xy_v[i]; }
    double& operator[](int i)       { cr(i); return xy_v[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  2, "i = " << i << " out of range!\n",1);
    }
  };

  class coord_type : public array_operators<coord_type, 2> {
    double xy[2];
  public:
    coord_type() {}
    coord_type(coord_proxy p)            { init(p.xy_v);}
    coord_type(double const* xy_)        { init(xy_);}
    coord_type& operator=(coord_proxy p) { init(p.xy_v); return *this;}


    double  operator[](int i) const { cr(i); return xy[i]; }
    double& operator[](int i)       { cr(i); return xy[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  2, "i = " << i << " out of range!\n",1);
    }
    void init(double const* p)
      { xy[0] = p[0]; xy[1] = p[1]; }
  };




  coord_proxy coord(Vertex const& v) 
    { return coord_proxy(xy + 2*v.handle());}
  coord_type  coord(Vertex const& v) const 
    { return coord_type (xy + 2*v.handle());}
};


inline std::ostream&
operator<<(std::ostream& out, stored_geometry_triang2d::coord_type const& p)
{ return (out << p[0] << ' ' << p[1]);}

inline std::istream&
operator>>(std::istream&  in, stored_geometry_triang2d::coord_type & p)
{ return (in >> p[0] >> p[1]);}


inline void
stored_geometry_triang2d::coord_proxy::operator=
(stored_geometry_triang2d::coord_type const& p)
{  xy_v[0] = p[0]; xy_v[1] = p[1]; }

template<>
struct point_traits<stored_geometry_triang2d::coord_type> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang2d::coord_type,double,2> {};

template<>
struct point_traits<stored_geometry_triang2d::coord_proxy> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang2d::coord_proxy,double,2> 
{
  typedef Ptype reference_type;
  static void ConstructWithDim(unsigned d, Ptype)
    { REQUIRE( d == 2, "d = " << d, 1); }
};

template<class Q>
inline void assign_point(stored_geometry_triang2d::coord_proxy p,
			 Q const& q) 
{
 typedef point_traits<stored_geometry_triang2d::coord_proxy> ptP;
 typedef point_traits<Q> ptQ;

 REQUIRE( (ptP::Dim(p) == ptQ::Dim(q)), 
	  "Cannot assign points of different dimension!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "(dim(q) = " << ptQ::Dim(q), 1);
 
 int iq = ptQ::LowerIndex(q); 
 int ip = ptP::LowerIndex(p);  
 for(; iq <= ptQ::UpperIndex(q); ++ip, ++iq)
   p[ip] = q[iq]; 
}  


#endif