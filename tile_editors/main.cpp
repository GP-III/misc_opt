////////////////////////////////////////
//   simple tile-map editors  - moecmks
//   mails:sosicklove@yahoo.com
////////////////////////////////////////

////////////////////////////////////////
//   I AM NOT GOOD AT USING C + +  :(
////////////////////////////////////////

# include "precompiled.h"
# include <stdint.h>
# include <assert.h>

// shared global struct .
class cs_mainFrame;
class cs_ViewStatus;
class cs_ViewGlobalZoom;
class cs_ViewRootTexture;
class cs_ViewClaySet;
class cs_ViewPixel;
class cs_PageTile;
class cs_ClayTile;

class shared_cs {

public:
  class cs_mainFrame *cs_mainFrame_;
  class cs_ViewStatus *cs_ViewStatus_;
  class cs_ViewGlobalZoom *cs_ViewGlobalZoom_;
  class cs_ViewRootTexture *cs_ViewRootTexture_;
  class cs_ViewClaySet *cs_ViewClaySet_;
  class cs_ViewPixel *cs_ViewPixel_;
  class cs_PageTile *cs_PageTile_;
  class cs_ClayTile *cs_ClayTile_;

public:
  shared_cs () {
    cs_mainFrame_ = nullptr;
    cs_ViewStatus_ = nullptr;
    cs_ViewGlobalZoom_ = nullptr;
    cs_ViewRootTexture_ = nullptr;
    cs_ViewClaySet_ = nullptr;
    cs_ViewPixel_ = nullptr;
    cs_PageTile_ = nullptr;
    cs_ClayTile_ = nullptr;
  }
 ~shared_cs () {
    cs_mainFrame_ = nullptr;
    cs_ViewStatus_ = nullptr;
    cs_ViewGlobalZoom_ = nullptr;
    cs_ViewRootTexture_ = nullptr;
    cs_ViewClaySet_ = nullptr;
    cs_ViewPixel_ = nullptr;
    cs_PageTile_ = nullptr;
    cs_ClayTile_ = nullptr;
  }
};

namespace common {
  static void fillImg (wxImage &c, char r, char g, char b) {
    wxSize s = c.GetSize ();
    for (int y = 0; y != s.y; y++)  {
      for (int x = 0; x != s.x; x++)  {
        c.SetRGB (x, y, r, g, b);
      }
    }
  }
  static void fillImg (wxImage &c, wxColour &s) {
    fillImg (c, (char )s.Red(), (char )s.Green(), (char )s.Blue());
  }
  static void getMousePos (wxWindow *wd, wxPoint & pts)  {
    wxPoint pt = wxGetMousePosition ();
    pts.x = pt.x - wd->GetScreenPosition ().x;
    pts.y = pt.y - wd->GetScreenPosition ().y;
  } 
  static void getAbsRect2 (wxRect &pe, wxPoint &p1, wxPoint &p2)  {

    int pBigger = p2.x;
    int pSmaller = p1.x;

    if (p1.x > p2.x) {
      pBigger = p1.x;
      pSmaller = p2.x;
    }
    pe.x = pSmaller &-8;
    pe.width = ( pBigger+ 7) & -8; // XXX: stop around 8.
    pe.width = pe.width - pe.x;

    pBigger = p2.y;
    pSmaller = p1.y;

    if (p1.y > p2.y) {
      pBigger = p1.y;
      pSmaller = p2.y;
    }
    pe.y = pSmaller &-8;
    pe.height = ( pBigger+ 7) & -8; // XXX: stop around 8.
    pe.height = pe.height - pe.y;
  }
  static void limitBoard (wxPoint & pt) {
    pt.x &= -8;
    pt.y &= -8;
  }
  static void getRound (wxPoint & pt) {
    pt.x >>= 3;
    pt.y >>= 3;
  }
  static void getRound (wxSize & se) {
    se.x >>= 3;
    se.y >>= 3;
  }
   static void setExpand (wxPoint & pt) {
    pt.x <<= 3;
    pt.y <<= 3;
  }
    static void getMiniOffset (wxPoint & pt) {
    pt.x &= 7;
    pt.y &= 7;
  }
  static void limitBoard (wxRect & pt) {
    pt.x &= -8;
    pt.y &= -8;
  }
  static void getRound (wxRect & pt) {
    pt.x >>= 3;
    pt.y >>= 3;
  }
  static void setExpand (wxRect & pt) {
    pt.x <<= 3;
    pt.y <<= 3;
  }
  static void getMiniOffset (wxRect & pt) {
    pt.x &= 7;
    pt.y &= 7;
  }
};

struct imgSlot {
  bool enable;
  wxString *strPath;
  wxBitmap *img;
  wxImage *img2;
  wxUint8 id;
  
  imgSlot () {
    init ();
  }
  void init () {
    enable = false;
    strPath = nullptr;
    img2 = nullptr;
    img = nullptr;
    id = 0xFF;
  }
  void uninit () {
    if (strPath != nullptr) delete strPath;
    if (img != nullptr) delete img;
    if (img2 != nullptr) delete img2;
    enable = false;
    id = 0xFF;
    strPath = nullptr;
    img = nullptr;
    img2 = nullptr;
  }
  ~imgSlot() {
    uninit ();
  }
  static const int image_size_w = 128;
  static const int image_size_h = 128;
};

struct tileChunk {
  bool enable;
  struct imgSlot *slot;
  wxUint8 idInSlot;
  
  tileChunk () {
    init ();
  }
  void init () {
    enable = false;
    slot = nullptr;
    idInSlot = 0xFF;
  }
};

struct tileChunkMap {
  struct tileChunk map[32*32];
  class wxString *desc ;
  char Rbg;
  char Gbg;
  char Bbg; 
  tileChunkMap () {
    init ();
  }
  ~ tileChunkMap () {
    if (desc != nullptr) delete desc;
    desc = nullptr;
  }
  void init () {
    wxUint16 id;
    for (id = 0; id != sizeof (map)/ sizeof (map[0]); id++)
    { map[id].init(); desc = nullptr; }
  }
  void copy (struct tileChunkMap & t)  {
    wxUint16 id;
    for (id = 0; id != sizeof (map)/ sizeof (map[0]); id++)
    { map[id].enable  = t.map[id].enable; 
      map[id].idInSlot  = t.map[id].idInSlot; 
      map[id].slot  = t.map[id].slot; 
    }
  }
  void copy (struct tileChunkMap & t, imgSlot *p, wxUint8 land)  {
    wxUint16 id;
    for (id = 0; id != sizeof (map)/ sizeof (map[0]); id++)
    { map[id].enable  = (p == nullptr) ? false : p->enable; 
    map[id].idInSlot  = land; 
    map[id].slot  = p; 
    }
  }
};

class cs_mainFrame : public wxFrame {
    
public:
  enum wxRESID {
    wxID_LOAD_TEXTURE = 1005,
    wxID_LOAD_SETTINGS,
    wxID_SAVE_SETTINGS,
    wxID_PAGE_SETTINGS,
    wxID_PAGE_NEW,
    wxID_PAGE_CLOSE,
    wxID_PAGE_SAVE,
    wxID_CLAY_ADD,
    wxID_GVIEW,
    wxID_BTEXTURE,
    wxID_CLAY,
    wxID_STATUS_OUTPUT,
    wxID_PIXEL,
    wxID_ABOUT_WXWIDGETS,
    wxID_ABOUT_EDITORS
  };

public:

  cs_mainFrame (wxWindow* parent);
  ~cs_mainFrame()
  {
    // deinitialize the frame manager
    aui_Manager.UnInit();
  }

public:
  // event mapper .
  void OnFile_LoadImage (class wxCommandEvent& event);
  void OnFile_ClayAdd (class wxCommandEvent& event);
  void OnFile_PageNew  (class wxCommandEvent& event);
  void OnFile_PageSettings  (class wxCommandEvent& event);
  void OnFile_LoadSettings  (class wxCommandEvent& event);
  void OnFile_SaveSettings  (class wxCommandEvent& event);
  void OnFile_PageSave  (class wxCommandEvent& event);

  void OnAbout_wxWidgets (class wxCommandEvent& event);  
  void OnAbout_TileMap (class wxCommandEvent& event); 

  void OnView_StatusOutput  (class wxCommandEvent& event);
  void OnView_Close  (wxAuiManagerEvent& event);
  void OnView_BTexture  (class wxCommandEvent& event);
  void OnView_Clay  (class wxCommandEvent& event);

  void StatusShowText ();
  void StatusOut (wxString & content);
  void StatusShowClearAppendBaseTextSetTemp (wxString &s);
  void StatusOutWrap (wxString & content);
  void StatusOutWrapShow (wxString & content);

  // manger solt.
  // on pattern panel. only adjust. bank.
  struct imgSlot imgSlot_[256];
  std::list <tileChunkMap *> fmset;
  struct tileChunkMap tps;
public:
  class shared_cs share_cs_;
  class shared_cs & attach;
  class cs_PageTileTab *ctab;

  wxWindow *fd_mainFrame_;
  wxWindow *fd_ViewStatus_;
  wxWindow *fd_ViewGlobalZoom_;
  wxAuiNotebook *fd_ViewRootTexture_;
  wxAuiNotebook *fd_ViewClaySet_;
  wxWindow *fd_ViewPixel_;

  wxAuiManager aui_Manager;
  wxAuiNotebook *note_tab2;

  wxString statOut;
  struct tile_action_rt *umlink ;
  struct tile_translat *tefcopy;

  wxDECLARE_EVENT_TABLE();
public:
  struct imgSlot *getSlotByID (wxUint8 id_)  {
    for (int id= 0; id != sizeof (imgSlot_)/ sizeof (imgSlot_[0]); id++)  {
      if (imgSlot_[id].enable != false)  {
        if  (imgSlot_[id].id == id_)
          return & imgSlot_[id];
        else ;
      }
    }
    return nullptr  ;
  }
};

wxBEGIN_EVENT_TABLE (cs_mainFrame, wxFrame)  
  EVT_MENU (cs_mainFrame::wxID_LOAD_TEXTURE, cs_mainFrame::OnFile_LoadImage) 
  EVT_MENU (cs_mainFrame::wxID_CLAY_ADD, cs_mainFrame::OnFile_ClayAdd) 
  EVT_MENU (cs_mainFrame::wxID_PAGE_SETTINGS, cs_mainFrame::OnFile_PageSettings)
  EVT_MENU (cs_mainFrame::wxID_LOAD_SETTINGS, cs_mainFrame::OnFile_LoadSettings) 
  EVT_MENU (cs_mainFrame::wxID_SAVE_SETTINGS, cs_mainFrame::OnFile_SaveSettings) 
  EVT_MENU (cs_mainFrame::wxID_PAGE_SAVE, cs_mainFrame::OnFile_PageSave) 
  EVT_MENU (cs_mainFrame::wxID_CLAY, cs_mainFrame::OnView_Clay) 
  EVT_MENU (cs_mainFrame::wxID_ABOUT_WXWIDGETS, cs_mainFrame::OnAbout_wxWidgets)  
  EVT_MENU (cs_mainFrame::wxID_ABOUT_EDITORS, cs_mainFrame::OnAbout_TileMap) 
  EVT_MENU (cs_mainFrame::wxID_PAGE_NEW, cs_mainFrame::OnFile_PageNew)
  EVT_MENU (cs_mainFrame::wxID_STATUS_OUTPUT, cs_mainFrame::OnView_StatusOutput)
  EVT_MENU (cs_mainFrame::wxID_BTEXTURE, cs_mainFrame::OnView_BTexture)
  EVT_AUI_PANE_CLOSE (cs_mainFrame::OnView_Close)
wxEND_EVENT_TABLE () 

struct tileChunkSet {
  wxUint8 hnums;
  wxUint8 vnums;
  struct tileChunkMap *tcm;
  class wxString desc;
};

/* namespace*/
struct tile_file {
  struct dummyroot {
    char rgroup[imgSlot::image_size_w*imgSlot::image_size_h];
    char ggroup[imgSlot::image_size_w*imgSlot::image_size_h];
    char bgroup[imgSlot::image_size_w*imgSlot::image_size_h];
    char unused[imgSlot::image_size_w*imgSlot::image_size_h];
  };
  struct std_infos {
    wxUint8 mg[4];
    wxUint16 imgSlot_nums;
    wxUint16 claySet_nums;
    wxUint16 pageSet_nums;
    wxUint32 claySetDesc_Offset; // offset base byte. not ch encode.
    wxUint32 claySetChunk_Offset;
    wxUint32 pageSetDesc_Offset; // offset base byte. not ch encode.
    wxUint32 pageSetChunk_Offset;
  };
  struct RVA_uv_chunk {
    wxUint32 offset;
    wxUint32 length;
  };
  struct RVA_np_chunk {
    wxUint32 ch_offset;
    wxUint32 ch_length;
    wxUint32 pg_Width;
    wxUint32 pg_Height;
  };
  struct RVA_kc_chunk {
    wxUint8 enable;
    wxUint8 page;
    wxUint8 land;
  };
  struct RVA_as_chunk {
    struct RVA_kc_chunk kcset[32*32];
  };
  static void maptoas (RVA_as_chunk &as, tileChunkMap &cm) {
    for (int y = 0; y != 32; y++)  {
      for  (int x = 0; x != 32; x++) {
        int k = x+y*32;
        as.kcset[k].enable = cm.map[k].enable;
        if  (as.kcset[k].enable  != false)  {
          as.kcset[k].page = cm.map[k].slot->id;
          as.kcset[k].land = cm.map[k].idInSlot;
        }
      }
    }
  }

