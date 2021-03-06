#ifndef GRAL_GB_CARTESIAN3D_H
#define GRAL_GB_CARTESIAN3D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Grids/Complex2D/all.h"

#include "Container/index-map-nd.h"

#include "Utility/pre-post-conditions.h"


namespace GrAL {

namespace cartesian3d {

// forward declaration of related types

class CartesianGrid3D;
class Vertex_Cartesian3D;
class Edge_Cartesian3D;
class Facet_Cartesian3D;
class Cell_Cartesian3D;


class VertexOnCellIterator_Cartesian3D;
class EdgeOnCellIterator_Cartesian3D;
class FacetOnCellIterator_Cartesian3D;
  //class CellOnCellIterator_Cartesian3D;

class VertexOnFacetIterator_Cartesian3D;
class EdgeOnFacetIterator_Cartesian3D;
  //class CellOnFacetIterator_Cartesian3D;

  class VertexOnEdgeIterator_Cartesian3D;
  /*
  class  FacetOnEdgeIterator_Cartesian3D;
  class   CellOnEdgeIterator_Cartesian3D;

  class VertexOnVertexIterator_Cartesian3D;
  class   EdgeOnVertexIterator_Cartesian3D;
  class  FacetOnVertexIterator_Cartesian3D;
  class   CellOnVertexIterator_Cartesian3D;
  */

// collect related types in struct,
// to avoid retyping them in every one of the above classes.

struct grid_types_Cartesian3D : public grid_types_detail::grid_types_root {
  typedef grid_types_Cartesian3D     self;

  typedef index_map_nd<3>            index_map_type;
  typedef index_map_type::index_type index_type;

  typedef CartesianGrid3D            grid_type;

  typedef vertex_handle_int<grid_type,self>  vertex_handle;
  typedef edge_handle_int  <grid_type,self>  edge_handle;
  typedef facet_handle_int <grid_type,self>  facet_handle;
  typedef facet_handle                       face_handle;
  typedef cell_handle_int  <grid_type,self>  cell_handle;
  typedef int                                archetype_handle;

  typedef Vertex_Cartesian3D  Vertex;
  typedef Edge_Cartesian3D    Edge;
  typedef Facet_Cartesian3D   Facet;
  typedef Facet_Cartesian3D   Face;
  typedef Cell_Cartesian3D    Cell;

  typedef Vertex_Cartesian3D  VertexIterator;
  typedef Edge_Cartesian3D    EdgeIterator;
  typedef Facet_Cartesian3D   FacetIterator;
  typedef Facet_Cartesian3D   FaceIterator;
  typedef Cell_Cartesian3D    CellIterator;

  typedef VertexOnCellIterator_Cartesian3D VertexOnCellIterator;
  typedef EdgeOnCellIterator_Cartesian3D   EdgeOnCellIterator;
  typedef FacetOnCellIterator_Cartesian3D  FacetOnCellIterator;
  typedef FacetOnCellIterator_Cartesian3D  FaceOnCellIterator;
  // typedef CellOnCellIterator_Cartesian3D CellOnCellIterator;

  typedef VertexOnFacetIterator_Cartesian3D VertexOnFacetIterator;
  typedef EdgeOnFacetIterator_Cartesian3D   EdgeOnFacetIterator;
  typedef VertexOnFacetIterator_Cartesian3D VertexOnFaceIterator;
  typedef EdgeOnFacetIterator_Cartesian3D   EdgeOnFaceIterator;
  // typedef CellOnFacetIterator_Cartesian3D   CellOnFacetIterator;

  typedef VertexOnEdgeIterator_Cartesian3D VertexOnEdgeIterator;
  /*
  typedef  FacetOnEdgeIterator_Cartesian3D  FacetOnEdgeIterator;
  typedef   CellOnEdgeIterator_Cartesian3D   CellOnEdgeIterator;
  */

  /*
  typedef VertexOnVertexIterator_Cartesian3D VertexOnVertexIterator;
  typedef   EdgeOnVertexIterator_Cartesian3D EdgeOnVertexIterator;
  typedef  FacetOnVertexIterator_Cartesian3D FacetOnVertexIterator;
  typedef   CellOnVertexIterator_Cartesian3D CellOnVertexIterator;
  */

  typedef grid_dim_tag<3>     dimension_tag;

};



/*! \brief 3D Cartesian grid class. 
    \ingroup cartesian3dmodule

    Model of 
    $GrAL VertexGridRange,
    $GrAL EdgeGridRange,
    $GrAL FacetGridRange,
    $GrAL CellGridRange,
    $GrAL ArchetypedGrid.
  
    \todo CartesianGrid3D(1,1,1) gives an error in cell_map
          (division by 0). Check this in index_map_nd<>.
    \todo Implement validity checks for iterators
 */
class CartesianGrid3D : public grid_types_Cartesian3D {
public:
  typedef CartesianGrid3D              grid_type;
  typedef cartesian_grid_category_d<3> category;
  enum { dim = 3 };

  typedef Complex2D             archetype_type;
  typedef archetype_type const* archetype_iterator;

  //! \internal edge directions
  struct edge_direction   { enum dir { x  = 0, y  = 1, z = 2, size = 3 };};
  //! \internal facet directions
  struct  facet_direction { enum dir { xy = 0, xz = 1, yz= 2, size = 3 };};

private:
  index_map_type vertex_map; // maps vertex 3D integer coords to handles,
                             // and vice versa
  index_map_type cell_map;   // same for cells.
  index_map_type edge_maps [edge_direction::size];
  index_map_type facet_maps[facet_direction::size];

