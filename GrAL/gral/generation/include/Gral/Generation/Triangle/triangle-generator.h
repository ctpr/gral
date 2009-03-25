#ifndef GRAL_MG_TRIANGLE_GENERATOR_H
#define GRAL_MG_TRIANGLE_GENERATOR_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Generation/Triangle/triangle-input.h"
#include "Gral/Generation/Triangle/triangle-adapter.h"
#include "Gral/Generation/Triangle/TriangleOptionsParser.h"
#include <stdio.h>

namespace GrAL {

namespace triangle_generator {

using triangle_adapter::TriangleAdapter;
using triangle_adapter::free_input_triangulateio;
using triangle_adapter::free_output_triangulateio;


class NotInvertibleError : public std::exception {
  public:
    NotInvertibleError() : det(0.0) {};
    NotInvertibleError(double d) : det(d) {}
    virtual ~NotInvertibleError() throw() {}

    virtual const char* what() const throw () {
       snprintf(msg, 63,"Transformation not invertible, det=%18.14e", det);
       return msg;  
    }
  private:
    mutable char msg[64];
    double det;
};

class TriangleGenerator {
  public:

     inline TriangleGenerator(); 
     virtual inline ~TriangleGenerator();
     // Copy constructor is a default copy constructor generated by compiler
      
     // assignment operator is a default assignment operator generated by
     // compier
     
     void Triangulate(TriangleInput & input, const char *flags, 
                      TriangleAdapter &output);

     void Triangulate(TriangleInput & input, 
                      TriangleAdapter &output);

     const char* GetOptions(); 
     void SetOptions(const char *str) { _options.Parse(str);}

     std::string Info(const char* flags) { TriangleOptionsParser p;
                                           return p.Info(flags); }
  
     bool GetOption(const std::string name) { return _options.GetOption(name);}
     void SetOption(const std::string name, bool v) {
       _options.SetOption(name,v);
     }
  
     double GetMinAngle() const { return _options.GetMinAngle();}
     void SetMinAngle(const double angle) { _options.SetMinAngle(angle);}
  
     double GetMaxArea() const {return _options.GetMaxArea();}
     void SetMaxArea(const double area) { _options.SetMaxArea(area);}

     virtual int triunsuitable(const double *pa, const double *pb, const 
                               double *pc, const double area) const {
         return 0; // do not impose user constraints
     }

     virtual void  incircle_transform(const double *apa, const double *apb, 
                 const double *apc, const double *apd, 
                 double *pa, double *pb, double *pc, double *pd,
                 double *inverse) const {
          // identity transformation 
          pa[0] = apa[0];  pa[1] = apa[1]; 
          pb[0] = apb[0];  pb[1] = apb[1]; 
          pc[0] = apc[0];  pc[1] = apc[1]; 
          pd[0] = apd[0];  pd[1] = apd[1]; 
          if (inverse) {
            inverse[0]=1.0;
            inverse[1]=0.0;
            inverse[2]=0.0;
            inverse[3]=1.0;
          }
     }

  private:
     typedef TriangleInput::vertex_iterator vertex_iterator;
     typedef TriangleInput::segment_iterator segment_iterator;
     typedef TriangleInput::hole_iterator hole_iterator;
     typedef TriangleInput::region_iterator region_iterator;

     TriangleOptionsParser _options;
     char options_string[100];

     bool use_vertex_markers, use_vertex_params;