  static void astomap (tileChunkMap &cm, RVA_as_chunk &as, class cs_mainFrame &csm) {
    for (int y = 0; y != 32; y++)  {
      for (int x = 0; x != 32; x++)  {
        int k = y*32+x;
        cm.map[k].enable = !! as.kcset[k].enable;
        if  (cm.map[k].enable != false) {
          cm.map[k].idInSlot = as.kcset[k].land;
          cm.map[k].slot = csm. getSlotByID (as.kcset[k].page);
        }
      }
    }
  }
  // -------------------------------------------------------
  // file struct desc . 
  // 
  // dword    magic number. assert file vailed.
  // word     imgSlot nums. [max : 256]
  // word     clayset nums  [max : word max ]
  // word     page set nums 
  // dword    clay desc ch heap offset 
  // dword    clay chunk heap offset 
  // dword    page desc ch heap offset 
  // dword    page chunk heap offset  
  // -------------------------------------------------------
  // byte*?     vailed id count[imgSlot nums]
  // dummyroot*? imgSlot RGB pixel set [imgSlot nums] 
  // -------------------------------------------------------
  // RVA_uv_chunk*?     desc struct [clayset nums]
  // ucs-2 buffer    clay desc set
  // RVA_as_chunk*?      as chunk set [clayset nums]
  // -------------------------------------------------------
  // RVA_np_chunk*?     desc struct [page nums]
  // ucs-2 buffer    page desc set
  // RVA_as_chunk* *?      as chunk set [page nums]
  // -------------------------------------------------------
};

// AUI-WINDOW status output.
class  cs_ViewStatus : public wxTextCtrl {

public:
  cs_ViewStatus (class shared_cs &attach_, class wxWindow *parent, wxWindowID id, 
    const wxString &title, 
    const wxPoint &point,
    const wxSize &size,
    long style ) : wxTextCtrl (parent, id, title, point, size, style), attach (attach_)
  { attach.cs_ViewStatus_ = this; }

  class shared_cs & attach;
};



// read edit 
struct tile_action_rt {
  bool collect;
  bool stop;
  void set_discard () {
    collect = false;
    stop = false;
  }
  void init () {
    set_discard ();
  }
  bool cando () {
    return ((collect == false) && (stop == true));
  }
  bool canselectOrpt () {
    return ((collect != false) || (stop != false));
  }
};

struct tile_translat {

  enum CBG_fromtype {
    CBG_FROM_UNSET = -1,
    CBG_FROM_ROOTTEXTURE = 0,
    CBG_FROM_CLAY,
    CBG_FROM_PPSET
  };
  struct tileChunkSet *pSet;
  enum CBG_fromtype type;
  class wxRect mapRect;
  union  {
    struct imgSlot *slot_;
    struct tileChunkMap *map_;
  };
};

// <CLAY-TILE> =====================================================================
class cs_ClayTile : public wxAuiNotebook {

public:
  cs_ClayTile (class shared_cs & attach_, wxWindow *parent) 
    : wxAuiNotebook (parent), attach (attach_) 
  { attach.cs_ClayTile_ = this; }
  
  class shared_cs & attach;
};

class cs_ClayTileTab : public wxNotebookPage {

public:
  cs_ClayTileTab (class shared_cs & attach_, wxWindow *parent, wxWindowID id, int vnums, int hnums, wxString & caption) 
    : wxNotebookPage (parent, id), attach (attach_) {}
public:
  class shared_cs & attach;
  tileChunkSet *chunkset;
};

// <PAGE-TILE> =====================================================================
class cs_PageTile : public wxAuiNotebook {

public:
  cs_PageTile (class shared_cs & attach_, wxWindow *parent) 
    : wxAuiNotebook (parent), attach (attach_) 
  {
    attach.cs_PageTile_ = this;
    this->Connect (wxEVT_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler (cs_PageTile::OnAui_page_changed));
    this->Connect (wxEVT_AUINOTEBOOK_PAGE_CLOSED, wxAuiNotebookEventHandler (cs_PageTile::OnAui_page_closed));
  }
  void cs_PageTile::OnAui_page_changed (wxAuiNotebookEvent &event)  {
    attach.cs_mainFrame_->ctab = (class cs_PageTileTab *) this->GetPage(event.GetSelection ());
  }
  void cs_PageTile::OnAui_page_closed (wxAuiNotebookEvent &event)  {

    attach.cs_mainFrame_->ctab = (class cs_PageTileTab *) this->GetCurrentPage ();
  }

public:
  class shared_cs & attach;
};

// main tab.
class cs_PageTileTab : public wxNotebookPage {

public:
  cs_PageTileTab (class shared_cs & attach_, wxWindow *parent, wxWindowID id, int hnums, int vnums, wxString & caption, tileChunkSet * pp = nullptr) 
    : wxNotebookPage (parent, id), attach (attach_) 
  {
    // create notepage
    if  (pp == nullptr)  {
      chunkset.hnums = hnums;
      chunkset.vnums = vnums;
      chunkset.tcm = new tileChunkMap [hnums * vnums];
      chunkset.desc = caption.size () > 0 ? caption : wxT ("noname");
    } else {
      chunkset.hnums = pp->hnums;
      chunkset.vnums = pp->vnums;
      chunkset.tcm = new tileChunkMap [pp->hnums * pp->vnums];
      chunkset.desc = pp->desc;
      for (int y = 0; y != chunkset.vnums; y++)  {
        for (int x = 0; x != chunkset.hnums; x++)  {
          int c = y*chunkset.hnums+x;
          chunkset.tcm[c].copy(pp->tcm[c]);
        }
      }
    }
    XorBgColor (chunkset);
    
    this->SetBackgroundColour ( wxColour (0xd8b166));

    wxAuiNotebook *bk = (wxAuiNotebook *) parent;
    bk->AddPage (this, chunkset.desc);
    int switchbank = 0;

    // set scrolls.
    vScr = new wxScrollBar (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL);
    hScr = new wxScrollBar (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);

    dgs.init ();
    SetScrFitSize ();
    SetThumbSize ();

    ptStart.x =
    ptStart.y =
    ptPoll.x =
    ptPoll.y = 0;

    // link message map.
    this->Connect (wxEVT_SIZE, wxSizeEventHandler (cs_PageTileTab::OnSize_Event));
    this->Connect (wxEVT_PAINT, wxPaintEventHandler (cs_PageTileTab::OnPaint_Event));
    this->Connect (wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler (cs_PageTileTab::OnThumbTrackOrRelease_Event));
    this->Connect (wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler (cs_PageTileTab::OnThumbTrackOrRelease_Event));

    this->Connect (wxEVT_MOTION, wxMouseEventHandler (cs_PageTileTab::OnMotion_Event));
    this->Connect (wxEVT_LEFT_DOWN, wxMouseEventHandler (cs_PageTileTab::OnMouseLButtonDown_Event));
    this->Connect (wxEVT_RIGHT_UP, wxMouseEventHandler (cs_PageTileTab::OnMouseRButtonUp_Event));
    this->Connect (wxEVT_RIGHT_DOWN, wxMouseEventHandler (cs_PageTileTab::OnMouseRButtonDown_Event));
  }

  void XorBgColor (tileChunkSet &cs) {
    for ( wxUint16 y = 0; y != cs.vnums; y++)  {
      for ( wxUint16 x = 0; x != cs.hnums; x++)  {
        int n = y;
        int c = x;
        int s = x+y*cs.hnums;
        n &= 1;
        c &= 1;
        c ^= n;
        if (c == 0) {
          cs.tcm[s].Rbg = (char )0x00;
          cs.tcm[s].Gbg = (char )0xC6;
          cs.tcm[s].Bbg = (char )0xFF;
        } else  {
          cs.tcm[s].Rbg = (char )0xFF;
          cs.tcm[s].Gbg = (char )0xAF;
          cs.tcm[s].Bbg = (char )0x02;
        }
      }     
    }
  }
  void XorBgColor () {
    XorBgColor (chunkset);
  }
  void OnThumbTrackOrRelease_Event (wxScrollEvent& event) {
    int curid = event.GetId ();
    outputBaseInfos ();
    if  (curid == vScr->GetId ())  {
      ptStart.y = vScr->GetThumbPosition();
      this->Refresh ();
    } else if (curid == hScr->GetId ())  {
      ptStart.x = hScr->GetThumbPosition();
      this->Refresh ();
    } else  {
      event.Skip ();
    } 
  }

  void SetThumbSize () {
    wxSize u;
    wxSize v;
    if ( GetClientSize2(u) != false)  {
      GetLogicSize2(v);
      hScr->SetRange (v.x);
      hScr->SetThumbSize (u.x);
      vScr->SetRange (v.y);
      vScr->SetThumbSize (u.y);
    } 
  }

  void SetScrFitSize () {

    wxRect mvRect = this->GetClientRect();
    wxRect vdRect (mvRect);
    int q = vdRect.GetRight ();
    int q2 = q - 20;
    int q3 = vdRect.GetBottom ();
    int q4 = q3 - 20;
    vdRect.SetLeft (q2);
    vdRect.SetRight (q);
    vdRect.SetBottom (q4);
    vScr->SetSize (vdRect);

    wxRect hdRect (mvRect);
    q = hdRect.GetBottom ();
    q2 = q - 20;
    q3 = hdRect.GetRight ();
    q4 = q3 - 20;
    hdRect.SetTop (q2);
    hdRect.SetBottom (q);
    hdRect.SetRight (q4);
    hScr->SetSize (hdRect);
  }

  inline 
  struct tileChunk *getTileChunk (int hnumsInList, int vnumsInList, struct tileChunkMap **pp = nullptr)  {
    // chunkset.hnums;
    // 32 32
    //  get vpos.
    int b = vnumsInList >> 5;
    int p = vnumsInList & 31;
    int q = hnumsInList >> 5;
    int d = hnumsInList & 31;
    if (hnumsInList >= (chunkset.hnums <<5)) return nullptr;
    if (vnumsInList >= (chunkset.vnums <<5)) return nullptr;
    struct tileChunkMap * c = & chunkset.tcm[b *chunkset.hnums+q];
    if (pp != nullptr) * pp = c;
    return & c->map[ (p <<5)+d];
  }

  void OnMouseLButtonDown_Event (wxMouseEvent &event) { 
    
    wxPoint pe  = wxGetMousePosition ();
    wxSize es;
    common :: getMousePos (this, pe);
    GetClientSize2 (es);

    if (pe.x >= 0
      && (pe.y >= 0)
      && (pe.x < es.x)
      && (pe.y < es.y))
    {
      wxPoint pg = pe;
      common ::getMiniOffset (pg);

      // fill rect.
      tile_translat *tf = attach.cs_mainFrame_->tefcopy;
      common :: limitBoard (pe);

      if (tf != nullptr)  {
        
        wxPoint sp (tf->mapRect.width, tf->mapRect.height);
        wxPoint ap (tf->mapRect.x, tf->mapRect.y);
        common :: getRound (pe);
        common :: limitBoard (ap);
        common :: getRound (ap);
        common :: getRound (sp);
        wxPoint cc ( hScr->GetThumbPosition () + 8* !! (ptStart.x & 7),
                     vScr->GetThumbPosition () + 8* !! (ptStart.y & 7)  ); // XXX:mini offset incorrent.
        common :: getRound (cc);
        cc += pe;
        for ( int y = 0; y < sp.y; y++)  {
          for (int x = 0; x < sp.x; x++)  {
            if  (tf->type == tile_translat:: CBG_FROM_ROOTTEXTURE)  {
              // to cachemap.
              imgSlot *pq = tf->slot_;
              if (pq->enable == false)
                continue  ;
              wxPoint st (x, y);
              st += cc;
              struct tileChunk *j = getTileChunk (st.x, st.y);
              // tileChunk *p = & map2->map[xq+pe.x+(yq+pe.y)*32];
              if (j != nullptr)  {
                j->slot = pq;
                j->enable = true;
                j->idInSlot = ap.x + x;
                j->idInSlot += ( ap.y + y) * 16;
              }
            } else if (tf->type == tile_translat::CBG_FROM_CLAY)  {
              wxPoint st2 (x, y);
              st2 += ap;
              int sc = st2.x+st2.y*32;
              imgSlot *pq = attach.cs_mainFrame_->tps.map[sc].slot;
              if (pq == nullptr)
                continue  ;
              if (pq->enable == false)
                continue  ;
              wxPoint st (x, y);
              st += cc;
              struct tileChunk *j = getTileChunk (st.x, st.y);
              // tileChunk *p = & map2->map[xq+pe.x+(yq+pe.y)*32];
              if (j != nullptr)  {
                j->slot = pq;
                j->enable = true;
                j->idInSlot = attach.cs_mainFrame_->tps.map[sc].idInSlot;
              }
            }
          }
        }
        this->Refresh ();
      }
    }
  }

  void outputBaseInfos ()  {

    wxPoint s;
    wxSize s2;
    wxString rc;
    common ::getMousePos (this, s); 
    GetClientSize2 (s2);

    rc.Printf (  
      wxT ("M-(x:%d y:%d)|(b8x:%d b8y:%d)|(m8x:%d m8y:%d)")
      wxT (" W-(w:%d h:%d)|(b8w:%d b8h:%d)|(m8w:%d m8h:%d)")
      wxT (" H-(total:%d pos:%d size:%d)")
      wxT (" V-(total:%d pos:%d size:%d)"), 
      // mouse 
      s.x, s.y,
      s.x >> 3, s.y >> 3,
      s.x % 8, s.y % 8,
      // window 
      s2.x, s2.y,
      s2.x >> 3, s2.y >> 3,
      s2.x % 8, s2.y % 8,
      // sh.
      hScr->GetRange (),
      hScr->GetThumbPosition(),
      hScr->GetThumbSize (),
      // sv.
      vScr->GetRange (),
      vScr->GetThumbPosition(),
      vScr->GetThumbSize () );

    attach.cs_mainFrame_->GetStatusBar ()->SetLabelText (rc);
  }

  void OnMouseRButtonDown_Event (wxMouseEvent &event) { event.Skip (); }
  void OnMouseRButtonUp_Event (wxMouseEvent &event) { event.Skip (); }