  int edge_handle_offset [edge_direction::size]; // offset[x] = 0; offset[y] = NumOfXEdges(); ...
  int facet_handle_offset[facet_direction::size];


private:
  // static data
  class SD {
  public:
    typedef CartesianGrid3D grid_type;
    SD();
    // get 3D vertex index from cell index
    // and local vertex number. Acts as geometry for the archetype below.
    grid_type::index_type vertex_on_cell_index_offset[8]; 

    edge_direction::dir edge_on_cell_dir[12];
    index_type          edge_on_cell_index_offset[12];
    facet_direction::dir facet_on_cell_dir[6];
    index_type           facet_on_cell_index_offset[6];

    index_type vertex_on_facet_index_offset[facet_direction::size][4];
    index_type edge_on_facet_index_offset[facet_direction::size][4];
    edge_direction::dir  edge_on_facet_dir[facet_direction::size][4];

    index_type edge_index_offset [edge_direction::size]; // { offset[x]=index_type(1,0,0); ...}
    index_type facet_index_offset[facet_direction::size][2]; // facet has 2 dimensions

                                          
    // 2D grid of a cell surface.
    // Although there is only one archetype, we
    // use an array to easily get an iterator type.
    grid_type::archetype_type  the_archetype[1]; 
  };
  static SD sd;

  class archetype_geom {
  public:
    typedef CartesianGrid3D::index_type coord_type;
   
    typedef CartesianGrid3D::archetype_type grid_type;
    typedef grid_types<archetype_type>      agt;
  private:
    grid_type const* g;
  public:
    archetype_geom(grid_type const& gg) : g(&gg) {}

    coord_type const& coord(agt::Vertex const& v) const
      { return CartesianGrid3D::sd.vertex_on_cell_index_offset[v.handle()];}
  };

public:
  //! Experimental: A geometry for the archetype
  typedef archetype_geom archetype_geom_type;

  friend class archetype_geom;
  friend class Vertex_Cartesian3D;
  friend class Edge_Cartesian3D;
  friend class Facet_Cartesian3D;
  friend class Cell_Cartesian3D;
  friend class VertexOnCellIterator_Cartesian3D;
  friend class EdgeOnCellIterator_Cartesian3D;
  friend class FacetOnCellIterator_Cartesian3D;
  friend class VertexOnFacetIterator_Cartesian3D;
  friend class EdgeOnFacetIterator_Cartesian3D;
public:
  /*@{*/ 
  /*! \name Constructors */
  //! Empty grid
  CartesianGrid3D();
  //! Grid with \f$ vx \times vy \times vz \f$ vertices.
  CartesianGrid3D(int vx, int vy, int vz);
  //! Grid with \f$ v[0] \times v[1] \times v[2] \f$ vertices.
  CartesianGrid3D(index_type v);
  /*@}*/
private:
  void init(int vx, int vy, int vz);
public:

  bool empty() const { return (NumOfVertices() == 0);}

  unsigned dimension() const { return 3;}
  index_type vertex_size() const { return vertex_map.max_tuple() + index_type(1);}
  index_type cell_size()   const { return cell_map  .max_tuple() + index_type(1);}

  /*@{*/ 
  /*! \name Sequence iteration 
     \brief Allows for STL style iteration
  */
  inline VertexIterator FirstVertex() const;
  inline EdgeIterator   FirstEdge()   const;
  inline FacetIterator  FirstFacet()  const;
  inline FaceIterator   FirstFace()   const;
  inline CellIterator   FirstCell()   const;
  inline VertexIterator EndVertex() const;
  inline EdgeIterator   EndEdge()   const;
  inline FacetIterator  EndFacet()  const;
  inline FaceIterator   EndFace()   const;
  inline CellIterator   EndCell()   const;

  unsigned NumOfVertices() const { return vertex_map.max_flat_index()+1;}
  unsigned NumOfCells   () const { return cell_map  .max_flat_index()+1;}
  unsigned NumOfEdges   () const { return NumOfXDirEdges()+NumOfYDirEdges()+NumOfZDirEdges(); }
  unsigned NumOfFacets  () const { return NumOfXYDirFacets()+NumOfXZDirFacets()+NumOfYZDirFacets();}
  unsigned NumOfFaces   () const { return NumOfFacets();}
  /*@}*/

  /*@{*/ 
  /*! \name Cartesian 1D element numbers
       \brief Number of vertices, cells along a grid dimension 
   */
  //! equal to \c vx arg of constructor
  unsigned NumOfXVertices() const { return vertex_map.max_tuple()[0]+1;}
  //! equal to \c vy arg of constructor
  unsigned NumOfYVertices() const { return vertex_map.max_tuple()[1]+1;}
  //! equal to \c vz arg of constructor
  unsigned NumOfZVertices() const { return vertex_map.max_tuple()[2]+1;}
  //! equal to \c NumOfXVertices()-1
  unsigned NumOfXCells   () const { return cell_map.max_tuple()[0]+1  ;}
  //! equal to \c NumOfYVertices()-1
  unsigned NumOfYCells   () const { return cell_map.max_tuple()[1]+1  ;}
  //! equal to \c NumOfZVertices()-1
  unsigned NumOfZCells   () const { return cell_map.max_tuple()[2]+1  ;}
  /*@}*/

  /*@{*/ 
  /*! \name Directional edge,facet element numbers
      \brief Number of Edges/Facets having a given direction.
      \note The meaning is different from NumOfXVertices() / NumOfXCells() etc. !! 
  */
  unsigned NumOfXDirEdges() const { return NumOfDirEdges(edge_direction::x);}
  unsigned NumOfYDirEdges() const { return NumOfDirEdges(edge_direction::y);}
  unsigned NumOfZDirEdges() const { return NumOfDirEdges(edge_direction::z);}
  unsigned NumOfDirEdges(edge_direction::dir dir) const { return edge_maps[dir].max_flat_index()+1;}

