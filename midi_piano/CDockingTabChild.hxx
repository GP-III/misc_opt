# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CDOCKINGTABCHILD_INCLUED_HEADER_CXX_)
#  define _CDOCKINGTABCHILD_INCLUED_HEADER_CXX_

# include "stdwtl.h"
# include "CDockingTabContainer.hxx"

namespace dockwins
{
template < class T,
         class TBase = CWindow,
         class TDockingWinTraits = COutlookLikeExDockingContainerWindowTraits>
class ATL_NO_VTABLE CDockingTabChild : public dockwins::CBoxedDockingWindowImpl< T,
                TBase, TDockingWinTraits >   
{
  // CDockingTabChild (){}
  //~CDockingTabChild(){}
};
}//namespace dockwins

# endif // !defined(_CDOCKINGTABCHILD_INCLUED_HEADER_CXX_)