  void OnMotion_Event (wxMouseEvent &event) { 
    common ::getMousePos (this, ptPoll); 
    outputBaseInfos ();
    this->Refresh ();
  }
  void OnSize_Event (wxSizeEvent &event) {

    SetScrFitSize ();
    SetThumbSize ();
    outputBaseInfos ();
    ptStart.y = vScr->GetThumbPosition();
    ptStart.x = hScr->GetThumbPosition();
    this->Refresh();
  }

  void OnPaint_Event (wxPaintEvent &event) {

    // try scan. 
    // first scan current'size tile base scroll's offset and client size.
    
    class wxPoint s = ptStart;
    class wxPoint s2 = s;
    class wxSize e;

    GetClientSize2 (e);
    common :: getMiniOffset (s2);

    if (e.x <= 0 || e.y <= 0)
      return ;
    else ;

    int needScanHoriNums = e.x / 8 + !! (e.x % 8);
    int nessScanVertNums = e.y / 8 + !! (e.y % 8);

    // check more scan base scroll..
    if (s2.x != 0)   {
      //  exist remain.
      if  (((8 - s2.x) & -8) < ((e.x) & 7)) needScanHoriNums ++;
    }
    if (s2.y != 0)   {
      //  exist remain.
      if  (((8 - s2.y) & -8) < ((e.y) & 7)) nessScanVertNums ++;
    }
    int q = s.x >> 3;
    int g = s.y >> 3;
    q = q + !! s2.x;
    g = g + !! s2.y;

    // dummy draw buffer.
    class wxImage img (needScanHoriNums * 8, nessScanVertNums * 8);
    common :: fillImg(img, 0xFF, 0xFF, 0xFF);
    // img.Clear ();

    // scan base image.
    for (int y = 0; y != nessScanVertNums; y++)  {
      for (int x = 0; x != needScanHoriNums; x++)  {
        tileChunkMap *pp;
        tileChunk *p = getTileChunk (x+q, y+g, & pp);
        if (p == nullptr)
          continue  ;
        if ( p->enable == false)
          goto LL  ;
        imgSlot *k = p->slot;
        if (k == nullptr)
          goto LL  ;
        if (k->enable == false)
          goto LL  ;
        int xs = p->idInSlot & 15;
        int ys = p->idInSlot >> 4;
        xs *= 8;
        ys *= 8;
        for (int u = 0; u != 8; u++)  {
          for  (int v = 0; v != 8; v++)  {
            char R = k->img2->GetRed (xs+v, ys+u);
            char G = k->img2->GetGreen (xs+v, ys+u);
            char B = k->img2->GetBlue (xs+v, ys+u);
            img.SetRGB (x*8+v, y*8+u, R, G, B );
          }
        }
        continue  ;
LL:     
        for (int u = 0; u != 8; u++)  {
          for  (int v = 0; v != 8; v++)  { // 5514fb
            img.SetRGB (x*8+v, y*8+u, pp->Rbg, pp->Gbg, pp->Bbg );
          }
        }
      }
    }

    // alpha rect.
    if ( attach.cs_mainFrame_->tefcopy != nullptr)  {
      if (attach.cs_mainFrame_->umlink != nullptr) {
        if (attach.cs_mainFrame_->umlink->cando() != false)  {
          // check range.
          wxPoint pe;
          common :: getMousePos (this, pe);
          // if (inRange (pe) != false)  {
          if (  1 ) {
            // switch 
            tile_translat *m = attach.cs_mainFrame_->tefcopy;
            //   alpha select rect/.
            switch (m->type)  {
            case  tile_translat::CBG_FROM_CLAY:
              {
                //   iterater.
                for  (int y = 0; y < m->mapRect.height; y+=8)  {
                  for  (int x = 0; x < m->mapRect.width; x+=8)  {
                    //   pick imgSlot .
                    int xm = m->mapRect.x & -8;
                    int xd = xm + x;
                    int ym = m->mapRect.y & -8;
                    int yd = ym + y;
                    xd >>= 3;
                    yd >>= 3;
                    tileChunk *p = & attach.cs_mainFrame_->tps.map[xd+yd*32];
                    imgSlot *pq = p->slot;
                    if (p->enable == false)
                      continue  ;
                    if (pq->enable == false)
                      continue  ;
                    common :: limitBoard (pe);
                    int fx = x + pe.x;
                    int fy = y + pe.y;
                    int tx = p->idInSlot & 0x0F;
                    int ty = p->idInSlot >> 4;
                    tx <<= 3;
                    ty <<= 3;
                    for (int y1 = 0; y1 != 8; y1++)  {
                      for  (int x1 = 0; x1 != 8; x1++)  {
                        // fetch source.
                        int q = tx + x1;
                        int b = ty + y1;
                        int p = fx + x1;
                        int d = fy + y1;
                        if (p >= 0 && (p < e.x)
                          && (d >= 0 && (d < e.y))
                          && (q >= 0 && (q <= 127)) 
                          && (b >= 0 && (b <= 127)) )
                        {
                          char r = pq->img2->GetRed (q, b);
                          char g = pq->img2->GetGreen (q, b);
                          char b2 = pq->img2->GetBlue (q, b);
                          char R = img.GetRed (p, d);
                          char G = img.GetGreen (p, d);
                          char B = img.GetBlue (p, d);
                          // alpha 50% rect.
                          r = wxColour::AlphaBlend (r, R, 0.5);
                          g = wxColour::AlphaBlend (g, G, 0.5);
                          b2 = wxColour::AlphaBlend (b2, B, 0.5);
                          img.SetRGB (p, d, r, g, b2);
                        }
                      }
                    }
                  }      
                }
              }
              break;
            case  tile_translat::CBG_FROM_ROOTTEXTURE:
              {
                //   iterater.
                common :: limitBoard (pe);
                for  (int y = 0; y < m->mapRect.height; y+=8)  {
                  for  (int x = 0; x < m->mapRect.width; x+=8)  {
                    //   pick imgSlot .
                    imgSlot *pq = m->slot_;

                    if (pq->enable == false)
                      continue  ;
                    int tx = x +  m->mapRect.x;
                    int ty = y +  m->mapRect.y;
                    int fx = x + pe.x;
                    int fy = y + pe.y;

                    for (int y1 = 0; y1 != 8; y1++)  {
                      for  (int x1 = 0; x1 != 8; x1++)  {
                        // fetch source.
                        int q = tx + x1;
                        int b = ty + y1;
                        int p = fx + x1;
                        int d = fy + y1;
                        if (p >= 0 && (p < e.x)
                          && (d >= 0 && (d < e.y))
                          && (q >= 0 && (q <= 127)) 
                          && (b >= 0 && (b <= 127)) )
                        {
                          char r = pq->img2->GetRed (q, b);
                          char g = pq->img2->GetGreen (q, b);
                          char b2 = pq->img2->GetBlue (q, b);
                          if ( (p -s2.x) < 0)
                            continue  ;
                          if ( (d - s2.y) < 0)
                            continue  ;
                          char R = img.GetRed (p, d);
                          char G = img.GetGreen (p, d);
                          char B = img.GetBlue (p, d);
                          // alpha 50% rect.
                          r = wxColour::AlphaBlend (r, R, 0.5);
                          g = wxColour::AlphaBlend (g, G, 0.5);
                          b2 = wxColour::AlphaBlend (b2, B, 0.5);
                          img.SetRGB (p, d, r, g, b2);
                        }
                      }
                    }
                  }      
                }
              }
              break ;
            }
          }
        }
      }
    }
    // dummy dc.
    wxBufferedPaintDC dc (this);
    wxMemoryDC mdc;// (wxBitmap (img));
    mdc.SelectObject (wxBitmap (img));
    SetBackgroundStyle (wxBG_STYLE_CUSTOM);
    dc.Clear ();
    wxPoint pt (0, 0);
    wxPoint pt2 (ptStart.x & 7, ptStart.y & 7);
    wxSize ep;
    GetClientSize2 (ep);
    dc.Blit (pt, ep, & mdc, pt2);
    // event.Skip ();
  }

  bool GetClientSize2 (wxSize &se)  {
    wxRect mvRect = this->GetClientRect();
    se.x = mvRect.width -20;
    se.y = mvRect.height-20;

    if (se.x <= 0 || (se.y <= 0))
      return false;
    else  
      return true;
  }
  void GetLogicSize2 (wxSize &se)  {
    se.x = chunkset.hnums * 256;
    se.y = chunkset.vnums * 256;
  }
public:
  class wxScrollBar *hScr;
  class wxScrollBar *vScr;
  class shared_cs & attach;
  struct tileChunkSet chunkset;
  struct tile_action_rt dgs;
  struct tile_translat tec_local;
  class wxPoint ptStart;
  class wxPoint ptPoll;
};

class cs_ViewRootTexture : public wxAuiNotebook {

public:
  cs_ViewRootTexture (class shared_cs & attach_, class wxWindow *parent) 
    : wxAuiNotebook (parent), attach (attach_) 
  { attach.cs_ViewRootTexture_ = this; }

  class shared_cs & attach;
};

// read. page.
class cs_NotePage : public wxNotebookPage {
public:
  cs_NotePage (class shared_cs & cs_, struct imgSlot *imgs, class wxWindow *parent, wxWindowID id = wxID_ANY)
    : attach (cs_), wxNotebookPage (parent, id)
  {
    pc_bank = imgs;
    art.init ();

    this->Connect (wxEVT_PAINT, wxPaintEventHandler (cs_NotePage::OnPaint_Event));
    this->Connect (wxEVT_MOTION, wxMouseEventHandler (cs_NotePage::OnMotion_Event));
    this->Connect (wxEVT_RIGHT_DOWN, wxMouseEventHandler (cs_NotePage::OnMouseRButtonDown_Event));
    this->Connect (wxEVT_RIGHT_UP, wxMouseEventHandler (cs_NotePage::OnMouseRButtonUp_Event));
  }
  void OnPaint_Event (wxPaintEvent& event) {
    class wxBufferedPaintDC dc (this);
    SetBackgroundStyle (wxBG_STYLE_CUSTOM);

    if  (art.canselectOrpt() != false)  {
       // alpha rect. 
      class wxRect trs;
      class wxImage img = pc_bank->img->ConvertToImage();
      common :: getAbsRect2 (trs, ptStart, ptPoll);
      if (trs.x < 0 || trs.y < 0
        || trs.width < 0
        || trs.height < 0)
        return  ;
      // limit range.
      if ( (trs.x + trs.width) > imgSlot::image_size_w) 
        trs.width = imgSlot::image_size_w - trs.x;
      if ( (trs.y + trs.height) > imgSlot::image_size_h) 
        trs.height = imgSlot::image_size_h - trs.y;
      if (attach.cs_mainFrame_->tefcopy == & tec_local)
        tec_local.mapRect = trs;
      // alpha it.
      for  (int y = 0; y < trs.height; y++)  {
        for  (int x = 0; x < trs.width; x++)  {
          int u = x + trs.x;
          int v = y + trs.y;
          if (u >= 0 && u < imgSlot::image_size_w
            && v >= 0 && v < imgSlot::image_size_h)
          {
            char r = img.GetRed (u, v);
            char g = img.GetGreen (u, v);
            char b = img.GetBlue (u, v);
            r = wxColour::AlphaBlend (r, 0x6C, 0.25);
            g = wxColour::AlphaBlend (g, 0x00, 0.25);
            b = wxColour::AlphaBlend (b, 0xFF, 0.25);
            img.SetRGB (u, v, r, g, b);     
          }
        }      
      }
      class wxBitmap gbitmap (img);
      dc.Clear ();
      dc.DrawBitmap (gbitmap, 0, 0);
      return ;
    }
    dc.Clear ();
    dc.DrawBitmap (*pc_bank->img, 0, 0);
  }
  void OnMouseRButtonUp_Event (wxMouseEvent & event) {
    art.stop = true;
    art.collect = false;
  }
  void outputBaseInfos ()  {

    wxPoint s;
    wxString rc;
    common ::getMousePos (this, s); 
    // GetClientSize2 (s2);
    int xi = -1;
    int yi = -1;
    int c = -1;
    if (s.x >= 0 && (s.x <= 127)) 
      xi = s.x >> 3;
    if (s.y >= 0 && (s.y <= 127))
      yi = s.y >> 3;
    if (xi != -1 && yi != -1)
      c = xi + yi * 16;
    rc.Printf (  
      wxT ("PAGE-%02X(%d) ID:-%02X(%d)"), pc_bank->id, pc_bank->id, c, c );
    attach.cs_mainFrame_->GetStatusBar ()->SetLabelText (rc);
  }
  void OnMotion_Event (wxMouseEvent & event){
    outputBaseInfos ();
    if (art.stop == false) {
      common :: getMousePos (this, ptPoll);
      this->Refresh ();
    }
  }
  bool inRange (wxPoint & pt)  {
    bool inRange_X =  (pt.x >= 0) && (pt.y < imgSlot::image_size_w);
    bool inRange_Y =  (pt.y >= 0) && (pt.y < imgSlot::image_size_h);
    return (inRange_Y && inRange_X);
  }
  void OnMouseRButtonDown_Event (wxMouseEvent & event) {
    art.stop = false;
    class wxPoint pe;
    common :: getMousePos (this, pe);

    // ctor tec_local.
    tec_local.type = tile_translat::CBG_FROM_ROOTTEXTURE;
    tec_local.slot_ = pc_bank;

    // check range.
    if (inRange (pe) == false) {
      if (attach.cs_mainFrame_->tefcopy == & tec_local) {
        attach.cs_mainFrame_->tefcopy = nullptr;
      }
    } else   {
      struct tile_action_rt *p = attach.cs_mainFrame_->umlink;
      attach.cs_mainFrame_->umlink = nullptr;
      if (p != nullptr)  p->set_discard ();
      attach.cs_mainFrame_->umlink = & art;
      attach.cs_mainFrame_->Refresh ();
      art.collect = true;
      pe.x += ! (pe.x % 8 );
      pe.y += ! (pe.y % 8 ); // round 8
      ptPoll = pe;
      ptStart = pe;
      attach.cs_mainFrame_->tefcopy = & tec_local;
    }
    this->Refresh ();
  }
public:
  struct imgSlot *pc_bank;
  class shared_cs &attach;
  struct tile_action_rt art;
  struct tile_translat tec_local;
  class wxPoint ptStart, ptPoll;
};

