#ifndef GRAL_GB_TRIANG3D_STORED_GEOMETRY_H
#define GRAL_GB_TRIANG3D_STORED_GEOMETRY_H

// $LICENSE_NEC_2003

#include "Gral/Grids/Triang3D/triang3d.h"
#include "Geometry/point-traits.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>

/*! \brief Geometry wrapper class for Triang3D

   This class assumes vertex coordinates in an array
   \f$ (x_0,y_0, x_1, y_1, \ldots) \f$.

   It is a model of $GrAL VertexGridGeometry.
 */
class stored_geometry_triang3d {
public:
  typedef Triang3D              grid_type; 
  typedef grid_types<grid_type> gt;
  typedef gt::Vertex            Vertex;
private:
  grid_type const* g;
  double         * xyz;
  bool             owned;
public:
  stored_geometry_triang3d() : g(0), xyz(0), owned(false) {}
  //! initialize with value semantics (coords are owned)
  stored_geometry_triang3d(grid_type const& g_)
    : g(&g_)
    { 
      init_xyz();
    }
  //! initialize with reference semantics (coords are referenced)
  stored_geometry_triang3d(grid_type const& g_, double* xyz_)
    : g(&g_), xyz(xyz_), owned(false) {}

  ~stored_geometry_triang3d() { clear();}

  //! change to value semantics
  void set_grid(grid_type const& g_) {
    clear();
    g = & g_;
    init_xyz();
  }

private:
  void clear() { if (owned) delete [] xyz; xyz = 0;}
  void init_xyz() {
    if(g->NumOfVertices() > 0) {
      xyz = new double[3*g->NumOfVertices()];
      owned = true;
    }
    else {
      xyz = 0;
      owned = false;
    }
  }

public:
  grid_type const& TheGrid() const { return *g;}

  class coord_type;
  class coord_proxy {
    double* xyz_v; // start address of xyz values of a vertex
    friend class coord_type;
  public:
    coord_proxy(double* xyz_v_) : xyz_v(xyz_v_) {}
    inline void operator=(coord_type const& coo);
    
    double  operator[](int i) const { cr(i); return xyz_v[i]; }
    double& operator[](int i)       { cr(i); return xyz_v[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
    }
  };

  class coord_type : public array_operators<coord_type, double, 3> {
    double xyz[3];
  public:
    coord_type() {}
    coord_type(coord_proxy p)            { init(p.xyz_v);}
    coord_type(double const* xyz_)       { init(xyz_);}
    coord_type& operator=(coord_proxy p) { init(p.xyz_v); return *this;}


    double  operator[](int i) const { cr(i); return xyz[i]; }
    double& operator[](int i)       { cr(i); return xyz[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
    }
    void init(double const* p)
      { xyz[0] = p[0]; xyz[1] = p[1]; xyz[2] = p[2]; }
  };




  coord_proxy coord(Vertex const& v) 
    { return coord_proxy(xyz + 3*v.handle());}
  coord_type  coord(Vertex const& v) const 
    { return coord_type (xyz + 3*v.handle());}
};


inline std::ostream&
operator<<(std::ostream& out, stored_geometry_triang3d::coord_type const& p)
{ return (out << p[0] << ' ' << p[1] << ' ' << p[2]);}

inline std::istream&
operator>>(std::istream&  in, stored_geometry_triang3d::coord_type & p)
{ return (in >> p[0] >> p[1] >> p[2]);}


inline void
stored_geometry_triang3d::coord_proxy::operator=
(stored_geometry_triang3d::coord_type const& p)
{  xyz_v[0] = p[0]; xyz_v[1] = p[1]; xyz_v[2] = p[2];  }

template<>
struct point_traits<stored_geometry_triang3d::coord_type> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang3d::coord_type,double,3> {};

template<>
struct point_traits<stored_geometry_triang3d::coord_proxy> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang3d::coord_proxy,double,3> 
{
  typedef Ptype reference_type;
  static void ConstructWithDim(unsigned d, Ptype)
    { REQUIRE( d == 3, "d = " << d, 1); }
};

template<class Q>
inline void assign_point(stored_geometry_triang3d::coord_proxy p,
			 Q const& q) 
{
 typedef point_traits<stored_geometry_triang3d::coord_proxy> ptP;
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