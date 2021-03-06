#ifndef NMWR_GB_OVERLAP_RANGE_INPUT_C
#define NMWR_GB_OVERLAP_RANGE_INPUT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

template<class E>
template<class OVLPRANGE, class FILTER>
void overlap_range_input<E>::copy_to(OVLPRANGE      & dest,
                                     FILTER    const& f) const
{ 
  // init();
  REQUIRE_ALWAYS( ( (in != 0) && (in)), "Invalid stream!\n",1);
  dest.TheContainer().reserve(size());
  unsigned cnt = 0; 
  for(; cnt < npriv; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_exposed_begin();
  for(; cnt < npriv+nexposed; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_shared_begin();
  for(; cnt < npriv+nexposed+nshared; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
  dest.set_copied_begin();
  for(; cnt < npriv+nexposed+nshared+ncopied; ++cnt) {
    value_type v;
    (*in) >> v;  
    dest.push_back(f(v));
  }
}

} // namespace GrAL 

#endif