// read/wrire clay.
class cs_NotePage2 : public wxNotebookPage {
public:
  cs_NotePage2 (class shared_cs & cs_, struct tileChunkMap *cms, class wxWindow *parent, wxWindowID id_)
    : attach (cs_), wxNotebookPage (parent, id_)
  {
    map2 = cms;
    cls.init ();

    this->Connect (wxEVT_PAINT, wxPaintEventHandler (cs_NotePage2::OnPaint_Event));
    this->Connect (wxEVT_MOTION, wxMouseEventHandler (cs_NotePage2::OnMotion_Event));
    this->Connect (wxEVT_RIGHT_DOWN, wxMouseEventHandler (cs_NotePage2::OnMouseRButtonDown_Event));
    this->Connect (wxEVT_RIGHT_UP, wxMouseEventHandler (cs_NotePage2::OnMouseRButtonUp_Event));
    this->Connect (wxEVT_LEFT_DOWN, wxMouseEventHandler (cs_NotePage2::OnMouseLButtonDown_Event));
  }
  void OnPaint_PaintBase (class wxImage &src) {
    for (int y = 0; y != 256; y+= 8)  {
      for (int x = 0; x != 256; x+= 8)  {
        struct tileChunk *pck = & map2->map[x/8+y*4];
        struct imgSlot *s = pck->slot;
        if (s == nullptr)
          continue  ;
        if (pck->enable == false)
          continue  ;
        else if (s->enable == false)
          continue  ;
        else ;
        class wxPoint q;
        // pixel .
        q.x = pck->idInSlot & 15;
        q.y = pck->idInSlot >> 4;
        common :: setExpand (q);

        for (int u = 0; u != 8; u++)  {
          for  (int v = 0; v != 8; v++)  {
            int z = q.x + v;
            int m = q.y + u;
            char R = s->img2->GetRed (z, m);
            char G = s->img2->GetGreen (z, m);
            char B = s->img2->GetBlue (z, m);
            src.SetRGB (x+v, y+u, R, G, B );
          }
        }
      }
    }
  }
  bool inRange (wxPoint & pt)  {
    bool inRange_X =  (pt.x >= 0) && (pt.y <= 255);
    bool inRange_Y =  (pt.y >= 0) && (pt.y <= 255);
    return (inRange_Y && inRange_X);
  }
  void OnPaint_Event (wxPaintEvent& event)
  { 
    // check/paint select alpha rect.
    class wxImage *ipr = nullptr;
    class wxBufferedPaintDC dc (this);
    SetBackgroundStyle (wxBG_STYLE_CUSTOM);
    dc.Clear ();
    class wxImage img (256, 256);
    img.Clear ();
    
    if (cls.canselectOrpt() != false)  {
      class wxRect trs;
      common :: getAbsRect2(trs, ptPoll, ptStart);
      if (trs.x < 0 || trs.y < 0
        || trs.width < 0
        || trs.height < 0)
        return  ;
      if ( (trs.x + trs.width) > 256) 
        trs.width = 256 - trs.x;
      if ( (trs.y + trs.height) > 256) 
        trs.height = 256 - trs.y;
      if (attach.cs_mainFrame_->tefcopy == & tec_local) 
        tec_local.mapRect = trs;
      common :: limitBoard (trs);
      // draw base .
      OnPaint_PaintBase (img);
      ipr = & img;
      for  (int y = 0; y < trs.height; y+= 8)  {
        for  (int x = 0; x < trs.width; x+= 8)  {
          // get slot chunk.
          class wxPoint ac (trs.x + x, trs.y + y);
          common :: getRound (ac);
          struct tileChunk *u = & tec_local.map_->map[ac.x + ac.y*32];
          if (u->enable == false 
            || u->slot->enable == false)
            continue  ;
          for  (int y1 = 0; y1 != 8; y1++) {
             for (int x1 = 0; x1 != 8; x1++)  {
               int u = x +x1+trs.x;
               int v = y +y1+trs.y;
               char r = img.GetRed (u, v);
               char g = img.GetGreen (u, v);
               char b = img.GetBlue (u, v);
               r = wxColour::AlphaBlend (r, 0x6C, 0.25);
               g = wxColour::AlphaBlend (g, 0x00, 0.25);
               b = wxColour::AlphaBlend (b, 0xFF, 0.25);
               img.SetRGB (u, v, r, g, b);
             }
          }
        }      
      }
    }

    if ( attach.cs_mainFrame_->tefcopy != nullptr)  {
      if (attach.cs_mainFrame_->umlink != nullptr) {
        if (attach.cs_mainFrame_->umlink->cando() != false)  {
          // check range.
          class wxPoint pe;
          common :: getMousePos (this, pe);
          if (inRange (pe) != false)  {
            // switch 
            struct tile_translat *m = attach.cs_mainFrame_->tefcopy;
            // paint. base.
            if (ipr == nullptr)  {
              ipr = & img;
              OnPaint_PaintBase (*ipr);
            }
            //   alpha select rect/.
            common :: limitBoard (pe);
            switch (m->type)  {
            case  tile_translat::CBG_FROM_CLAY:
              {
                class wxPoint ptg;
                //   iterater.
                for  (ptg.y = 0; ptg.y < m->mapRect.height; ptg.y+=8)  {
                  for  (ptg.x = 0; ptg.x < m->mapRect.width; ptg.x+=8)  {
                    //   pick imgSlot .
                    class wxPoint S ( m->mapRect.x, m->mapRect.y);
                    class wxPoint T;
                    common :: limitBoard (S);
                    S += ptg;
                    common :: getRound (S);
                    struct tileChunk *p = & attach.cs_mainFrame_->tps.map[S.x+S.y*32];
                    struct imgSlot *pq = p->slot;
                    if (pq == nullptr)
                      continue  ;
                    if (p->enable == false)
                      continue  ;
                    if (pq->enable == false)
                      continue  ;
                    S.x = pe.x;
                    S.y = pe.y;
                    S += ptg;
                    T.x = p->idInSlot & 0x0F;
                    T.y = p->idInSlot >> 4;
                    common :: setExpand (T);
                    class wxPoint K;

                    for (K.y = 0; K.y != 8; K.y++)  {
                      for  (K.x = 0; K.x != 8; K.x++)  {
                        // fetch source.
                        class wxPoint L (K);
                        class wxPoint R (K);
                        L += T;
                        R += S;
                        if (R.x >= 0 && (R.x <= 255)
                          && (R.y >= 0 && (R.y <= 255))
                          && (L.x >= 0 && (L.x <= 127)) 
                          && (L.y >= 0 && (L.y <= 127)) )
                        {
                          char r = pq->img2->GetRed (L.x, L.y);
                          char g = pq->img2->GetGreen (L.x, L.y);
                          char b2 = pq->img2->GetBlue (L.x, L.y);
                          char R2 = ipr->GetRed (R.x, R.y);
                          char G = ipr->GetGreen (R.x, R.y);
                          char B = ipr->GetBlue (R.x, R.y);
                          // alpha 50% rect.
                          r = wxColour::AlphaBlend (r, R2, 0.5);
                          g = wxColour::AlphaBlend (g, G, 0.5);
                          b2 = wxColour::AlphaBlend (b2, B, 0.5);
                          ipr->SetRGB (R.x, R.y, r, g, b2);
                        }
                      }
                    }
                  }      
                }
              }
              break;
            case  tile_translat::CBG_FROM_ROOTTEXTURE:
              {
                //   iterater.
                for  (int y = 0; y < m->mapRect.height; y+=8)  {
                  for  (int x = 0; x < m->mapRect.width; x+=8)  {
                    //   pick imgSlot .
                    struct imgSlot *pq = m->slot_;
                    class wxPoint sPR (x +  m->mapRect.x, y +  m->mapRect.y);
                    class wxPoint dPR (x +  pe.x, y +  pe.y);
                    class wxPoint spS;
                    if (pq == nullptr)
                      continue  ;
                    if (pq->enable == false)
                      continue  ;
                    for (spS.y = 0; spS.y != 8; spS.y++)  {
                        for  (spS.x = 0; spS.x != 8; spS.x++)  {
                          // fetch source.
                          class wxPoint T (spS);
                          class wxPoint S (spS);
                          S += sPR;
                          T += dPR;

                          if ( (T.x >= 0 && (T.x <= 255)) 
                            && (T.y >= 0 && (T.y <= 255))
                            && (S.x >= 0 && (S.x <= 127)) 
                            && (S.y >= 0 && (S.y <= 127)) )
                          {
                            char r = pq->img2->GetRed (S.x, S.y);
                            char g = pq->img2->GetGreen (S.x, S.y);
                            char b2 = pq->img2->GetBlue (S.x, S.y);
                            char R = ipr->GetRed (T.x, T.y);
                            char G = ipr->GetGreen (T.x, T.y);
                            char B = ipr->GetBlue (T.x, T.y);
                            // alpha 50% rect.
                            r = wxColour::AlphaBlend (r, R, 0.5);
                            g = wxColour::AlphaBlend (g, G, 0.5);
                            b2 = wxColour::AlphaBlend (b2, B, 0.5);
                            ipr->SetRGB (T.x, T.y, r, g, b2);
                          }
                        }
                    }
                  }      
                }
              }
              break ;
            }
          }
        }
      }
    }
    if (ipr != nullptr)  {
      class wxBitmap gbitmap (*ipr);
      dc.DrawBitmap (gbitmap, 0, 0);
    } else  {
      OnPaint_PaintBase (img);
      class wxBitmap gbitmap (img);
      dc.DrawBitmap (gbitmap, 0, 0);
    }
  }
  void OnMouseRButtonUp_Event (wxMouseEvent & event)
  {
    cls.stop = true;
    cls.collect = false;
  }
  void outputBaseInfos ()  {

    wxPoint s;
    wxString rc;
    common ::getMousePos (this, s); 
    // GetClientSize2 (s2);
    int xi = -1;
    int yi = -1;
    int c = -1;
    int page = -1;
    int id = -1;
    bool inRange = false;
    if (s.x >= 0 && (s.x <= 255)) 
      xi = s.x >> 3;
    if (s.y >= 0 && (s.y <= 255))
      yi = s.y >> 3;
    if (xi != -1 && yi != -1)
      c = xi + yi * 32;
    if (c != -1){ 
        inRange = true;
        if (map2->map[c].enable != false)  {
          id = map2->map[c].idInSlot;
          if (map2->map[c].slot != nullptr)  {
            page = map2->map[c].slot->id ;
          }
        }
    }
    rc.Printf (  
      wxT ("INRANGE:%s BID-%02X(%d) PAGE-%02X(%d) ID:-%02X(%d)"), inRange ? wxT ("true") : wxT ("false"), 
         c, c, page, page, id, id);
    attach.cs_mainFrame_->GetStatusBar ()->SetLabelText (rc);
  }
  void OnMotion_Event (wxMouseEvent & event)
  {
    // check drag done.
    outputBaseInfos ();
    if ( cls.stop == false && ( attach.cs_mainFrame_->tefcopy != nullptr))  {
      common :: getMousePos (this, ptPoll);
      this->Refresh ();
    } else  {
      this->Refresh ();
    }
  }

  void OnMouseRButtonDown_Event (wxMouseEvent & event)
  {
    cls.stop = false;
    wxPoint pe;
    common :: getMousePos (this, pe);

    tec_local.type = tile_translat::CBG_FROM_CLAY;
    tec_local.map_ = map2;

    if (pe.x > 255 || pe.y > 255
      || pe.x < 0 || pe.y < 0) {
      cls.collect = false;
      if (attach.cs_mainFrame_->tefcopy == & tec_local) {
        attach.cs_mainFrame_->tefcopy = nullptr;
      }
    } else  {
      struct tile_action_rt *p = attach.cs_mainFrame_->umlink;
      attach.cs_mainFrame_->umlink = nullptr;
      if (p != nullptr)  p->set_discard ();
      attach.cs_mainFrame_->umlink = & cls;
      attach.cs_mainFrame_->Refresh ();
      cls.collect = true;
      ptPoll = pe;
      ptStart = pe;
      attach.cs_mainFrame_->tefcopy = & tec_local;
      attach.cs_mainFrame_->tps.copy (*map2);
    }
    this->Refresh ();
  }
  void OnMouseLButtonDown_Event (wxMouseEvent & event)
  {
    wxPoint pe  = wxGetMousePosition ();
    common :: getMousePos (this, pe);

    if  (  pe.x >= 0 
        && pe.x <= 255 
       && pe.y >= 0 
       && pe.y <= 255)
    {
       // fill rect.
      tile_translat *tf = attach.cs_mainFrame_->tefcopy;
      if (tf != nullptr && tf->type == tile_translat::CBG_FROM_CLAY)  {
        common :: limitBoard (pe);
        class wxPoint sp;
        class wxPoint ap;
        class wxPoint kpc;
        // limit range.
        if (pe.x + tf->mapRect.width > 256)
          sp.x = tf->mapRect.width - ( pe.x + tf->mapRect.width - 256);
        else 
          sp.x = tf->mapRect.width;
        if (pe.y + tf->mapRect.height > 256)
          sp.y = tf->mapRect.height - ( pe.y + tf->mapRect.height - 256);
        else 
          sp.y = tf->mapRect.height;
        common :: getRound (pe);
        ap.x = tf->mapRect.x;
        ap.y = tf->mapRect.y;
        struct tileChunkMap *mq = & attach.cs_mainFrame_->tps;
        struct tileChunkMap mt;
        if (map2 == tf->map_)  {
          memcpy (& mt, mq, sizeof (mt));
          mq = & mt;
        }
        for ( kpc.y = 0; kpc.y < sp.y; kpc.y+= 8)  {
          for (kpc.x = 0; kpc.x < sp.x; kpc.x+= 8)  {
            class wxPoint sgp (ap);
            sgp += kpc;
            common :: getRound (sgp);
            struct tileChunk *p = & mq->map[sgp.x+sgp.y*32];
            struct tileChunk *d = & map2->map[kpc.x/8+pe.x+(kpc.y/8+pe.y)*32];
            if (p->enable == false)
              continue  ;
            d->enable = true;
            d->idInSlot = p->idInSlot;
            d->slot = p->slot;
          }
        }
        this->Refresh ();
        return  ;
      }

       if (tf != nullptr && tf->type == tile_translat::CBG_FROM_ROOTTEXTURE)  {
         common :: limitBoard (pe);
         class wxPoint sp;
         class wxPoint ap;
         class wxPoint kpc;
         // limit range.
         if (pe.x + tf->mapRect.width > 256)
           sp.x = tf->mapRect.width - ( pe.x + tf->mapRect.width - 256);
         else 
           sp.x = tf->mapRect.width;
         if (pe.y + tf->mapRect.height > 256)
           sp.y = tf->mapRect.height - ( pe.y + tf->mapRect.height - 256);
         else 
           sp.y = tf->mapRect.height;
         common :: getRound (pe);
         ap.x = tf->mapRect.x;
         ap.y = tf->mapRect.y;
         common :: getRound (ap);
         for ( kpc.y = 0; kpc.y < sp.y; kpc.y+= 8)  {
             for (kpc.x = 0; kpc.x < sp.x; kpc.x+= 8)  {
               // to cachemap.
               class wxPoint ks (kpc);
               struct imgSlot *pq = tf->slot_;
               if (pq == nullptr)
                 continue  ;
               if (pq->enable == false)
                 continue  ;
               common :: getRound (ks);
               tileChunk *p = & map2->map[ks.x+pe.x+(ks.y+pe.y)*32];
               p->slot = pq;
               p->enable = true;
               p->idInSlot = ap.x + ks.x;
               p->idInSlot += ( ap.y + ks.y) * 16;
             }
         }
         this->Refresh ();
         return  ;
       }
    }
    this->Refresh ();
  }
public:
  // ptr to bank.
  class shared_cs &attach;
  struct tile_action_rt cls;
  struct tile_translat tec_local;
  class wxPoint ptStart;
  class wxPoint ptPoll;
  struct tileChunkMap  *map2;
};