  unsigned NumOfXYDirFacets() const { return NumOfDirFacets(facet_direction::xy);}
  unsigned NumOfXZDirFacets() const { return NumOfDirFacets(facet_direction::xz);}
  unsigned NumOfYZDirFacets() const { return NumOfDirFacets(facet_direction::yz);}
  unsigned NumOfDirFacets(facet_direction::dir dir) const { return facet_maps[dir].max_flat_index()+1;}
  /*@}*/


  /*@{*/ 
  /*! \internal
      \name Element map accessors.
      Access to element maps, mapping flat indices to 3D and vice versa 
  */
  index_map_type const& VertexMap()                   const { return vertex_map;}
  index_map_type const& CellMap()                     const { return cell_map;}
  index_map_type const& EdgeMap (edge_direction::dir dir)  const { return edge_maps [dir];}
  index_map_type const& FacetMap(facet_direction::dir dir) const { return facet_maps[dir];}
  /*@}*/

  /*@{*/ 
  /*! \name Index+direction / handle conversion
      \brief direction/index extraction from handles and vice versa
   */
  vertex_handle get_vertex_handle(index_type const& I) const { return VertexMap()(I);}
  index_type    get_index        (vertex_handle h)     const { return VertexMap()(h);}  

  cell_handle   get_cell_handle  (index_type const& I) const { return CellMap()(I);}
  index_type    get_index        (cell_handle h)       const { return CellMap()(h);}  

  edge_handle get_edge_handle(edge_direction::dir d, index_type const& I) const
  { return edge_handle( EdgeMap(d)(I) + edge_handle_offset[d]); }

  index_type get_index(edge_handle e) const
  { 
    edge_direction::dir d = get_direction(e);
    return EdgeMap(d)(e - edge_handle_offset[d]);
  }

  edge_direction::dir get_direction(edge_handle e) const { cv(e);  
  return (e < (int) NumOfXDirEdges() ? edge_direction::x
	  : e <  (int) (NumOfXDirEdges() + NumOfYDirEdges()) ? edge_direction::y
	  : edge_direction::z);
  }

  facet_handle get_facet_handle(facet_direction::dir d, index_type const& I) const
  { return facet_handle( FacetMap(d)(I) + facet_handle_offset[d]); }

  index_type get_index(facet_handle f) const
  { 
    facet_direction::dir d = get_direction(f);
    return FacetMap(d)(f - facet_handle_offset[d]);
  }

  facet_direction::dir get_direction(facet_handle f) const
  {
    cv(f);  
    return (f < (int)NumOfXYDirFacets() ? facet_direction::xy
		: f < (int) (NumOfXYDirFacets() + NumOfXZDirFacets()) ? facet_direction::xz
		    : facet_direction::yz);
  }

  face_handle get_face_handle(facet_direction::dir d, index_type const& I) const
  { return get_facet_handle(d,I);}


  /*@}*/

  /*@{*/ 
  /*! \name Checking functions 
     \brief Check handles for validity
  */
  void cv(vertex_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(vertex_handle e) const { return (0 <= e && e < (int)NumOfVertices()); }
  void cv(edge_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(edge_handle e) const { return (0 <= e && e < (int)NumOfEdges()); }
  void cv(facet_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(facet_handle e) const { return (0 <= e && e < (int)NumOfFacets()); }
  void cv(cell_handle e) const { REQUIRE(valid(e), "",1);}
  bool valid(cell_handle e) const { return (0 <= e && e < (int)NumOfCells()); }
  /*@}*/

  /*@{*/ 

  /*! \name Archetype handling
   */
  static archetype_type const& Archetype(int a) {
    REQUIRE(a == 0, "a = " << a,1);
    return *BeginArchetype();
  }

  static archetype_type   const& ArchetypeOf (Cell const&)  
    { return *BeginArchetype();}
  static archetype_type   const& ArchetypeOf (cell_handle) 
    { return *BeginArchetype();}

  static archetype_handle        archetype_of(cell_handle) 
    { return 0;}
  static archetype_handle        archetype_of(Cell const&) 
    { return 0;}

  static archetype_iterator BeginArchetype() 
    { return archetype_iterator(sd.the_archetype);}
  static archetype_iterator EndArchetype()  { return BeginArchetype() +1;}
  static unsigned NumOfArchetypes() { return 1;}


  static archetype_geom_type ArchetypeGeom(archetype_type const& A 
				    = *BeginArchetype()) 
    { return archetype_geom_type(A); }

  static archetype_handle handle(archetype_iterator a)  
    { return a - BeginArchetype();}
  /*@}*/
};


class elem_base_Cartesian3D : public grid_types_Cartesian3D 
{
 private:
   grid_type const* g;
 public:
  elem_base_Cartesian3D() : g(0) {}
  elem_base_Cartesian3D(grid_type const& gg) : g(&gg) {}

  typedef grid_type anchor_type;
  grid_type const& TheGrid  () const { return *g;}
  grid_type const& TheAnchor() const { return *g;}
 
  bool bound() const { return g != 0;}
  void cb()    const { REQUIRE( bound(), "invalid element!",1);}
};


  /*! \brief Vertex class for CartesianGrid3D */
class Vertex_Cartesian3D : public elem_base_Cartesian3D {
  typedef Vertex_Cartesian3D     self;
  typedef elem_base_Cartesian3D  base;

  vertex_handle            h;
public:
  typedef self      value_type;
  struct category :
    virtual grid_vertex_category,
    virtual grid_vertex_iterator_category {};

  typedef vertex_type_tag      element_type_tag;

public:
  Vertex_Cartesian3D() {}  
  explicit
  Vertex_Cartesian3D(grid_type const&  gg, vertex_handle v = 0)
    : base(gg), h(v) {}
  Vertex_Cartesian3D(grid_type const&  gg, grid_type::index_type Iv)
    : base(gg)
  { h = TheGrid().vertex_map(Iv);}
  Vertex_Cartesian3D(grid_type const& gg, int x, int y, int z)
    : base(gg)
  {
    h = TheGrid().VertexMap()(grid_type::index_type(x,y,z));
  }

  self &      operator++()       { cv(); ++h; return *this;}
  self const& operator* () const { cv(); return *this;}
  bool IsDone() const { cb(); return h >= static_cast<int>(TheGrid().NumOfVertices());}
  vertex_handle handle() const { cv(); return h;}
  grid_type::index_type  index() const { cv(); return TheGrid().VertexMap()(h);}

  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h <  rhs.h);}

  bool valid() const { return bound() && !IsDone(); }
  void cv()    const { REQUIRE(valid(), "invalid Vertex: h = " << h , 1)};
};