     void setup_vertex_coords(TriangleInput &input,
                              struct ::triangulateio *t);
     void setup_vertex_params(TriangleInput &input,
                              struct ::triangulateio *t);
     void setup_vertex_markers(TriangleInput & input,
                              struct ::triangulateio *t);
     void setup_segments(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_holes(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_regions(TriangleInput & input,
                              struct ::triangulateio *in);
     void setup_input(TriangleInput & input,
                              struct ::triangulateio *t);

     void init_triangulateio(::triangulateio *t);
 
     void triangulate_impl(TriangleInput & input, 
                      TriangleAdapter &output);
};

inline
TriangleGenerator::TriangleGenerator() : use_vertex_markers(true), 
                                         use_vertex_params(true) {
  //empty
}

inline
TriangleGenerator::~TriangleGenerator() {
  // empty
}

template <class TriangleAccepter, class IncircleMapper>
class TriangleGeneratorPro : public TriangleGenerator {
   public : 
   TriangleGeneratorPro() {}
   TriangleGeneratorPro(TriangleAccepter const& accepter, 
                        IncircleMapper const &mapper) 
     : accepter_(accepter), mapper_(mapper) {}

   void SetTriangleAccepter(TriangleAccepter const& accepter) {
       accepter_ = accepter;
   }

   void SetIncircleMapper(IncircleMapper const & map) {
       mapper_ = map;
   }
   TriangleAccepter const & FetTriangleAccepter() const {return accepter_; }

   IncircleMapper const &GetIncircleMapper() const {return mapper_;}

   int triunsuitable(const double *pa, const double *pb, const 
                               double *pc, const double area) const {
      return accepter_(pa, pb, pc, area); 
   }

   void  incircle_transform(const double *apa, const double *apb, 
                 const double *apc, const double *apd, 
                 double *pa, double *pb, double *pc, double *pd,
                 double *inverse) const {
      IncircleMapper *m = const_cast<IncircleMapper *>(&mapper_);
      m->operator()(apa, apb, apc, apd, pa, pb, pc, pd, inverse); 
   }
   private:
      TriangleAccepter accepter_;
      IncircleMapper mapper_;
};


class AcceptAllTriangles {
  public:
    AcceptAllTriangles() {}
   ~AcceptAllTriangles() {};
    int  operator() (const double *pa, const double *pb, const double *pc,
                     const double area) const {
      return 0;
    }
};

class IdentityMapper {
  public:
    IdentityMapper() {}
   ~IdentityMapper() {};

    void operator() (const double *apa, const double *apb, const double *apc,
                     const double *apd, double *pa, double *pb, double *pc,
                     double *pd, double* inverse=NULL) const {
      pa[0] = apa[0];  pa[1] = apa[1]; 
      pb[0] = apb[0];  pb[1] = apb[1]; 
      pc[0] = apc[0];  pc[1] = apc[1]; 
      pd[0] = apd[0];  pd[1] = apd[1]; 
      if (inverse) {
        inverse[0] = 1.0;
        inverse[1] = 0.0;
        inverse[2] = 0.0;
        inverse[3] = 1.0;
      }
    }
};

class ConstEllipseMapper {
  public:
    ConstEllipseMapper() {
      SetTransformation(1.0, 1.0, 1.0, 0.0); 
    }
    ConstEllipseMapper(const double *lambda, const double *e) {
      SetTransformation(lambda, e); 
    }
    ConstEllipseMapper(double lambda1, double lambda2, 
                            double e1, double e2) {
      SetTransformation(lambda1, lambda2, e1, e2);  
    }
    // + default copy constructor
    // + default assignment operator
    
    void SetTransformation(const double *lambda, const double *e);
    
    void SetTransformation(double lambda1, double lambda2, 
                           double e1, double e2);

    const double*  GetEigenValues() const {return lambda;}
    const double*  GetEigenDir() const {return e;}
    
   ~ConstEllipseMapper() {};

    void operator() (const double *apa, const double *apb, const double *apc,
                     const double *apd, double *pa, double *pb, double *pc,
                     double *pd, double *inverse) const {
       transform(apa, pa);
       transform(apb, pb);
       transform(apc, pc);
       transform(apd, pd); 
       if (inverse) {
         inverse[0] = m_inv[0];
         inverse[1] = m_inv[1];
         inverse[2] = m_inv[2];
         inverse[3] = m_inv[3];
         //memmove(inverse, m_inv, 4*sizeof(double));
       }
    }
  private:
    void transform(const double *x, double *y) const {
       y[0] = m[0]*x[0] + m[1]*x[1];
       y[1] = m[2]*x[0] + m[3]*x[1];
    }
    void set_transformation(); 
      
    double lambda[2];
    double e[2];
    double m[4];
    double m_inv[4];
};

} // namespace triangle_generator
} // namespace GrAL

#endif /* ifndef GRAL_MG_TRIANGLE_GENERATOR_H */