void cs_mainFrame::OnView_BTexture  (class wxCommandEvent& event) {

  //  create notebook ctrl.
  wxUint16 id;
  fd_ViewRootTexture_ = new wxAuiNotebook (this);

  // serach rt.
  for (id = 0; id != sizeof (imgSlot_)/ sizeof (imgSlot_[0]); id++)  {
    if  (imgSlot_[id].enable == true)  {
      // append to pane.
      cs_NotePage *cn = new cs_NotePage (attach, & imgSlot_[id], fd_ViewRootTexture_);
      wxRect rt (0, 0, imgSlot::image_size_w, imgSlot::image_size_h); 
      cn->SetSize(rt, 0);
      wxString wxStr;
      wxStr.Printf (wxT ("PAGE-%02X(%d)"), imgSlot_[id].id, imgSlot_[id].id);
      cn->SetBackgroundColour ( wxColour (0xd8b166));    // XXX:bgcolour.
      fd_ViewRootTexture_->AddPage ( cn, wxStr); 
    }
  }
  aui_Manager.AddPane ( fd_ViewRootTexture_, 
    wxAuiPaneInfo().Name (wxT ("VIEW-root texture")).Caption (wxT ("VIEW-root texture")).
    Right().Layer(0).Position(0).
    Icon (wxBitmap (g_xpmopgptex)));
  GetMenuBar()->Enable (wxID_BTEXTURE, false);
  aui_Manager.GetPane (fd_ViewRootTexture_).DestroyOnClose (true);
  // tell the manager to "commit" all the changes just made
  fd_ViewRootTexture_->Layout();
  aui_Manager.Update();
}

void cs_mainFrame::OnView_Clay  (class wxCommandEvent& event) {

  //  dummy notebook ctrl.
  fd_ViewClaySet_ = new wxAuiNotebook (this);

  std::list <tileChunkMap *>::iterator iter;

  for (iter = fmset.begin(); iter != fmset.end(); iter++)  
  {  
    cs_NotePage2 *cn = new cs_NotePage2 (attach, *iter, fd_ViewClaySet_, wxID_ANY );
    cn->SetSize(wxRect (0, 0, 256, 256), 0);
    cn->SetBackgroundColour ( wxColour (0xd8b166));
    fd_ViewClaySet_->AddPage ( cn, ( (*iter)->desc != nullptr ? *(*iter)->desc : wxT ("noname")));
  }  
  aui_Manager.AddPane ( fd_ViewClaySet_, 
    wxAuiPaneInfo().Name (wxT ("VIEW-clay texture")).Caption (wxT ("VIEW-clay texture")).
    Right().Layer(0).Position(0).
    Icon (wxBitmap (g_xpmopgpclay)));
  GetMenuBar()->Enable (wxID_CLAY, false);
  aui_Manager.GetPane (fd_ViewClaySet_).DestroyOnClose (true);
  // tell the manager to "commit" all the changes just made
  fd_ViewClaySet_->Layout();
  aui_Manager.Update();
}

// load texture.

void cs_mainFrame::OnFile_LoadImage (class wxCommandEvent& event)  {

  wxString caption = wxString ("choose texture.");
  wxString wildcard = wxString ("BMP files(*.bmp)|*.bmp|JPG files(*.jpg)|*.jpg|JPEG files(*.jpeg)|*.jpeg|PNG files(*.png)|*.png");
  wxString defaultfn = wxEmptyString;
  wxFileDialog  dialog  (this, caption, wxEmptyString, wxEmptyString, wildcard, wxFD_OPEN);
  if ( dialog.ShowModal () == wxID_OK)  {
    wxString Path = dialog.GetPath ();
    int fid = dialog.GetFilterIndex ();
    // check image.
    wxBitmap img;
    if ( img.LoadFile (Path, wxBITMAP_TYPE_ANY) != false) {
      wxSize s = img.GetSize ();
      if ( ! ( s.x == imgSlot::image_size_w && (s.y == imgSlot::image_size_h))) {
        // size unmatch.
        wxString e;
        e.Printf (wxT ("errors:load texture failed"));
        StatusOutWrap (e);
        e.Printf (wxT ("reason:unmatch image size"));
        StatusOutWrap (e);
        e.Printf (wxT ("size-x:%d size-y:%d"), s.x, s.y);
        StatusOutWrap (e);
        e.Printf (wxT ("file:%s"), Path);
        StatusOutWrap (e);
        StatusShowText();
        return ;
      }
      // read it. size 128*128
      // set page and misc.
      wxNumberEntryDialog s2 ( this, wxT ("Please set current page"),
        wxT ("Enter a number:"), wxT ("Page Select"), 0, 0, 255);
      if (s2.ShowModal () == wxID_OK) {
        long nums = s2.GetValue ();
        // serach exist empty chunk..
        imgSlot *ptr = nullptr;
        bool s = false;
        for  (int id = 0; id != sizeof (imgSlot_)/ sizeof (imgSlot_[0]); id++)  {
          if (imgSlot_[id].enable != false)  {
            if  (imgSlot_[id].id == nums) {
              // find chunk.
              // do repleace.
              wxWindowID q = wxMessageBox ( wxT ("current page exist object, continue?"), wxT ("Page select"), wxOK | wxCENTER | wxCANCEL);
              if (q == wxCANCEL)
                return  ;
              else  ;
              s = true;
              ptr = & imgSlot_[id];
              break; 
            }
          } else  {
            if (ptr == nullptr)
              ptr = & imgSlot_[id];
            else ;
          }
        }

        assert (ptr != nullptr);
        ptr->uninit ();
        ptr->enable = true;
        ptr->id = nums;
        ptr->img = new wxBitmap (img);
        wxImage c = ptr->img->ConvertToImage();
        ptr->img2 = new wxImage (c);
        ptr->strPath = new wxString (Path);
        
        if (s != false && (fd_ViewRootTexture_ != nullptr))  {
          for  (int id = 0; id != fd_ViewRootTexture_->GetPageCount (); id++)  {
            cs_NotePage *cs = (cs_NotePage *) fd_ViewRootTexture_->GetPage (id);
            if (cs->pc_bank == ptr)  {
              fd_ViewRootTexture_->DeletePage (id);
              break  ;
            }
          }
        } 
        if  (fd_ViewRootTexture_ != nullptr)  {
          // append item. to notebook.
          cs_NotePage *cn = new cs_NotePage (attach, ptr, fd_ViewRootTexture_, wxID_ANY);
          cn->SetBackgroundColour ( wxColour (0xd8b166));
          cn->SetSize (wxRect (0, 0, imgSlot::image_size_w, imgSlot::image_size_h), 0);
          wxString wxStr;
          wxStr.Printf (wxT ("PAGE-%02X(%d)"), ptr->id, ptr->id);
          fd_ViewRootTexture_->AddPage ( cn, wxStr); 
        }
        return ;
      }
    }
    // file read fail.
    wxString e;
    e.Printf (wxT ("errors:load texture failed"));
    StatusOutWrap (e);
    e.Printf (wxT ("reason:load texture failed"));
    StatusOutWrap (e);
    e.Printf (wxT ("file:%s"), Path);
    StatusOutWrap (e);
    StatusShowText();
    return ;
  }
}

void cs_mainFrame::OnFile_ClayAdd (class wxCommandEvent& event) {

  wxDialog T (this, wxID_ANY, wxT ("Add Clay"), wxPoint (0, 0), wxSize (160, 80) );
  wxButton butOk (& T, wxID_OK, wxT ("OK"), wxPoint (4, 24), wxSize (60, 24));
  wxButton butCancel (& T, wxID_CANCEL, wxT ("CANCEL"), wxPoint (80, 24), wxSize (60, 24));
  wxTextCtrl textCtlPageDesc (& T, wxID_ANY, wxEmptyString, wxPoint (80, 4), wxSize (60, 19));
  wxStaticText textSetPageDesc (& T, wxID_ANY, wxT ("clay desc"), wxPoint (4, 4));

  // set range.

  T.SetIcon ( wxIcon ( g_xpmopgpclay));
  T.CenterOnParent ();
  int msgid = T.ShowModal();
  if (msgid == wxID_OK) {
    // append to list.
    tileChunkMap *cmap = new tileChunkMap;
    cmap->init();
    cmap->desc = nullptr;
    wxString s = textCtlPageDesc.GetValue ();
    if (s.size() > 0) 
      cmap->desc = new wxString (s);
    else 
      cmap->desc = new wxString ("noname");
    fmset.push_front (cmap);
    // cs_NotePage2 *cn = new cs_NotePage2 (attach, *iter, fd_ViewClaySet_, wxID_ANY );
    if  (fd_ViewClaySet_ != nullptr)  {
      // append item. to notebook.
      cs_NotePage2 *cn = new cs_NotePage2 (attach, cmap, fd_ViewClaySet_, wxID_ANY);
      cn->SetBackgroundColour ( wxColour (0xd8b166));
      cn->SetSize (wxRect (0, 0, 256, 256), 0);
      wxString wxStr;
      wxStr.Printf (wxT ("PAGE-%02s(%d)"));
      fd_ViewClaySet_->AddPage ( cn, *cmap->desc); 
    }
    // new cs_PageTileTab (attach, note_tab2, wxID_PAGE_NEW, spinWidth.GetValue (), spinHeight.GetValue (),textCtlPageDesc.GetValue ()  );
  }
}

void  cs_mainFrame::OnView_Close  (wxAuiManagerEvent& event) {

  if (event.GetPane()->window == fd_ViewStatus_) {
    fd_ViewStatus_ = nullptr;
    attach.cs_ViewStatus_ = nullptr;
    GetMenuBar()->Enable (wxID_STATUS_OUTPUT, true);
  } else if ( event.GetPane()->window ==  fd_ViewRootTexture_) {
    fd_ViewRootTexture_ = nullptr;
    attach.cs_ViewRootTexture_ = nullptr;
    GetMenuBar()->Enable (wxID_BTEXTURE, true);
  } else if ( event.GetPane()->window ==  fd_ViewClaySet_) {
    fd_ViewClaySet_ = nullptr;
    attach.cs_ViewClaySet_ = nullptr;
    GetMenuBar()->Enable (wxID_CLAY, true);
  }
}