 /*! \brief Edge class for CartesianGrid3D */

  class Edge_Cartesian3D : public elem_base_Cartesian3D {
    typedef Edge_Cartesian3D       self;
    typedef elem_base_Cartesian3D  base;
    typedef grid_type::edge_direction::dir direction_type;

    edge_handle h;
  public: 
    typedef self value_type;
    struct category :
      virtual grid_edge_category,
      virtual grid_edge_iterator_category {};
    typedef edge_type_tag      element_type_tag;
  public:
    Edge_Cartesian3D() {}
    Edge_Cartesian3D(grid_type const& gg, edge_handle e = 0) 
      : base(gg), h(e) {}
    //Edge_Cartesian3D(grid_type const& gg, index_type v1, index_type v2);
    Edge_Cartesian3D(grid_type const& gg, index_type v1, direction_type d)
      : base(gg), h(gg.get_edge_handle(d,v1)) {}
    
    self      & operator++()       { cv(); ++h; return *this;}
    self const& operator* () const { cv(); return *this;}
    bool IsDone() const { cb(); return h >= static_cast<int>(TheGrid().NumOfEdges());}

    edge_handle           handle() const { cv(); return h;}
    grid_type::index_type index () const { cv(); return TheGrid().get_index(h); }
    direction_type        dir   () const { cv(); return TheGrid().get_direction(h);}

    unsigned NumOfVertices() const { return 2;}
    inline VertexOnEdgeIterator FirstVertex() const;
    inline VertexOnEdgeIterator EndVertex()   const;
    /*!\brief  Vertex of edge with logical coordinate \c i
       \c V(0) is the start vertex, \c V(1) is the end vertex, 
        for other values of \c i  a vertex outside the edge is obtained.
     */
    Vertex V(int i) const { cv(); return Vertex(TheGrid(), index() + i*index_offset());}
    Vertex V1() const { return V(0);}
    Vertex V2() const { return V(1);}

    index_type const& index_offset() const { return grid_type::sd.edge_index_offset[dir()];}
    
    friend bool operator==(self const& lhs, self const& rhs)
    { lhs.cb(); rhs.cb(); return (lhs.h == rhs.h);}
    friend bool operator!=(self const& lhs, self const& rhs)
    { return !(lhs == rhs);}
    friend bool operator< (self const& lhs, self const& rhs)
    { lhs.cb(); rhs.cb(); return (lhs.h <  rhs.h);}
    
    bool valid() const { return bound() && !IsDone(); }
    void cv()    const { REQUIRE(valid(), "invalid Edge: h = " << h , 1)};
  };



 /*! \brief Facet class for CartesianGrid3D */

class Facet_Cartesian3D : public elem_base_Cartesian3D {
  typedef Facet_Cartesian3D       self;
  typedef elem_base_Cartesian3D   base;
  typedef grid_type::facet_direction::dir direction_type;

  facet_handle h;
public:
   typedef self value_type;
   struct category :
     virtual grid_facet_category,
     virtual grid_facet_iterator_category {};
   typedef facet_type_tag      element_type_tag;
public:
  Facet_Cartesian3D() {}
  Facet_Cartesian3D(grid_type const& gg, facet_handle e = 0) 
    : base(gg), h(e) {}
  //Facet_Cartesian3D(grid_type const& gg, index_type v1, index_type v2);
  Facet_Cartesian3D(grid_type const& gg, index_type v1, direction_type d)
  : base(gg), h(gg.get_facet_handle(d,v1)) {}

  self      & operator++()       { cv(); ++h;  return *this; }
  self const& operator* () const { cv();  return *this;}
  bool IsDone() const { cb(); return h >= static_cast<int>(TheGrid().NumOfFacets());}
  facet_handle          handle() const { cv(); return h;}
  grid_type::index_type index () const { cv(); return TheGrid().get_index(h);}
  direction_type        dir   () const { cv(); return TheGrid().get_direction(h);}

  Vertex V(int i, int j) const { 
    cv();
    direction_type d = dir();
    return Vertex(TheGrid(), index() + i*index_offset(d,0) + j*index_offset(d,1));
  }
  grid_type::index_type const& index_offset(direction_type d, int i) const { return grid_type::sd.facet_index_offset[d][i];}

