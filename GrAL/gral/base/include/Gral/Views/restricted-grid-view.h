#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Base/grid-function-hash.h"


namespace GrAL {

/*! \brief Provide a view on a grid part defined by a  cell predicate



   \todo implement Edge/Facet if supported by \c GRID
   \todo reuse archetypes from GRID
   \see \ref gridviews module
   \see Test in \ref test-restricted-components-view.C

*/

namespace restricted_grid_view { 
  /*! \defgroup restricted_grid_view_grp  Restricted Grid View
      \brief Provide a view on a grid part defined by a  cell predicate
  
      \ingroup gridviews
  */

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class grid_view;
  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell;
  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_on_cell_iterator;

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view : public archetypes_from_base_grid_types<GT> {

    typedef grid_view<GRID,INSIDE_PRED, GT> grid_type;

    typedef typename  GT::vertex_handle vertex_handle;
    typedef typename  GT::cell_handle   cell_handle;
    typedef vertex<GRID,INSIDE_PRED, GT>                  Vertex;
    typedef cell  <GRID,INSIDE_PRED, GT>                  Cell;
    typedef vertex_iterator<GRID,INSIDE_PRED, GT>         VertexIterator;
    typedef cell_iterator  <GRID,INSIDE_PRED, GT>         CellIterator;
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> VertexOnCellIterator;

    typedef typename GT::dimension_tag dimension_tag;
  };

  /*! \brief The restricted grid view class

    This class is a model of $GrAL Cell-VertexInputGridRange.
    Grid functions are defined for the vertex type.

    \templateparams
    - GRID:
      - Vertex/VertexIterator
      - Cell/CellIterator
      - VertexOnCellIterator
    - INSIDE_PRED
      - decide whether a cell is to be considered inside,
        <tt> bool operator()(Cell) </tt>

       \ingroup restricted_grid_view_grp
       \see Module \ref restricted_grid_view_grp
       \see Tested in \ref test-restricted-grid-view.C 
  */

  template<class GRID, class INSIDE_PRED, class GT>
  class grid_view : public archetypes_from_base<grid_view<GRID, INSIDE_PRED, GT>, GRID, GT>  {
  public:
    typedef GRID        grid_type;
    typedef INSIDE_PRED pred_type;
    typedef GT          basegt;
    
    typedef typename  GT::Vertex               baseVertex;
    typedef typename  GT::Cell                 baseCell;
    typedef typename  GT::CellIterator         baseCellIterator;
    typedef typename  GT::VertexOnCellIterator baseVertexOnCellIterator;
    typedef typename  GT::vertex_handle        vertex_handle;
    typedef typename  GT::cell_handle          cell_handle;
    
    typedef enumerated_vertex_range<GRID>      range_type;
  private:
    ref_ptr<grid_type const>  g;
    ref_ptr<pred_type>        inside_p;
    
    mutable range_type        range;
    mutable bool              vertices_initialized;
    mutable int               num_of_cells;
  public:
    grid_view() {}
    grid_view(grid_type const& gg,
	      pred_type        ins)
    { init(const_ref_to_ref_ptr(gg), copy_to_ref_ptr(ins)); }

    grid_view(ref_ptr<grid_type const> gg,
	      ref_ptr<pred_type>       ins)
    { init(gg, ins);}

    void init(ref_ptr<grid_type const> gg,
	      ref_ptr<pred_type>       ins)
    {
      g = gg;
      inside_p = ins;
      range.set_grid(g);
      vertices_initialized = false;
      num_of_cells = -1;
    }

    unsigned dimension() const { return g->dimension();}
  public:
    typedef  vertex         <GRID,INSIDE_PRED, GT>  Vertex;
    typedef  vertex_iterator<GRID,INSIDE_PRED, GT>  VertexIterator;
    typedef  cell           <GRID,INSIDE_PRED, GT>  Cell;
    typedef  cell_iterator  <GRID,INSIDE_PRED, GT>  CellIterator;

    VertexIterator FirstVertex()   const { init_vertices(); return VertexIterator(*this);}
    VertexIterator EndVertex()     const { init_vertices(); return VertexIterator(*this, range.EndVertex());}
    unsigned       NumOfVertices() const { init_vertices(); return range.NumOfVertices();}

    CellIterator   FirstCell()     const { return CellIterator(*this);}
    CellIterator   EndCell()       const { return CellIterator(*this, BaseGrid().EndCell());}
    unsigned       NumOfCells()    const { if(num_of_cells < 0) init_num_cells(); return num_of_cells;}

    grid_type const& BaseGrid() const { return *g;}
    grid_type const& TheGrid()  const { return *g;}
    pred_type const& pred()     const { return *inside_p;} 
    bool             inside(baseCell const& c)   const { return (*inside_p)(c);}   
  private:
    void init_num_cells()    const;
    void init_vertices() const;

    friend class restricted_grid_view::vertex_iterator<GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::vertex         <GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::cell_iterator  <GRID,INSIDE_PRED, GT>;

  };