cs_mainFrame::cs_mainFrame (wxWindow* parent) 
  : wxFrame (parent, wxID_ANY, wxT ("tilemap_editors")), attach (share_cs_)
{
  // clear attach .
  umlink = nullptr;
  tefcopy = nullptr;

  // init mgr solt.
  wxUint16 id;
  for  (id = 0; id != sizeof (imgSlot_)/ sizeof (imgSlot_[0]); id++) {
    imgSlot_[id].init ();
  }

  // init view assert .
  fd_mainFrame_ = nullptr;
  fd_ViewStatus_ = nullptr;
  fd_ViewGlobalZoom_ = nullptr;
  fd_ViewRootTexture_ = nullptr;
  fd_ViewClaySet_ = nullptr;
  fd_ViewPixel_ = nullptr;

  tps.init ();
  attach.cs_mainFrame_ = this;
  ctab = nullptr;

  // init menu. 
  wxMenuBar *menubar = new wxMenuBar;
  wxMenu *submenu;
  
  submenu = new wxMenu;
  wxMenuItem *subitem = new wxMenuItem (submenu, wxID_LOAD_TEXTURE, wxT ("&load texture\tCtrl-Q"), wxT ("Read 128 square tile texture."));
  subitem->SetBitmap (wxBitmap (g_xpmoptex));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_LOAD_SETTINGS, wxT ("&load settings\tCtrl-T"), wxT ("Read environment configuration files."));
  subitem->SetBitmap (wxBitmap (g_xpmldset));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_SAVE_SETTINGS, wxT ("&save settings\tCtrl-C"), wxT ("Save environment configuration files."));
  subitem->SetBitmap (wxBitmap (g_xpmstset));
  submenu->Append (subitem);
  submenu->AppendSeparator ();
  subitem = new wxMenuItem (submenu, wxID_PAGE_SETTINGS, wxT ("&page settings\tCtrl-V"), wxT ("Current tile map edit"));
  subitem->SetBitmap (wxBitmap (g_xpmadjust_a));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_PAGE_NEW, wxT ("&page new\tCtrl-D"), wxT ("Alloc new tile-map page"));
  subitem->SetBitmap (wxBitmap (g_xpmpage_new));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_PAGE_CLOSE, wxT ("&page close\tCtrl-R"), wxT ("Close current tile-map page"));
  subitem->SetBitmap (wxBitmap (g_xpmpage_close));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_PAGE_SAVE, wxT ("&page save\tCtrl-E"), wxT ("Save tile-map files."));
  subitem->SetBitmap (wxBitmap (g_xpmstchr));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_CLAY_ADD, wxT ("&clay add\tCtrl-Z"), wxT ("Add clay files."));
  subitem->SetBitmap (wxBitmap (g_xpmopgpclay));
  submenu->Append (subitem);
  submenu->AppendSeparator ();
  subitem = new wxMenuItem (submenu, wxID_EXIT, wxT ("&exit\tCtrl-A"), wxT ("Exit ."));
  subitem->SetBitmap (wxBitmap (g_xpmopexit));
  submenu->Append (subitem);
  menubar->Append (submenu, wxT ("&File"));

  submenu = new wxMenu;
  subitem = new wxMenuItem (submenu, wxID_GVIEW, wxT ("&gview\tCtrl-S"), wxT ("Thumbnail previews"));
  subitem->SetBitmap (wxBitmap (g_xpmopgview));
  submenu->Append (subitem);
  submenu->AppendSeparator ();
  subitem = new wxMenuItem (submenu, wxID_BTEXTURE, wxT ("&basetexture\tCtrl-B"), wxT ("128 square tile texture."));
  subitem->SetBitmap (wxBitmap (g_xpmopgptex));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_CLAY, wxT ("&clay\tCtrl-L"), wxT ("Mixed tile set."));
  subitem->SetBitmap (wxBitmap (g_xpmopgpclay));
  submenu->Append (subitem);
  submenu->AppendSeparator ();
  subitem = new wxMenuItem (submenu, wxID_STATUS_OUTPUT, wxT ("&status output\tCtrl-O"), wxT ("Same as name."));
  subitem->SetBitmap (wxBitmap (g_xpmopgstatout));
  submenu->Append (subitem);
  submenu->AppendSeparator ();
  subitem = new wxMenuItem (submenu, wxID_PIXEL, wxT ("&pixel\tCtrl-M"), wxT ("Pixel status ."));
  subitem->SetBitmap (wxBitmap (g_xpmopgpixel));
  submenu->Append (subitem);
  menubar->Append (submenu, wxT ("&View"));

  submenu = new wxMenu;
  subitem = new wxMenuItem (submenu, wxID_ABOUT_WXWIDGETS, wxT ("&wxWidgets\tCtrl-U"), wxT ("About wxWidgets framework."));
  subitem->SetBitmap (wxBitmap (g_xpmop_wxwidgets));
  submenu->Append (subitem);
  subitem = new wxMenuItem (submenu, wxID_ABOUT_EDITORS, wxT ("&tilemap editors\tCtrl-G"), wxT ("About program."));
  subitem->SetBitmap (wxBitmap (g_xpmop_tilemap));
  submenu->Append (subitem);
  menubar->Append (submenu, wxT ("&About"));

  this->SetMenuBar (menubar);
  this->CreateStatusBar(); 
  this->Maximize ();
  this->note_tab2 = new cs_PageTile (attach, this);
  // this->note_tab2->SetArtProvider (new wxAuiSimpleTabArt ());
  this->note_tab2->Hide();

  aui_Manager.SetManagedWindow (this);
  aui_Manager.SetFlags ( wxAUI_MGR_ALLOW_FLOATING | wxAUI_MGR_VENETIAN_BLINDS_HINT | wxAUI_MGR_ALLOW_ACTIVE_PANE);
  aui_Manager.SetArtProvider (new wxAuiDefaultDockArt);
  aui_Manager.AddPane(this->note_tab2, wxCENTER);
  wxColour s;
  s.SetRGB (0x00ff13);
  aui_Manager.GetArtProvider()->SetColor (wxAUI_DOCKART_BORDER_COLOUR, s);
  s.SetRGB (0xff6600);
  aui_Manager.GetArtProvider()->SetColor (wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, s);
  s.SetRGB (0xdb844b);
  aui_Manager.GetArtProvider()->SetColor (wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, s);

  // tell the manager to "commit" all the changes just made
  aui_Manager.Update();
  StatusOutWrap (wxString ("init success..."));
}

void cs_mainFrame::OnView_StatusOutput  (class wxCommandEvent& event) {

  // init status out [only text ctrl] 
  fd_ViewStatus_ = new cs_ViewStatus (share_cs_, this, wxID_ANY, wxT (""),
    wxDefaultPosition, wxSize (200,150),
    wxTE_MULTILINE);
  StatusShowText ();
  // disable it.
  // set panel .
  aui_Manager.AddPane ( fd_ViewStatus_, 
    wxAuiPaneInfo().Name (wxT ("VIEW-status output")).Caption (wxT ("VIEW-status output")).
    Right().Layer(0).Position(0).
    Icon (wxBitmap (g_xpmopgstatout)));
  aui_Manager.GetPane (fd_ViewStatus_).DestroyOnClose (true);
  aui_Manager.Update ();
  GetMenuBar()->Enable (wxID_STATUS_OUTPUT, false);
}

// wxSmith. 
class pageSettingsDialog: public wxDialog
{
    public:
        pageSettingsDialog (wxWindow* parent,wxWindowID id, tileChunkSet * cs, cs_mainFrame *ce);
    public:
        void CheckMtx (wxWindowID secid, wxCheckBox **ppBlock, int n) {
          for (int id =0 ; id != n; id++)  {
            if (ppBlock[id]->GetId () == secid)  {
              bool c = ppBlock[id]->IsChecked ();
              if (c == false)  {
                // dis false. 
                ppBlock[id]->SetValue (true);
                return  ;
              }
              // canel other. 
              for (int id =0 ; id != n; id++)  {
                if (ppBlock[id]->GetId () != secid) 
                  ppBlock[id]->SetValue (false);
                else  ;
              }
              return  ;
            }
          }
        }
        void On_CheckMtxOnCtl_Event (wxCommandEvent& event) {

          wxCheckBox *pg[3] = { checkBox_disall, checkBox_saveold, checkBox_fill };
          CheckMtx (event.GetId (), & pg[0], sizeof (pg)/ sizeof (pg[0]));
        }
        void On_CheckMtxOnH_Event (wxCommandEvent& event) {

          wxCheckBox *pg[2] = { checkBox_addh, checkBox_subh };
          CheckMtx (event.GetId (), & pg[0], sizeof (pg)/ sizeof (pg[0]));
        }
        void On_CheckMtxOnV_Event (wxCommandEvent& event) {

          wxCheckBox *pg[2] = { checkBox_addv, checkBox_subv };
          CheckMtx (event.GetId (), & pg[0], sizeof (pg)/ sizeof (pg[0]));
        }

        static const long wxID_DISALL;
        static const long wxID_SAVEOLD;
        static const long wxID_FILL;
        static const long wxID_SETPAGE;
        static const long wxID_SETLAND;
        static const long wxID_ADDH;
        static const long wxID_SUBH;
        static const long wxID_SETBPOSH;
        static const long wxID_SETBVOLH;
        static const long wxID_ADDV;
        static const long wxID_SUBV;
        static const long wxID_SETBPOSV;
        static const long wxID_SETBVOLV;
        //*)

        //(*Declarations(pageSettingsDialog)
        wxCheckBox *checkBox_disall;
        wxCheckBox *checkBox_saveold;
        wxCheckBox *checkBox_fill;
        wxCheckBox *checkBox_addh;
        wxCheckBox *checkBox_subh;
        wxCheckBox *checkBox_addv;
        wxCheckBox *checkBox_subv;

        DECLARE_EVENT_TABLE()
};

//(*IdInit(pageSettingsDialog)
const long pageSettingsDialog::wxID_DISALL = wxNewId();
const long pageSettingsDialog::wxID_SAVEOLD = wxNewId();
const long pageSettingsDialog::wxID_FILL = wxNewId();
const long pageSettingsDialog::wxID_SETPAGE = wxNewId();
const long pageSettingsDialog::wxID_SETLAND = wxNewId();
const long pageSettingsDialog::wxID_ADDH = wxNewId();
const long pageSettingsDialog::wxID_SUBH = wxNewId();
const long pageSettingsDialog::wxID_ADDV = wxNewId();
const long pageSettingsDialog::wxID_SUBV = wxNewId();
//*)

BEGIN_EVENT_TABLE(pageSettingsDialog,wxDialog)
  EVT_CHECKBOX (wxID_DISALL, On_CheckMtxOnCtl_Event)
  EVT_CHECKBOX (wxID_SAVEOLD, On_CheckMtxOnCtl_Event)
  EVT_CHECKBOX (wxID_FILL, On_CheckMtxOnCtl_Event)
  EVT_CHECKBOX (wxID_ADDH, On_CheckMtxOnH_Event)
  EVT_CHECKBOX (wxID_SUBH, On_CheckMtxOnH_Event)
  EVT_CHECKBOX (wxID_ADDV, On_CheckMtxOnV_Event)
  EVT_CHECKBOX (wxID_SUBV, On_CheckMtxOnV_Event)
END_EVENT_TABLE()

pageSettingsDialog::pageSettingsDialog (wxWindow* parent, wxWindowID id, tileChunkSet * cs, cs_mainFrame *ce)
{
  Create (parent, wxID_ANY, wxT("pagesettings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, wxT("wxID_ANY"));
  SetClientSize (wxSize(402,285));
  wxString csHeight, csWidth ;
  csWidth.Printf (wxT ("%d"), cs->hnums);
  csHeight.Printf (wxT ("%d"), cs->vnums);

  new wxTextCtrl (this, wxID_ANY, csWidth, wxPoint(80,40), wxDefaultSize, wxTE_READONLY);
  new wxStaticText (this, wxID_ANY, wxT("width"), wxPoint(32,40), wxSize(40,14));
  new wxStaticText (this, wxID_ANY, wxT("height"), wxPoint(32,72));
  new wxTextCtrl (this, wxID_ANY, csHeight, wxPoint(80,72), wxDefaultSize, wxTE_READONLY);
  new wxStaticBox (this, wxID_ANY, wxT("H-VEC"), wxPoint(240,16), wxSize(128,96));
  new wxStaticBox (this, wxID_ANY, wxT("ctrl"), wxPoint(16,24), wxSize(192,136));
  new wxStaticBox (this, wxID_ANY, wxT("V-VEC"), wxPoint(248,120), wxSize(136,96));
  new wxStaticLine (this, wxID_ANY, wxPoint(16,232), wxSize(368,-1));
  new wxButton (this, wxID_OK, wxT("OK"), wxPoint(240,248), wxSize(64,24));
  new wxButton (this, wxID_CANCEL, wxT("CANCEL"), wxPoint(320,248), wxSize(67,24));

    // check box add. 
  (checkBox_disall = new wxCheckBox (this, wxID_DISALL, wxT("disall"), wxPoint(32,104)))->SetValue (true);
  (checkBox_saveold = new wxCheckBox (this, wxID_SAVEOLD, wxT("old"), wxPoint(32,120)))->SetValue (false);
  (checkBox_fill = new wxCheckBox (this, wxID_FILL, wxT("fill"), wxPoint(32,136)))->SetValue (false);
  (checkBox_addv = new wxCheckBox (this, wxID_ADDV, wxT("+"), wxPoint(296,136)))->SetValue (true);
  (checkBox_subv = new wxCheckBox (this, wxID_SUBV, wxT("-"), wxPoint(336,136)))->SetValue (false);
  (checkBox_addh = new wxCheckBox (this, wxID_ADDH, wxT("+"), wxPoint(288,32)))->SetValue (true);
  (checkBox_subh = new wxCheckBox (this, wxID_SUBH, wxT("-"),  wxPoint(328,32)))->SetValue (false);
  // spin ctrl add.
  wxSpinCtrl *spc_SetPage = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(144,104), wxSize(56,22), 0, 0, 255, 0);
  wxSpinCtrl *spc_SetLand = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(144,128), wxSize(56,22), 0, 0, 255, 0);
  wxSpinCtrl *spc_SetBposH = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(296,56), wxSize(64,16), 0, 0, 65535, 0);
  wxSpinCtrl *spc_SetVolH = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(296,88), wxSize(64,16), 0, 0, 65535, 0);
  wxSpinCtrl *spc_SetBposV = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(300,156), wxSize(80,24), 0, 0, 65535, 0);
  wxSpinCtrl *spc_SetVolV = new wxSpinCtrl (this, wxID_ANY, wxT("0"), wxPoint(300,188), wxSize(80,22), 0, 0, 65535, 0);
  // some misctextg. 
  new wxStaticText (this, wxID_ANY, wxT("page"), wxPoint(96,104), wxSize(40,14));
  new wxStaticText (this, wxID_ANY, wxT("land"), wxPoint(96,128), wxSize(30,14));
  new wxStaticText (this, wxID_ANY, wxT("phase"), wxPoint(248,32));
  new wxStaticText (this, wxID_ANY, wxT("bpos b"), wxPoint(248,56));
  new wxStaticText (this, wxID_ANY, wxT("volume"), wxPoint(248,88));
  new wxStaticText (this, wxID_ANY, wxT("phase"), wxPoint(256,136));
  new wxStaticText (this, wxID_ANY, wxT("bpos b"), wxPoint(256,152));
  new wxStaticText (this, wxID_ANY, wxT("volume"), wxPoint(256,184));
  
  SetIcon (wxIcon (g_xpmadjust_a));
  if ( ShowModal () == wxID_OK)  {
    // adjust page attr/size.
    bool bHAdd = checkBox_addh->GetValue ();
    bool bVAdd = checkBox_addv->GetValue ();
    bool bSaveOld  = checkBox_saveold->GetValue ();
    bool bDiscard = checkBox_disall->GetValue ();
    bool bSetPage = checkBox_fill->GetValue ();
    int nHPos = spc_SetBposH->GetValue ();
    int nHVolume = spc_SetVolH->GetValue ();
    int nVPos = spc_SetBposV->GetValue ();
    int nVVolume = spc_SetVolV->GetValue ();
    int nHNewVol = cs->hnums + (bHAdd ? nHVolume : -nHVolume);
    int nVNewVol = cs->vnums + (bVAdd ? nVVolume : -nVVolume);
    int clearRangeLeftH = nHPos;
    int clearRangeRightH = nHPos + nHVolume - 1;
    int clearRangeLeftV = nVPos;
    int clearRangeRightV = nVPos + nVVolume - 1;
    wxUint8 page = spc_SetPage->GetValue ();
    wxUint8 land = spc_SetLand->GetValue ();
    // TODO:check,
    tileChunkMap *ppBlock = new tileChunkMap [nHNewVol * nVNewVol];
    if (bDiscard != false)  goto LLCC;
    // copy. hori .
    for (int y = 0, ptY = 0; y != (bVAdd ? nVNewVol : cs->vnums); y ++)  {
      if (!  (y >= clearRangeLeftV && (y <= clearRangeRightV)))  {
        int ats = y;
        int atd = ptY;
        int mts = nHNewVol;
        int mtd = cs->hnums;
        if  (bVAdd != false)  {
            // small -> big
          ats = ptY;
          atd = y;
          mts = cs->hnums;
          mtd = nHNewVol;
        }   
        for (int x = 0, ptX = 0; x != (bHAdd ? nHNewVol : cs->hnums); x ++)  {
          // check range?? 
          if (!  (x >= clearRangeLeftH && (x <= clearRangeRightH)))  {
            int cts = x;
            int ctd = ptX;

            if  (bHAdd != false)  {
              // small -> big
              cts = ptX;
              ctd = x;             
            }  

            int s = cts + ats * mts;
            int d = ctd + atd * mtd;
            if (bSetPage == false)
              ppBlock [d].copy (cs->tcm[s]);
            else  
              ppBlock [d].copy (cs->tcm[s], ce->getSlotByID (page), land);
            ptX ++;
          }
        }
        ptY ++;
      }
    }
LLCC: delete [] cs->tcm;
    cs->hnums = nHNewVol;
    cs->vnums = nVNewVol;
    cs->tcm = ppBlock;
  }
}