  unsigned NumOfVertices() const { return 4;}
  unsigned NumOfEdges   () const { return 4;}
  inline VertexOnFacetIterator FirstVertex() const;
  inline VertexOnFacetIterator EndVertex()   const;
  inline EdgeOnFacetIterator   FirstEdge  () const;
  inline EdgeOnFacetIterator   EndEdge  ()   const;

  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h <  rhs.h);}

  bool valid() const { return bound() && !IsDone(); }
  void cv()    const { REQUIRE(valid(), "invalid Facet: h = " << h , 1)};
};





 /*! \brief Cell class for CartesianGrid3D */

class Cell_Cartesian3D : public elem_base_Cartesian3D {
  typedef Cell_Cartesian3D       self;
  typedef elem_base_Cartesian3D  base;
  typedef grid_type::archetype_type archetype_type;
  typedef grid_types<archetype_type> archgt;

  cell_handle              h;
  grid_type::index_type    I;

public:
   typedef self value_type;
  struct category :
    virtual grid_cell_category,
    virtual grid_cell_iterator_category {};

    typedef cell_type_tag      element_type_tag;
public:
  Cell_Cartesian3D() {}  
  explicit
  Cell_Cartesian3D(grid_type const&  gg, cell_handle c = 0)
    : base(gg), h(c) 
    { if(valid()) I = TheGrid().cell_map(h);}
  Cell_Cartesian3D(grid_type const&  gg, grid_type::index_type Ic)
    : base(gg), I(Ic) 
    { 
      if(TheGrid().CellMap().valid(I))
	h = TheGrid().cell_map(I);
    }

  // could be done more efficiently: cell_map.incr(I)
  self      & operator++()       { cv(); ++h; if(valid()) { I = TheGrid().cell_map(h);}  return *this;}
  self const& operator* () const { cv(); return *this;}
  bool        IsDone    () const { cb(); return h >= static_cast<int>(TheGrid().NumOfCells());}

  cell_handle                  handle() const { cv(); return h;}
  grid_type::index_type const& index () const { cv(); return I;}

  inline VertexOnCellIterator FirstVertex() const;
  inline VertexOnCellIterator EndVertex() const;
  unsigned NumOfVertices() const { return 8;}

  Vertex V(archgt::vertex_handle lh) const { // local2global
    cv(); return Vertex(TheGrid(), I+grid_type::sd.vertex_on_cell_index_offset[lh]);}
  vertex_handle v(archgt::vertex_handle lh) const 
    { cv(); return V(lh).handle();} // inefficient
  Vertex        V(archgt::Vertex lv) const { cv(); return V(lv.handle());}
  vertex_handle v(archgt::Vertex lv) const { cv(); return v(lv.handle());}

  Vertex V(int i, int j, int k) const { cv(); return Vertex(TheGrid(), index() + grid_type::index_type(i,j,k));}

  inline EdgeOnCellIterator FirstEdge() const;
  inline EdgeOnCellIterator EndEdge() const;
  unsigned NumOfEdges() const { return 12;}

  inline FacetOnCellIterator FirstFacet() const;
  inline FacetOnCellIterator EndFacet() const;
  unsigned NumOfFacets() const { return 6;}

  inline FacetOnCellIterator FirstFace() const;
  inline FacetOnCellIterator EndFace() const;
  unsigned NumOfFaces() const { return 6;}


  archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}

  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.h <  rhs.h);}

  bool valid() const { return bound() && !IsDone(); }
  void cv()    const { REQUIRE(valid(), "invalid Cell: h = " << h , 1)};
};





class VertexOnCellIterator_Cartesian3D 
  : public grid_types_Cartesian3D 
{
  typedef VertexOnCellIterator_Cartesian3D self;
private:
  Cell   c;
  int    lv; // archetype::vertex_handle?
public:
  typedef Vertex value_type;
  typedef Cell   anchor_type;
  typedef grid_incidence_iterator_category_d<0,3> category;
public:
  VertexOnCellIterator_Cartesian3D()  {}
  explicit
  VertexOnCellIterator_Cartesian3D(Cell const& cc, int llv = 0) 
    : c(cc), lv(llv) {}  

  self&  operator++()       { cv(); ++lv; return *this;}
  Vertex operator* () const { cv(); return Vertex(c.TheGrid(), index()); }
  bool IsDone() const { cb(); return lv >= 8;}

  grid_type::index_type   index()  const { cv(); 
    return c.index() + grid_type::sd.vertex_on_cell_index_offset[lv];
  }
  vertex_handle handle() const { cv(); return TheGrid().VertexMap()(index()); }
  int local_handle() const { cv(); return lv;}

  Cell      const& TheCell  () const { cb(); return c;}
  Cell      const& TheAnchor() const { cb(); return c;}
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c == rhs.c) && (lhs.lv == rhs.lv);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c < rhs.c) || ((lhs.c == rhs.c) && (lhs.lv <  rhs.lv));}

  bool bound() const { return c.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound VertexOnCellIterator!",1);}
  bool valid() const { return c.valid() && (0 <= lv) && (lv < 8);}
  void cv()    const { REQUIRE(valid(), "Invalid VertexOnCellIterator! lv = " << lv,1);}
};