  //--- Vertex ---

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex {
    typedef vertex<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>       grid_type;
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::baseVertex        base_element_type;
    typedef typename grid_type::vertex_handle     vertex_handle;
  private:
    grid_type   const*  g;
    baseVertex          v;
  public:
    vertex() : g(0) {}
    vertex(grid_type const& gg) 
      : g(&gg)
      {
	g->init_vertices();
	v = * g->range.FirstVertex();
      }
    vertex(grid_type const& gg, vertex_handle vv) 
      : g(&gg)
      {
	g->init_vertices();
	v = baseVertex(g->BaseGrid(), vv);
      }
    vertex(grid_type const& gg, baseVertex vv) 
      : g(&gg)
      {
	g->init_vertices();
	v = vv;
      }

    operator baseVertex() const { return v;}
    baseVertex Base() const { return v;}
    vertex_handle handle() const { return v.handle();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };

  //--- VertexIterator ---

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_iterator {
    typedef vertex_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::range_type               range_type;
    typedef typename range_type::VertexIterator          rangeVertexIterator; 
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::Vertex            Vertex;
    typedef typename grid_type::vertex_handle     vertex_handle;
  private:
    grid_type   const*  g;
    rangeVertexIterator v;
  public:
    vertex_iterator() : g(0) {}
    vertex_iterator(grid_type const& gg)
      : g(&gg)
      {
	v = g->range.FirstVertex();
      }
    vertex_iterator(grid_type const& gg, rangeVertexIterator vv)
      : g(&gg), v(vv) 
      {
      }
   

    self& operator++() { ++v; return *this;}
    Vertex operator*() const { return Vertex(*g, handle());}
    operator baseVertex() const { return *v;}
    baseVertex Base() const { return *v;}
    bool  IsDone() const { return v.IsDone();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    vertex_handle handle() const { return v.handle();}

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  };


  //--- Cell ---
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell {
    typedef cell<GRID,INSIDE_PRED, GT> self;
    typedef grid_types_grid_view<GRID,INSIDE_PRED, GT> gt;
    typedef grid_types<typename gt::archetype_type> archgt;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::baseCell        base_element_type;
    typedef typename grid_type::cell_handle     cell_handle;
    typedef typename grid_type::baseCellIterator baseCellIterator;
    //   typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;
 private:
    grid_type   const*  g;
    baseCell            c;
  public:
    cell() : g(0) {}
    cell(grid_type const& gg, baseCell cc = * gg.BaseGrid().FirstCell() ) 
      : g(&gg), c(cc) {}

    operator baseCell() const { return c;}
    baseCell  Base() const { return c;}
    grid_type const& TheGrid() const { return *g;} 
    cell_handle handle() const { return c.handle();}
    unsigned NumOfVertices() const { return Base().NumOfVertices();}
    vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> FirstVertex() const;

    vertex<GRID,INSIDE_PRED, GT>  V(typename archgt::Vertex v) const { 
      return vertex<GRID,INSIDE_PRED, GT>(TheGrid(), Base().V(v)); 
    }
    
    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };



  //--- CellIterator ---
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell_iterator {
    typedef cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::cell_handle     cell_handle;
    typedef typename grid_type::baseCellIterator baseCellIterator;
    //   typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;
 private:
    ref_ptr<grid_type   const>  g;
    baseCellIterator    c;
  public:
    cell_iterator() : g(0) {}
    cell_iterator(grid_type const& gg) 
      : g(gg)
      {
	c = g->BaseGrid().FirstCell();
	advance_till_valid();
      }
    cell_iterator(grid_type const& gg, baseCellIterator cc) : g(gg), c(cc) {}

    self& operator++() { ++c; advance_till_valid();  return *this;}
    cell<GRID,INSIDE_PRED, GT>  operator*() const { return cell<GRID,INSIDE_PRED, GT>(*g,*c); }
    operator baseCell() const { return *c;}
    baseCell  Base() const { return *c;}
    bool  IsDone() const { return c.IsDone();}
    grid_type const& TheGrid() const { return *g;} 
    cell_handle handle() const { return c.handle();}
    unsigned NumOfVertices() const { return Base().NumOfVertices();}
    vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> FirstVertex() const;
    
    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  private:
    void advance_till_valid() {
      while(! IsDone() && ! g->inside(*c))
	++c;
    }
  };



  //--- VertexOnCellIterator ---
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_on_cell_iterator {
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::baseVertexOnCellIterator baseVertexOnCellIterator;
    typedef typename grid_type::vertex_handle            vertex_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;
  private:
    Cell c;
    baseVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator()  {}
    vertex_on_cell_iterator(Cell const& cc) : c(cc), vc(c.Base()) {}

    self& operator++() { ++vc; return *this;}
    Vertex operator*() const { return Vertex(TheGrid(),handle());}
    bool  IsDone() const { return vc.IsDone();}
    grid_type const& TheGrid() const { return c.TheGrid();} 
    vertex_handle handle() const { return vc.handle();}

    bool operator==(self const& rhs) const { return c == rhs.c && vc == rhs.vc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  };


  template<class GRID, class INSIDE_PRED, class GT>
  inline
  vertex_on_cell_iterator<GRID, INSIDE_PRED, GT>
  cell<GRID, INSIDE_PRED, GT>::FirstVertex() const 
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(*this);}







} // namespace restricted_grid_view


  /*! \brief Specialization of grid_types template
       \ingroup restricted_grid_view_grp
       \see Module \ref restricted_grid_view_grp

  */ 
template<class GRID, class INSIDE_PRED, class GT>
struct grid_types<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
  : public grid_types_base<restricted_grid_view::grid_types_grid_view<GRID,INSIDE_PRED,GT> >
{};

template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT> >
  : public element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};


template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_view::cell<GRID,INSIDE_PRED,GT> >
  : public element_traits_cell_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_cell_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};




  /*! \brief Vertex grid function specialization 
 
       This models $GrAL TotalGridFunction on restricted_grid_view::grid_view
       and uses hash maps.

      \relates restricted_grid_view_grp
      \ingroup  restricted_grid_view_grp
      \see \ref restricted_grid_view_grp
  */
template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>  base;
  typedef restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};

  /*! \brief Cell grid function specialization 
 
       This models $GrAL TotalGridFunction  on restricted_grid_view::grid_view
       and uses hash maps.

      \relates restricted_grid_view_grp
      \ingroup  restricted_grid_view_grp
      \see \ref restricted_grid_view_grp
  */
template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>  base;
  typedef restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "restricted-grid-view.C"
#endif

#endif