void cs_mainFrame::OnFile_PageSettings  (class wxCommandEvent& event) {

  if (this->ctab == nullptr)  {
    wxMessageBox (wxT ("no attach page"), wxT ("tilemap editors"), wxICON_WARNING);
    return  ;
  }
  wxSizeEvent e;
  pageSettingsDialog p (this, wxID_ANY, & this->ctab->chunkset, this);
  this->ctab->OnSize_Event (e);
  this->ctab->XorBgColor ();
  return ;
}

void cs_mainFrame::OnFile_PageNew  (class wxCommandEvent& event) {

  wxDialog T (this, wxID_ANY, wxT ("Add Page"), wxPoint (0, 0), wxSize (160, 140) );
  wxButton butOk (& T, wxID_OK, wxT ("OK"), wxPoint (4, 80), wxSize (60, 24));
  wxButton butCancel (& T, wxID_CANCEL, wxT ("CANCEL"), wxPoint (80, 80), wxSize (60, 24));
  wxSpinCtrl spinWidth (& T, wxID_ANY, wxEmptyString, wxPoint (104, 6), wxSize (48, 19));
  wxSpinCtrl spinHeight (& T, wxID_ANY, wxEmptyString, wxPoint (104, 30), wxSize (48, 19));
  wxTextCtrl textCtlPageDesc (& T, wxID_ANY, wxEmptyString, wxPoint (76, 54), wxSize (60, 19));
  wxStaticText textSetWidth (& T, wxID_ANY, wxT ("set page width"), wxPoint (4, 6));
  wxStaticText textSetHeight (& T, wxID_ANY, wxT ("set page height"), wxPoint (4, 30));
  wxStaticText textSetPageDesc (& T, wxID_ANY, wxT ("page desc"), wxPoint (4, 54));
  wxStaticLine lineSper (& T, wxID_ANY, wxPoint (4, 75), wxSize (144,-1));

  // set range.
  spinWidth.SetRange (1, 0xFFFF);
  spinHeight.SetRange (1, 0xFFFF);

  T.SetIcon ( wxIcon ( g_xpmpage_new));
  T.CenterOnParent ();
  int msgid = T.ShowModal();
  if (msgid == wxID_OK) {
    // ctab = 
    new cs_PageTileTab (attach, note_tab2, wxID_PAGE_NEW, spinWidth.GetValue (), spinHeight.GetValue (),textCtlPageDesc.GetValue ()  );
  }
}

void cs_mainFrame::OnAbout_wxWidgets (class wxCommandEvent& event) {

  wxDialog T (this, wxID_ANY, wxT ("About wxWidgets"));

  T.SetBackgroundColour ( wxSystemSettings::GetColour (wxSYS_COLOUR_WINDOW));
  T.SetSize (590, 500);
  T.SetIcon ( wxIcon ( g_xpmop_wxwidgets));
  T.CenterOnParent ();

  // set bitmap. 
  wxBitmap q (g_xpmwxwidgets_page, wxBITMAP_TYPE_XPM);
  wxBitmap q2 (g_xpmwxWidgets, wxBITMAP_TYPE_XPM);

  wxStaticBitmap p (& T, wxID_ANY, q, wxPoint (0, 0) );
  wxStaticBitmap p2  (& T, wxID_ANY, q2, wxPoint (0, 430) );

  // set text .
  wxChar *e =  wxT ( "wxWidgets was started in 1992 by Julian Smart at the University of Edinburgh.\n")
               wxT ( "Initially started as a project for creating applications that were\n")
               wxT ( "portable across Unix and Windows, it has grown to support Mac OS u, GTK+,\n")
               wxT ( "and many other toolkits and platforms (see the history page for more details).\n")
               wxT ( "The number of developers contributing to the project is now in the hundreds\n")
               wxT ( "and the toolkit has a strong userbase that includes everyone from open\n")
               wxT ( "source developers to corporations. So what is special about wxWidgets compared with other cross-platform GUI toolkits?\n\n")
               wxT ( "wxWidgets gives you a single, easy-to-use API for writing GUI applications on\n")
               wxT ( "multiple platforms that still utilize the native platform¡¯s controls and utilities.\n")
               wxT ( "Link with the appropriate library for your platform and compiler, and your application\n")
               wxT ( "will adopt the look and feel appropriate to that platform. On top of great GUI functionality,\n")
               wxT ( "wxWidgets gives you: online help, network programming, streams, clipboard and drag and drop, multithreading,\n")
               wxT ( "image loading and saving in a variety of popular formats, HTML viewing and printing, and much more.\n\n");
  wxChar *e2 = wxT (  "More about wxWidgets please read\n")
               wxT ( "and GitHub source code");

  wxStaticText s (& T, wxID_ANY, e, wxPoint (105,0), wxSize (466, 320));
  wxStaticText s2 (& T, wxID_ANY, e2, wxPoint (105, 350), wxSize (260, 40));

  // set hlink .
  wxHyperlinkCtrl c (& T, wxID_ANY, wxT ("https://www.wxwidgets.org"), wxT ("https://www.wxwidgets.org"), wxPoint (330, 350));
  wxHyperlinkCtrl c2 (& T, wxID_ANY, wxT ("https://github.com/wxWidgets/wxWidgets"), wxT ("https://github.com/wxWidgets/wxWidgets"), wxPoint (330, 368));

  // attach staticbiotmap .
  wxButton m (& p2, wxID_OK, wxT ("OK"), wxPoint (472, 6));
  T.ShowModal();
}

void cs_mainFrame::OnAbout_TileMap (class wxCommandEvent& event) {
  // wxCloseEvent ce;
  // this->Destroy ();
  wxMessageBox (wxT ("simple tile editors"), wxT ("tilemap_editors"), wxICON_NONE);
}

// our normal wxApp-derived class, as usual
class CApp : public wxApp {
public:
  
  bool OnInit()
  {
    wxInitAllImageHandlers();
    cs_mainFrame* frame = new cs_mainFrame(NULL);
    SetTopWindow(frame);
    frame->Show();
    return true;                    
  }
};

void cs_mainFrame:: StatusShowText () {  
  if (attach.cs_ViewStatus_ != nullptr)
    attach.cs_ViewStatus_->SetValue (statOut);
}
void cs_mainFrame:: StatusShowClearAppendBaseTextSetTemp (wxString &s) {  
  if (attach.cs_ViewStatus_ != nullptr) {
    // attach.cs_ViewStatus_->Clear();
    // GetVert.
    int n = attach.cs_ViewStatus_->GetScrollPos (wxSB_VERTICAL);
    wxString c (statOut);
    c.Append(s);
    attach.cs_ViewStatus_->SetValue (c);
  }
}
void cs_mainFrame::StatusOut (wxString & content) { 
  statOut.Append (content); 
  StatusShowText ();
}
void cs_mainFrame::StatusOutWrap (wxString & content) { 
  statOut.Append (content); 
#  if defined (_WIN32)
  statOut.Append (wxString ("\r\n")); 
#  else 
  statOut.Append (wxString ("\n")); 
#  endif 
}
void cs_mainFrame::StatusOutWrapShow (wxString & content) { 
  statOut.Append (content); 
#  if defined (_WIN32)
  statOut.Append (wxString ("\r\n")); 
#  else 
  statOut.Append (wxString ("\n")); 
#  endif 
  StatusShowText ();
}
void cs_mainFrame::OnFile_PageSave  (class wxCommandEvent& event) {

  if (this->ctab == nullptr)  {
    wxMessageBox (wxT ("no attach page"), wxT ("tilemap editors"), wxICON_WARNING);
    return  ;
  }

  wxDialog T (this, wxID_ANY, wxT ("Save Page"), wxPoint (0, 0), wxSize (160, 140) );
  wxButton butOk (& T, wxID_OK, wxT ("OK"), wxPoint (4, 80), wxSize (60, 24));
  wxButton butCancel (& T, wxID_CANCEL, wxT ("CANCEL"), wxPoint (80, 80), wxSize (60, 24));
  wxSpinCtrl spindispage (& T, wxID_ANY, wxEmptyString, wxPoint (104, 6), wxSize (48, 19));
  wxSpinCtrl spindisland (& T, wxID_ANY, wxEmptyString, wxPoint (104, 30), wxSize (48, 19));
  wxStaticText textSetWidth (& T, wxID_ANY, wxT ("dissetpage"), wxPoint (4, 6));
  wxStaticText textSetHeight (& T, wxID_ANY, wxT ("dissetland"), wxPoint (4, 30));
  wxStaticText textSetPageDesc (& T, wxID_ANY, wxT ("save type"), wxPoint (4, 54));
  wxRadioButton rbBut_CPP (& T, wxID_ANY, wxT ("CPP"), wxPoint (64, 54), wxSize(-1,-1), wxRB_GROUP);
  wxRadioButton rbBut_PNG (& T, wxID_ANY, wxT ("PNG"), wxPoint (111, 54), wxSize(-1,-1));
  wxStaticLine lineSper (& T, wxID_ANY, wxPoint (4, 75), wxSize (144,-1));

  // set range.
  spindispage.SetRange (0, 0xFF);
  spindisland.SetRange (0, 0xFF);

  T.SetIcon ( wxIcon ( g_xpmstchr));
  T.CenterOnParent ();
  int msgid = T.ShowModal();
  if (msgid == wxID_OK) {
    if (rbBut_PNG.GetValue () != false)  {
       // output png. 
        

    } else {
       // c source. 
      
      class wxString caption = wxString ("save c files.");
      class wxString wildcard = wxString ("c files(*.c)|*.c");
      class wxString defaultfn = wxEmptyString;
      class wxFileDialog  dialog  (this, caption, wxEmptyString, wxEmptyString, wildcard, wxFD_SAVE);
      if ( dialog.ShowModal () == wxID_OK)  {
        // create text files .
        wxString s1;
        wxTextFile t (dialog.GetPath ());
        wxUint8 dfp = spindispage.GetValue();
        wxUint8 dfl = spindisland.GetValue();

        t.Clear ();
        s1.Printf (wxT ("# define XXX_W %d"), ctab->chunkset.hnums);
        t.AddLine (s1);
        s1.Printf (wxT ("# define XXX_H %d"), ctab->chunkset.vnums);
        t.AddLine (s1);
        s1.Printf (wxT ("uint8_t ppta[] = { "));
        t.AddLine (s1);

        for (int y = 0; y != ctab->chunkset.vnums; y++)   {
            for (int x = 0; x != ctab->chunkset.hnums; x++)  {
               // 32 *  32 tile. 
              bool final = (x == ctab->chunkset.hnums - 1) && (y == ctab->chunkset.vnums -1);
              s1.Printf (wxT ("// PAGE-H %d PAGE-V %d"), x, y);
              t.AddLine (s1);
              tileChunkMap &q = ctab->chunkset.tcm[x+y*ctab->chunkset.hnums];
              for (int z = 0; z != 32; z++)   {
                wxString csp;
                for (int p = 0; p != 32; p++)  {
                  bool final2 = (z == 31) && (p == 31);
                  tileChunk &cc = q.map[p+z*32];
                  if (cc.enable == false || (cc.slot == nullptr))  
                    s1.Printf (wxT (" 0x%02X,0x%02X"), dfp, dfl);
                  else 
                    s1.Printf (wxT (" 0x%02X,0x%02X"), cc.slot->id, cc.idInSlot);
                  if (! (final2 && (final)))
                    s1.Append (wxT (","));
                  csp += s1;
                }
                t.AddLine (csp);
              }
            }
        }
        s1.Printf (wxT ("};"), ctab->chunkset.hnums);
        t.AddLine (s1);
        t.Write();
      }
    }
  }
}