class EdgeOnCellIterator_Cartesian3D 
  : public grid_types_Cartesian3D 
{
  typedef EdgeOnCellIterator_Cartesian3D self;
private:
  Cell   c;
  int    le; // archetype::edge_handle?
public:
  typedef Edge value_type;
  typedef Cell anchor_type;
  typedef grid_incidence_iterator_category_d<1,3> category;
public:
  EdgeOnCellIterator_Cartesian3D()  {}
  explicit
  EdgeOnCellIterator_Cartesian3D(Cell const& cc, int lle = 0) 
    : c(cc), le(lle) {}  

  self& operator++()       { cv(); ++le; return *this; }
  Edge  operator* () const { cv(); return Edge(c.TheGrid(), handle());}
  bool IsDone() const { cb(); return le >= 12;}

  grid_type::index_type          index()  const { cv(); return c.index() + grid_type::sd.edge_on_cell_index_offset[le]; }
  grid_type::edge_direction::dir dir()    const { cv(); return grid_type::sd.edge_on_cell_dir[le];}
  edge_handle                 handle()    const { cv(); return TheGrid().get_edge_handle(dir(), index());}
  int local_handle() const { cv(); return le;}

  Cell      const& TheCell()   const { cb(); return c;}
  Cell      const& TheAnchor() const { cb(); return c;}
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}


  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c == rhs.c) && (lhs.le == rhs.le);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c < rhs.c) || ((lhs.c == rhs.c) && (lhs.le <  rhs.le));}

  bool bound() const { return c.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound EdgeOnCellIterator!",1);}
  bool valid() const { return c.valid() && (0 <= le) && (le <  12);}
  void cv()    const { REQUIRE(valid(), "Invalid EdgeOnCellIterator! le = " << le,1);}
};




class FacetOnCellIterator_Cartesian3D 
  : public grid_types_Cartesian3D 
{
  typedef FacetOnCellIterator_Cartesian3D self;
  typedef grid_type::facet_direction::dir direction_type;
private:
  Cell   c;
  int    lf; // archetype::facet_handle?
public:
  typedef Facet value_type;
  typedef Cell  anchor_type;
  typedef grid_incidence_iterator_category_d<2,3> category;
public:
  FacetOnCellIterator_Cartesian3D()  {}
  explicit
  FacetOnCellIterator_Cartesian3D(Cell const& cc, int llf = 0) 
    : c(cc), lf(llf) {}  

  self&  operator++()       { cv(); ++lf; return *this; }
  Facet  operator* () const { cv(); return Facet(c.TheGrid(), handle());}
  bool IsDone() const { cb(); return lf >= 6;}

  grid_type::index_type index () const { cv(); return c.index() + grid_type::sd.facet_on_cell_index_offset[lf]; }
  direction_type        dir   () const { cv(); return grid_type::sd.facet_on_cell_dir[lf];}
  facet_handle          handle() const { cv(); return TheGrid().get_facet_handle(dir(), index());} 
  int local_handle() const { cv(); return lf;}

  Cell      const& TheCell()   const { cb(); return c;}
  Cell      const& TheAnchor() const { cb(); return c;}
  grid_type const& TheGrid() const { cb(); return c.TheGrid();}


  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c == rhs.c) && (lhs.lf == rhs.lf);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.c < rhs.c) || ((lhs.c == rhs.c) && (lhs.lf <  rhs.lf));}

  bool bound() const { return c.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound FacetOnCellIterator!",1);}
  bool valid() const { return c.valid() && (0 <= lf) && (lf < 6);}
  void cv()    const { REQUIRE(valid(), "Invalid FacetOnCellIterator! lf = " << lf,1);}
};




class VertexOnFacetIterator_Cartesian3D : public grid_types_Cartesian3D
{
  typedef VertexOnFacetIterator_Cartesian3D self;
private:
  Facet f;
  int   lv;
public:
  typedef Vertex value_type;
  typedef Facet  anchor_type;
  typedef grid_incidence_iterator_category_d<0,2> category;
public:
  VertexOnFacetIterator_Cartesian3D() {}
  explicit
  VertexOnFacetIterator_Cartesian3D(Facet const& ff, int llv = 0) : f(ff), lv(llv) {} 

  self&  operator++()       { cv(); ++lv; return *this; }
  Vertex operator* () const { cv(); return Vertex(f.TheGrid(), handle());}
  bool IsDone() const { cb(); return lv >= 4;}

  vertex_handle         handle() const { cv(); return TheGrid().VertexMap()(index());}
  grid_type::index_type index()  const { cv(); return (f.index() + grid_type::sd.vertex_on_facet_index_offset[f.dir()][lv]);}
  int local_handle() const { cv(); return lv;}

  Facet     const& TheFacet () const { cb(); return f;}
  Face      const& TheFace  () const { cb(); return f;}
  Face      const& TheAnchor() const { cb(); return f;}
  grid_type const& TheGrid()  const { cb(); return f.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs)
  {lhs.cb(); rhs.cb(); return (lhs.f == rhs.f) && (lhs.lv == rhs.lv);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  {lhs.cb(); rhs.cb(); return (lhs.f < rhs.f) || ((lhs.f == rhs.f) && (lhs.lv <  rhs.lv));}


  bool bound() const { return f.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound VertexOnFacetIterator!",1);}
  bool valid() const { return f.valid() && (0 <= lv) && (lv < 4);}
  void cv()    const { REQUIRE(valid(), "Invalid VertexOnFacetIterator! lv = " << lv,1);}
};



class EdgeOnFacetIterator_Cartesian3D : public grid_types_Cartesian3D
{
  typedef EdgeOnFacetIterator_Cartesian3D self;
  typedef grid_type::edge_direction::dir direction_type;
private:
  Facet f;
  int   le;
public:
  typedef Edge  value_type;
  typedef Facet anchor_type;
  typedef grid_incidence_iterator_category_d<1,2> category;
public:
  EdgeOnFacetIterator_Cartesian3D() {}
  explicit
  EdgeOnFacetIterator_Cartesian3D(Facet const& ff, int lle = 0) : f(ff), le(lle) {} 

  self& operator++()       { cv(); ++le; return *this; }
  Edge  operator* () const { cv(); return Edge(f.TheGrid(), handle());}
  bool IsDone() const { cb(); return le >= 4;}

  edge_handle            handle() const { cv(); return TheGrid().get_edge_handle(dir(), index());}
  grid_type::index_type  index () const { cv(); return (f.index() + grid_type::sd.edge_on_facet_index_offset[f.dir()][le]);}
  direction_type         dir   () const { cv(); return grid_type::sd.edge_on_facet_dir[f.dir()][le];}
  int local_handle() const { cv(); return le;}

  Facet     const& TheFacet () const { cb(); return f;}
  Face      const& TheFace  () const { cb(); return f;}
  Face      const& TheAnchor() const { cb(); return f;}
  grid_type const& TheGrid()  const { cb(); return f.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs)
  { return (lhs.f == rhs.f) && (lhs.le == rhs.le);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { return (lhs.f < rhs.f) || ((lhs.f == rhs.f) && (lhs.le <  rhs.le));}

  bool bound() const { return f.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound EdgeOnFacetIterator!",1);}

  bool valid() const { return f.valid() && (0 <= le) && (le < 4);}
  void cv()    const { REQUIRE(valid(), "Invalid EdgeOnFacetIterator! le = " << le,1);}
};




class VertexOnEdgeIterator_Cartesian3D : public grid_types_Cartesian3D
{
  typedef VertexOnEdgeIterator_Cartesian3D self;
private:
  Edge  e;
  int   lv;
public:
  typedef Vertex value_type;
  typedef Edge   anchor_type;
  typedef grid_incidence_iterator_category_d<0,1> category;
public:
  VertexOnEdgeIterator_Cartesian3D() {}
  explicit
  VertexOnEdgeIterator_Cartesian3D(Edge const& ee, int llv = 0) : e(ee), lv(llv) {} 

  self&  operator++()       { cv(); ++lv; return *this; }
  Vertex operator* () const { cv(); return Vertex(e.TheGrid(), handle());}
  bool   IsDone    () const { cb(); return lv >= 2;}

  vertex_handle         handle() const { cv(); return TheGrid().VertexMap()(index());}
  grid_type::index_type index () const { cv(); return (e.index() + lv * e.index_offset());}
  int             local_handle() const { cv(); return lv;}

  Edge      const& TheEdge()   const { cb(); return e;}
  Edge      const& TheAnchor() const { cb(); return e;}
  grid_type const& TheGrid() const { cb(); return e.TheGrid();}

  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.e == rhs.e) && (lhs.lv == rhs.lv);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb(); rhs.cb(); return (lhs.e < rhs.e) || ((lhs.e == rhs.e) && (lhs.lv <  rhs.lv));}


  bool bound() const { return e.valid();}
  void cb()    const { REQUIRE(bound(), "Unbound VertexOnEdgeIterator!",1);}
  bool valid() const { return e.valid() && (0 <= lv) && (lv < 2);}
  void cv()    const { REQUIRE(valid(), "Invalid VertexOnEdgeIterator! lv = " << lv,1);}
};


// inline member functions of Cartesian3D

inline Vertex_Cartesian3D
CartesianGrid3D::FirstVertex() const { return Vertex(*this);}
inline Vertex_Cartesian3D
CartesianGrid3D::EndVertex()   const { return Vertex(*this, vertex_handle(NumOfVertices()));}

inline Edge_Cartesian3D
CartesianGrid3D::FirstEdge() const { return Edge(*this);}
inline Edge_Cartesian3D
CartesianGrid3D::EndEdge()   const { return Edge(*this, edge_handle(NumOfEdges()));}


inline CartesianGrid3D::Face
CartesianGrid3D::FirstFace() const { return Face(*this);}
inline CartesianGrid3D::Face
CartesianGrid3D::EndFace()   const { return Face(*this, face_handle(NumOfFaces()));}

inline Facet_Cartesian3D
CartesianGrid3D::FirstFacet() const { return Facet(*this);}
inline Facet_Cartesian3D
CartesianGrid3D::EndFacet()   const { return Facet(*this, facet_handle(NumOfFacets()));}

inline Cell_Cartesian3D
CartesianGrid3D::FirstCell() const { return Cell(*this);}
inline Cell_Cartesian3D
CartesianGrid3D::EndCell()   const { return Cell(*this,cell_handle(NumOfCells()));}


// inline functions of Cell_Cartesian3D

inline VertexOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstVertex() const { return VertexOnCellIterator(*this);}
inline VertexOnCellIterator_Cartesian3D
Cell_Cartesian3D::EndVertex()   const { return VertexOnCellIterator(*this,NumOfVertices());}

inline EdgeOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstEdge() const { return EdgeOnCellIterator(*this);}
inline EdgeOnCellIterator_Cartesian3D
Cell_Cartesian3D::EndEdge()   const { return EdgeOnCellIterator(*this, NumOfEdges());}


inline FacetOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstFace()  const { return FacetOnCellIterator(*this);}
inline FacetOnCellIterator_Cartesian3D
Cell_Cartesian3D::EndFace()    const { return FacetOnCellIterator(*this, NumOfFaces());}

inline FacetOnCellIterator_Cartesian3D
Cell_Cartesian3D::FirstFacet() const { return FacetOnCellIterator(*this);}
inline FacetOnCellIterator_Cartesian3D
Cell_Cartesian3D::EndFacet()   const { return FacetOnCellIterator(*this, NumOfFacets());}

// inline functions of Facet_Cartesian3D

inline VertexOnFacetIterator_Cartesian3D
Facet_Cartesian3D::FirstVertex() const { return VertexOnFacetIterator(*this);}
inline VertexOnFacetIterator_Cartesian3D
Facet_Cartesian3D::EndVertex()   const { return VertexOnFacetIterator(*this, NumOfVertices());}

inline EdgeOnFacetIterator_Cartesian3D
Facet_Cartesian3D::FirstEdge() const { return EdgeOnFacetIterator(*this);}
inline EdgeOnFacetIterator_Cartesian3D
Facet_Cartesian3D::EndEdge()   const { return EdgeOnFacetIterator(*this, NumOfEdges());}


// inline functions of Edge_Cartesian3D

inline VertexOnEdgeIterator_Cartesian3D
Edge_Cartesian3D::FirstVertex() const  { return VertexOnEdgeIterator(*this);}
inline VertexOnEdgeIterator_Cartesian3D
Edge_Cartesian3D::EndVertex()   const  { return VertexOnEdgeIterator(*this, NumOfVertices());}

}; // namespace cartesian3d

// these must be in  namespace GrAL

// grid_types<>

template<>
struct grid_types<cartesian3d::CartesianGrid3D> 
  : public grid_types_base<cartesian3d::grid_types_Cartesian3D>
{
  typedef grid_type::archetype_type      archetype_type;
  //typedef grid_type::archetype_handle  archetype_handle;
  typedef grid_type::archetype_iterator  archetype_iterator;
  typedef grid_type::archetype_geom_type archetype_geom_type;
  typedef grid_types<archetype_type>     archgt;
};



// element traits

template<>
struct element_traits<cartesian3d::Vertex_Cartesian3D>
  : public element_traits_vertex_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>    consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Edge_Cartesian3D>
  : public element_traits_edge_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Facet_Cartesian3D>
  : public element_traits_facet_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};