void cs_mainFrame::OnFile_SaveSettings  (class wxCommandEvent& event) {

  // write into files.

  class wxString caption = wxString ("save epc files.");
  class wxString wildcard = wxString ("epc files(*.epc)|*.epc");
  class wxString defaultfn = wxEmptyString;
  class wxFileDialog  dialog  (this, caption, wxEmptyString, wxEmptyString, wildcard, wxFD_SAVE);
  if ( dialog.ShowModal () == wxID_OK)  {
    class wxString Path = dialog.GetPath ();
    class wxFile fd (Path, wxFile::write);
    fd.Seek (0);
    struct tile_file :: std_infos si;
    si.mg[0] = 0x8C;
    si.mg[1] = 0x9A;
    si.mg[2] = 0x68;
    si.mg[3] = 0x33;
    si.imgSlot_nums = 0;
    si.claySet_nums = 0;
    si.pageSet_nums = 0;
    si.claySetDesc_Offset = 0; 
    si.claySetChunk_Offset = 0;
    si.pageSetChunk_Offset = 0;
    si.pageSetDesc_Offset = 0;
    //  dummy ac chunk.
    wxUint32 nc;
    wxUint16 id;
    wxUint8 *ac = nullptr;
    struct tile_file :: dummyroot *sc = nullptr;
    const wxUint16 _max = sizeof (imgSlot_)/ sizeof (imgSlot_[0]);
    
    for  (id =0; id != _max; id++)  {
      if (imgSlot_[id].enable != false)  {
        si.imgSlot_nums ++;
      }
    }
    if  (si.imgSlot_nums != 0)  {     
      sc = new  tile_file :: dummyroot [si.imgSlot_nums];
      ac = new  wxUint8 [si.imgSlot_nums];
      for  (id =0, nc = 0; id != _max; id++)  {
        if (imgSlot_[id].enable != false)  {  
          wxImage &p = *imgSlot_[id].img2;
          ac[nc] = imgSlot_[id].id;
          for  (int y = 0; y != imgSlot::image_size_h; y++)  {
            for  (int x = 0; x != imgSlot::image_size_w; x++)  {
              int k = y * imgSlot::image_size_w + x;
              sc[nc].rgroup[k] = p.GetRed (x, y);
              sc[nc].ggroup[k] = p.GetGreen (x, y);
              sc[nc].bgroup[k] = p.GetBlue (x, y);
            }
          }
          nc ++;
        }
      }
    }

    wxString uvdesc, npdesc;
    std::list <tileChunkMap *>::iterator iter;
    tile_file ::RVA_uv_chunk *uv =nullptr;
    tile_file ::RVA_as_chunk *as =nullptr;
    tile_file ::RVA_as_chunk *as_ppblock =nullptr;
    tile_file :: RVA_np_chunk *np = nullptr;
    si.claySet_nums = fmset.size ();

    if (si.claySet_nums != 0)  {
      
      uv = new tile_file ::RVA_uv_chunk [si.claySet_nums];
      as = new tile_file ::RVA_as_chunk [si.claySet_nums];
      si.claySetDesc_Offset = sizeof (struct tile_file :: std_infos) 
                            +  (sizeof (wxUint8) + sizeof (tile_file :: dummyroot)) * si.imgSlot_nums
                            + sizeof (struct tile_file :: RVA_uv_chunk) * si.claySet_nums;

      for (id = 0, iter = fmset.begin(); iter != fmset.end(); id++, iter++)  
      {  
        uv[id].length = (*iter)->desc->size();
        uv[id].offset = si.claySetDesc_Offset + uvdesc.size () * 2;
        uvdesc.append ((*iter)->desc->wc_str(), (*iter)->desc->size());
      } 
      
      si.claySetChunk_Offset = si.claySetDesc_Offset + uvdesc.size () * 2;
      for (id = 0, iter = fmset.begin(); iter != fmset.end(); id++, iter++)  
      {  
        tileChunkMap *p = *iter;
        tile_file :: maptoas (as[id], *p);
      } 
    } else  {
       si.claySetDesc_Offset = sizeof (struct tile_file :: std_infos) 
                            +  (sizeof (wxUint8) + sizeof (tile_file :: dummyroot)) * si.imgSlot_nums;
       si.claySetChunk_Offset = si.claySetDesc_Offset;                  
    }
    
    //  enum page pane. 
    si.pageSet_nums = note_tab2->GetPageCount ();
    if  (si.pageSet_nums != 0)  {
      nc = 0;
      si.pageSetDesc_Offset = si.claySetChunk_Offset + sizeof (struct tile_file :: RVA_np_chunk) * si.pageSet_nums
                    +  si.claySet_nums * sizeof (struct tile_file :: RVA_as_chunk);
      np = new tile_file ::RVA_np_chunk [si.pageSet_nums];
      for (id = 0; id != si.pageSet_nums; id++)  {
        cs_PageTileTab *pt = (cs_PageTileTab *) note_tab2->GetPage (id);
        np[id].pg_Height = pt->chunkset.vnums;
        np[id].pg_Width = pt->chunkset.hnums;
        np[id].ch_length = pt->chunkset.desc.size ();
        np[id].ch_offset = si.pageSetDesc_Offset + npdesc.size () * 2;
        npdesc.append (pt->chunkset.desc.wc_str(), pt->chunkset.desc.size ());
        nc += (pt->chunkset.vnums  * pt->chunkset.hnums);
      }
      as_ppblock = new tile_file ::RVA_as_chunk [nc];
      tile_file ::RVA_as_chunk *ppc = as_ppblock;
      si.pageSetChunk_Offset = si.pageSetDesc_Offset + npdesc.size () *2;
      for (id = 0; id != si.pageSet_nums; id++)  {
        cs_PageTileTab *pt = (cs_PageTileTab *) note_tab2->GetPage (id);

        for  (int  y = 0; y != pt->chunkset.vnums; y++)  {
          for  (int  x = 0; x != pt->chunkset.hnums; x++)  {
            int a = y*pt->chunkset.hnums+x;
            tile_file :: maptoas (ppc[a], pt->chunkset.tcm[a]);
          }
        }
        ppc = & ppc[pt->chunkset.hnums*pt->chunkset.vnums];
      }
    } else {
      si.pageSetDesc_Offset = si.claySetChunk_Offset +  si.claySet_nums * sizeof (struct tile_file :: RVA_as_chunk);
      si.pageSetChunk_Offset = si.pageSetDesc_Offset;      
    }
    fd.Seek (0);
    fd.Write (& si, sizeof (si));
    if (si.imgSlot_nums != 0) fd.Write (ac, sizeof (wxUint8) * si.imgSlot_nums);
    if (si.imgSlot_nums != 0) fd.Write ( sc, sizeof (tile_file::dummyroot) * si.imgSlot_nums );
    if (si.claySet_nums != 0) fd.Write (uv, sizeof (tile_file::RVA_uv_chunk) * si.claySet_nums);
    if (si.claySet_nums != 0) fd.Write ( uvdesc.wc_str(), uvdesc.size () *2 );
    if (si.claySet_nums != 0) fd.Write ( as, sizeof (tile_file::RVA_as_chunk) * si.claySet_nums );
    if (si.pageSet_nums != 0) fd.Write ( np, sizeof (tile_file::RVA_np_chunk) * si.pageSet_nums );
    if (si.pageSet_nums != 0) fd.Write ( npdesc.wc_str(), npdesc.size () *2 );
    if (si.pageSet_nums != 0) fd.Write ( as_ppblock, sizeof (tile_file::RVA_as_chunk) *nc );
  }
}

void cs_mainFrame::OnFile_LoadSettings  (class wxCommandEvent& event) {

  wxString caption = wxString ("choose files.");
  wxString wildcard = wxString ("epc files(*.epc)|*.epc");
  wxString defaultfn = wxEmptyString;
  wxFileDialog  dialog  (this, caption, wxEmptyString, wxEmptyString, wildcard, wxFD_OPEN);
  if ( dialog.ShowModal () == wxID_OK)  {
    wxString Path = dialog.GetPath ();
    wxFile fd (Path, wxFile::read);
    fd.Seek (0);
    struct tile_file :: std_infos si;
    struct tile_file ::dummyroot *trs;
    fd.Read (& si, sizeof (si));
    wxUint8 idchunk[256];
    fd.Read (& idchunk[0], si.imgSlot_nums);
    trs = new tile_file ::dummyroot [si.imgSlot_nums];
    fd.Read (trs, sizeof (struct tile_file ::dummyroot) * si.imgSlot_nums);
    const wxUint16 _max = sizeof (imgSlot_)/ sizeof (imgSlot_[0]);
    int clayUv_Offset = sizeof (struct tile_file :: std_infos) + si.imgSlot_nums * (sizeof (wxUint8) + sizeof (tile_file :: dummyroot));

    //  clear slot.
    for  (int id =0; id != _max; id++)  {
      imgSlot_[id].uninit ();
    }

    //  fill slot.
    for  (int id =0; id != si.imgSlot_nums; id++)  {
      imgSlot_[id].enable = true;
      imgSlot_[id].id = idchunk[id];
      imgSlot_[id].strPath = nullptr;
      imgSlot_[id].img2 = new wxImage (imgSlot::image_size_w, imgSlot::image_size_h);
      // dummy image.
      for  (int y = 0; y != imgSlot::image_size_h; y++)  {
        for  (int x = 0; x != imgSlot::image_size_w; x++)  {
          int n = y * imgSlot::image_size_w + x;
          imgSlot_[id].img2->SetRGB (x, y, trs[id].rgroup[n], trs[id].ggroup[n], trs[id].bgroup[n]);
        }
      }
      imgSlot_[id].img = new wxBitmap (*imgSlot_[id].img2);
    }
    note_tab2->DeleteAllPages();
    std::list <tileChunkMap *>::iterator iter;
    for (iter = fmset.begin(); iter != fmset.end(); iter++)  
    {  
      delete *iter;
    } 
    fmset.clear ();
    for (int id =0; id != si.claySet_nums; id++) {  
      tile_file::RVA_uv_chunk uc;
      tileChunkMap *ap = new  tileChunkMap;
      ap->init ();
      fd.Seek (clayUv_Offset + id* sizeof (tile_file::RVA_uv_chunk));
      fd.Read (& uc, sizeof (uc));
      fd.Seek (uc.offset);
      wchar_t *c = new wchar_t [uc.length];
      fd.Read (c, uc.length *2);
      ap ->desc = new wxString;
      ap ->desc->append (c, uc.length);
      tile_file::RVA_as_chunk dt;
      fd.Seek (si.claySetChunk_Offset + id* sizeof (tile_file::RVA_as_chunk));
      fd.Read (& dt, sizeof (tile_file::RVA_as_chunk));
      tile_file :: astomap (*ap, dt, *this);
      fmset.push_back (ap);
    } 
    // add page.  
    for (int id =0, nc = 0; id != si.pageSet_nums; id++) {  
      // np chunk read.
      tileChunkSet tsc;
      tile_file ::RVA_np_chunk np;
      fd.Seek (si.claySetChunk_Offset + sizeof (tile_file :: RVA_as_chunk) *si.claySet_nums + id  *sizeof (np));
      fd.Read (& np, sizeof (np));
      fd.Seek (np.ch_offset);
      wchar_t *c= new wchar_t[np.ch_length];
      fd.Read (c, np.ch_length * 2);
      tsc.desc.append (c, np.ch_length);
      tsc.vnums = np.pg_Height;
      tsc.hnums = np.pg_Width;
      tsc.tcm = new tileChunkMap [tsc.hnums * tsc.vnums];
      struct tile_file ::RVA_as_chunk *pn = new tile_file ::RVA_as_chunk [tsc.hnums * tsc.vnums];
      fd.Seek (si.pageSetChunk_Offset + nc * sizeof (tile_file::RVA_as_chunk));
      fd.Read (pn, sizeof (tile_file ::RVA_as_chunk) * tsc.hnums * tsc.vnums);
      nc += (np.pg_Height * np.pg_Width);
      // as to map.
      for (int y = 0; y != tsc.vnums; y++)  {
        for (int x = 0; x != tsc.hnums; x++)  {
          int k = y * tsc.hnums + x;
          tile_file :: astomap (tsc.tcm[k], pn[k], *this);
        }    
      }
      new cs_PageTileTab (attach, note_tab2, wxID_PAGE_NEW, tsc.hnums, tsc.vnums, tsc.desc, & tsc  );
    } 
    // close root window..
    if  (fd_ViewRootTexture_ != nullptr)  {
      fd_ViewRootTexture_->DeleteAllPages ();
      aui_Manager.ClosePane (aui_Manager.GetPane (fd_ViewRootTexture_));
      GetMenuBar()->Enable (wxID_BTEXTURE, true);
      fd_ViewRootTexture_ = nullptr;
      //  append. new.
      wxCommandEvent emt;
      OnView_BTexture  (emt);
    }
    // close clay window..
    if  (fd_ViewClaySet_ != nullptr)  {
      fd_ViewClaySet_->DeleteAllPages ();
      aui_Manager.ClosePane (aui_Manager.GetPane (fd_ViewClaySet_));
      GetMenuBar()->Enable (wxID_CLAY, true);
      fd_ViewClaySet_ = nullptr;
      // append. new.
      wxCommandEvent emt;
      OnView_Clay (emt);
    }
  }
}

DECLARE_APP(CApp);
IMPLEMENT_APP(CApp);