template<>
struct element_traits<cartesian3d::Cell_Cartesian3D>
  : public element_traits_cell_base<cartesian3d::CartesianGrid3D>
{
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>  consecutive_tag;
};

 namespace cartesian3d {

 #define gt grid_types<cartesian3d::CartesianGrid3D>


  inline gt::VertexIterator   gral_begin(gt::grid_type const& G, gt::VertexIterator) { return G.FirstVertex();}
  inline gt::VertexIterator   gral_end  (gt::grid_type const& G, gt::VertexIterator) { return G.EndVertex();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::VertexIterator) { return G.NumOfVertices();}

  inline gt::EdgeIterator     gral_begin(gt::grid_type const& G, gt::EdgeIterator)   { return G.FirstEdge();}
  inline gt::EdgeIterator     gral_end  (gt::grid_type const& G, gt::EdgeIterator)   { return G.EndEdge();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::EdgeIterator)   { return G.NumOfEdges();}

  inline gt::FacetIterator    gral_begin(gt::grid_type const& G, gt::FacetIterator)  { return G.FirstFacet();}
  inline gt::FacetIterator    gral_end  (gt::grid_type const& G, gt::FacetIterator)  { return G.EndFacet();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::FacetIterator)  { return G.NumOfFacets();}

  inline gt::CellIterator     gral_begin(gt::grid_type const& G, gt::CellIterator)   { return G.FirstCell();}
  inline gt::CellIterator     gral_end  (gt::grid_type const& G, gt::CellIterator)   { return G.EndCell();}
  inline gt::size_type        gral_size (gt::grid_type const& G, gt::CellIterator)   { return G.NumOfCells();}



// on Edge
  inline gt::VertexOnEdgeIterator   gral_begin(gt::Edge   a, gt::VertexOnEdgeIterator)  { return gt::VertexOnEdgeIterator(a);}
  inline gt::VertexOnEdgeIterator   gral_end  (gt::Edge   a, gt::VertexOnEdgeIterator)  { return gt::VertexOnEdgeIterator(a,2);}
  inline gt::size_type              gral_size (gt::Edge   a, gt::VertexOnEdgeIterator)  { return 2;}

// on Facet
  inline gt::VertexOnFacetIterator  gral_begin(gt::Facet  a, gt::VertexOnFacetIterator) { return a.FirstVertex();}
  inline gt::VertexOnFacetIterator  gral_end  (gt::Facet  a, gt::VertexOnFacetIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Facet  a, gt::VertexOnFacetIterator) { return a.NumOfVertices();}

  inline gt::EdgeOnFacetIterator    gral_begin(gt::Facet  a, gt::EdgeOnFacetIterator) { return a.FirstEdge();}
  inline gt::EdgeOnFacetIterator    gral_end  (gt::Facet  a, gt::EdgeOnFacetIterator) { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Facet  a, gt::EdgeOnFacetIterator) { return a.NumOfEdges();}

// on Cell
  inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator)  { return a.FirstVertex();}
  inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator)  { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator)  { return a.NumOfVertices();}

  inline gt::EdgeOnCellIterator     gral_begin(gt::Cell   a, gt::EdgeOnCellIterator)    { return a.FirstEdge();}
  inline gt::EdgeOnCellIterator     gral_end  (gt::Cell   a, gt::EdgeOnCellIterator)    { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::EdgeOnCellIterator)    { return a.NumOfEdges();}

  inline gt::FacetOnCellIterator    gral_begin(gt::Cell   a, gt::FacetOnCellIterator)   { return a.FirstFacet();}
  inline gt::FacetOnCellIterator    gral_end  (gt::Cell   a, gt::FacetOnCellIterator)   { return a.EndFacet();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::FacetOnCellIterator)   { return a.NumOfFacets();}

#undef gt
 }


} // namespace GrAL 

#endif
