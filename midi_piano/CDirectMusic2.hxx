//  CDirectMusic2.hxx 
//  Provide Basic midi input and output (no midi keyboard input).
# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

//  lots of code from 
// 
//  <1> DirectMidi Librarary  http://directmidi.sourceforge.net/ 
//  <2> Mircosoft DirectX 9.0b SDK (with directmusic version) source code samples.
//  <3> codeproject's project https://www.codeproject.com/Articles/2651/Developing-MIDI-applications-with-DirectMusic
//
//    thanks them !
// 
//  I don't know much about DirectMusic and MIDI. :( 
// The code doesn't write very well.   soory             - moecmks 

# if !defined (_CDIRECTMUSIC2_INCLUED_HEADER_CXX_)
#  define _CDIRECTMUSIC2_INCLUED_HEADER_CXX_

# define INITGUID

# include <atldef.h>  // for ATLASSERT 
# include <initguid.h>
# include <dmksctrl.h>
# include <directx/include/dsound.h>
# include <directx/include/dmusicc.h>
# include <directx/include/dmusici.h>
# include <directx/include/dmusics.h>
# include <directx/include/dmusicf.h>
# include <directx/include/dmusbuff.h>
# include <directx/include/dmplugin.h>

class CDirectMusic2 {
public:
  struct DSFX_InitParam {
    const GUID  *IID_object;
    const GUID  *GUID_object;
    DWORD dwSize;
  };
  
private:
  // Global CDirectMusic2 COM root and settings. 
  static IDirectMusic *sm_DirectMusic_root;
  static IDirectSound8 *sm_DirectSound_root;
  static const int CURRENT_USE_EFFECT_TOTAL = 9;
  static DSFX_InitParam sm_DSFX_InitParam[CURRENT_USE_EFFECT_TOTAL];

public:
  static BOOL CALLBACK DSEnumCallback__(
                            LPGUID  lpGuid,    
                            LPCTSTR  lpcstrDescription,  
                            LPCTSTR  lpcstrModule,   
                            LPVOID  lpContext )

  {
    //  Enum audio device, for test .
    return TRUE;
  }

  // Global init for all CDirectMusic2 object 
  static void InitDirectMusic (HWND hWindow)  {
    HRESULT  sig  = CoCreateInstance (CLSID_DirectMusic, NULL,
          CLSCTX_INPROC, IID_IDirectMusic8,(void**)& sm_DirectMusic_root);
    ATLASSERT (SUCCEEDED (sig));

    // enum device. 
    // DirectSoundEnumerate (DSEnumCallback__, NULL);

    sig  = DirectSoundCreate8 (NULL,  & sm_DirectSound_root, NULL);
    ATLASSERT (SUCCEEDED (sig));
    sig  = sm_DirectSound_root->SetCooperativeLevel (hWindow, DSSCL_PRIORITY);
    ATLASSERT (SUCCEEDED (sig));
    sig =  sm_DirectMusic_root->SetDirectSound (sm_DirectSound_root, hWindow);
    ATLASSERT (SUCCEEDED (sig));
  }
  // Global uninit for all CDirectMusic2 object
  static void UninitDirectMusic (void)  {
    release_com<IDirectSound8>(sm_DirectSound_root);
    release_com<IDirectMusic>(sm_DirectMusic_root);
  }

public:
  //  pos in group index. 
  enum IDIRECTEFFECTS_POS_ {
    IDIRECTEFFECTS_POS_CHORUS = 0,
    IDIRECTEFFECTS_POS_COMPRESSOR,    
    IDIRECTEFFECTS_POS_DISTORTION,      
    IDIRECTEFFECTS_POS_ECHO,    
    IDIRECTEFFECTS_POS_FLANGER,
    IDIRECTEFFECTS_POS_GARGLE,    
    IDIRECTEFFECTS_POS_PARAMEQ,    
    IDIRECTEFFECTS_POS_REVERB,
    IDIRECTEFFECTS_POS_REVERB_LEVEL2
  };
  //  standard directsound effects .
  union DSFX_desc {
    DSFXChorus      efs_chorus;
    DSFXCompressor  efs_compressor;
    DSFXDistortion  efs_distortion;
    DSFXEcho        efs_echo;
    DSFXFlanger     efs_flanger;
    DSFXGargle      efs_gargle;
    DSFXParamEq     efs_paramEq;
    DSFXWavesReverb efs_reverb;
    DSFXI3DL2Reverb efs_reverb_level2;
  };  
  //  FX sound object set.
  union DSFX_object {
    IDirectSoundFXChorus8 *     efs_chorus;
    IDirectSoundFXCompressor8 * efs_compressor;
    IDirectSoundFXDistortion8 * efs_distortion;
    IDirectSoundFXEcho8 *       efs_echo;
    IDirectSoundFXFlanger8 *    efs_flanger;
    IDirectSoundFXGargle8 *     efs_gargle;
    IDirectSoundFXParamEq8 *    efs_paramEq;
    IDirectSoundFXWavesReverb8 *efs_reverb;
    IDirectSoundFXI3DL2Reverb8 *efs_reverb_level2;
    IUnknown *com_pointer;
    void *common_pointer;
  };

  struct DSFX_Reverb2 {
    DSFXI3DL2Reverb desc;
    int Quality;
    int Type;
  };
  //  for use . 
  struct DSFX_vec {
    DSFX_desc desc;
    DSFX_object object;
    DSFX_InitParam *init_param;
    BOOL enable;
  };
private:
  //  class object var. 
  DSFX_vec DSFX_vec_[CURRENT_USE_EFFECT_TOTAL];

  //  directmusic object pointer .
  IDirectSoundBuffer *                    m_pDSBuf;                       
  IDirectSoundBuffer8 *                   m_pDSBuf8;
  IDirectMusicLoader8 *                   m_pLoader;                        
  IDirectMusicDownloadedInstrument8 *     m_pDLSInstrument; // DLS instructments
  IDirectMusicInstrument8 *               m_pInstrument;    // instructments set
  IDirectMusicPort8 *                     m_pOutPort;
  IDirectMusicCollection8 *               m_pCollection;
  IDirectMusicBuffer8 *                   m_pDMBuf8;
  IReferenceClock *                       m_pClock; 

  std::vector <CString> m_vecCollectionDLS;                 // DLS cache from files. 
  std::vector <CString> m_vecInstrInDLS; 

  DMUS_PORTPARAMS8         m_OutParams;

  DWORD m_SelDLSIndex;
  DWORD m_SelInstructments;
public:
  //   export   private   object  method . 
  void  Get_vecCollectionDLS (std::vector <CString> *& p) {  p = & m_vecCollectionDLS; }
  void  Get_vecInstrInDLS (std::vector <CString> *&p) {  p = & m_vecInstrInDLS; }  
public:
  // deconstruct
 ~CDirectMusic2 (void) {
   ReleaseInstructment ();
   ReleaseCollection ();

   HRESULT sig = m_pOutPort->Activate (FALSE);
   ATLASSERT (SUCCEEDED (sig));

   // Release effects .
   for (DWORD id =0; id != CURRENT_USE_EFFECT_TOTAL; id++) {
     DSFX_vec_[id].init_param = & sm_DSFX_InitParam[id];
     DSFX_vec_[id].enable = FALSE;
     DSFX_vec_[id].object.com_pointer->Release ();
     DSFX_vec_[id].object.common_pointer = NULL;
   }
   sig = m_pDSBuf8->Stop ();
   ATLASSERT (SUCCEEDED (sig));

   sig = m_pDSBuf8->SetFX (0, NULL, NULL);
   ATLASSERT (SUCCEEDED (sig));

   release_com <IReferenceClock> ( m_pClock);
   release_com <IDirectMusicLoader8> ( m_pLoader);
   release_com <IDirectMusicBuffer8> ( m_pDMBuf8);
   release_com <IDirectMusicPort8> ( m_pOutPort);
   release_com <IDirectSoundBuffer8> ( m_pDSBuf8);
   release_com <IDirectSoundBuffer> ( m_pDSBuf);
  }
  //   construct 
  CDirectMusic2 (void) {

    m_pDSBuf = NULL;
    m_pDSBuf8 = NULL;
    m_pLoader = NULL;
    m_pOutPort = NULL;
    m_pDMBuf8 = NULL;
    m_pClock = NULL;
    m_pDLSInstrument = NULL;
    m_pInstrument = NULL;

    ZeroMemory (& m_OutParams, sizeof (DMUS_PORTPARAMS8));
    m_OutParams.dwSize = sizeof (m_OutParams);

    //  create IDirectMusicBuffer8 interface.
    DMUS_BUFFERDESC dmbuf_desc;                   // Create the DirectMusic buffer to store MIDI messages
    ZeroMemory (& dmbuf_desc, sizeof (DMUS_BUFFERDESC));
    dmbuf_desc.dwSize = sizeof (DMUS_BUFFERDESC);
    dmbuf_desc.guidBufferFormat = GUID_NULL;
    dmbuf_desc.cbBuffer = DMUS_EVENT_SIZE (32);    // at least 32 bytes to store messages
    
    HRESULT sig = sm_DirectMusic_root->CreateMusicBuffer (& dmbuf_desc,& m_pDMBuf8, NULL);
    ATLASSERT (SUCCEEDED (sig));

    //  create loader 
    sig = CoCreateInstance(CLSID_DirectMusicLoader,NULL,
        CLSCTX_INPROC_SERVER,IID_IDirectMusicLoader8, (LPVOID *)& m_pLoader);
    ATLASSERT (SUCCEEDED (sig));

    ZeroMemory (& DSFX_vec_[0], sizeof (DSFX_vec_));

    //  load effects global settings.
    for (DWORD id =0; id != CURRENT_USE_EFFECT_TOTAL; id++) {
      DSFX_vec_[id].init_param = & sm_DSFX_InitParam[id];
      DSFX_vec_[id].enable = FALSE;
      DSFX_vec_[id].object.common_pointer = NULL;
    }
    
    //  enum outport. 
    DMUS_PORTCAPS portinf;
    DWORD dwIndex = 0;
    HRESULT hr = -1;
 
    ZeroMemory (& portinf, sizeof(portinf));
    portinf.dwSize = sizeof (DMUS_PORTCAPS);
     
    while ((hr = sm_DirectMusic_root->EnumPort (dwIndex++, & portinf)) == S_OK) { 
      
      if (portinf.dwClass == DMUS_PC_OUTPUTCLASS) {
        if (portinf.dwFlags & DMUS_PC_DLS2)  {
          // Create DLS output .
          ZeroMemory (& m_OutParams, sizeof (m_OutParams));
          m_OutParams.dwSize = sizeof (m_OutParams);

          HRESULT sig = sm_DirectMusic_root->CreatePort (portinf.guidPort, & m_OutParams, & m_pOutPort, NULL);
          ATLASSERT (SUCCEEDED (sig));

          // get clock 
          sig = m_pOutPort->GetLatencyClock (& m_pClock);
          ATLASSERT (SUCCEEDED (sig));

          //  XXX: ext wave memory size. 
          struct WAVEFORMATEX__ {
            WAVEFORMATEX wave_;
            BYTE ext[256];
          } wave2;
          DWORD dwExtWaveSize;
          DWORD dwBufferSize;
          ZeroMemory (& wave2.wave_, sizeof (wave2.wave_)); 
          sig = m_pOutPort->GetFormat (& wave2.wave_, & dwExtWaveSize, & dwBufferSize);
          ATLASSERT (SUCCEEDED (sig));

          // create directsound buffer attach to directmusic port..
          DSBUFFERDESC dsbuf_desc;
          ZeroMemory (& dsbuf_desc, sizeof (DSBUFFERDESC)); 
          dsbuf_desc.dwSize = sizeof ( DSBUFFERDESC );
#  if     1
          dsbuf_desc.dwFlags =   DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFX | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS; 
#  else 
          dsbuf_desc.dwFlags =   DSBCAPS_LOCHARDWARE | DSBCAPS_CTRLFX | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS; 
#  endif 
          dsbuf_desc.dwBufferBytes = dwBufferSize; 
          dsbuf_desc.lpwfxFormat = & wave2.wave_; 

          sig = sm_DirectSound_root->CreateSoundBuffer (& dsbuf_desc, & m_pDSBuf, NULL);
          ATLASSERT (SUCCEEDED (sig));

          // get IDirectSoundBuffer8 from IDirectSoundBuffer
           
          sig = m_pDSBuf->QueryInterface (IID_IDirectSoundBuffer8, (LPVOID *) & m_pDSBuf8);
          ATLASSERT (SUCCEEDED (sig));

          m_pDSBuf8->SetVolume (DSBVOLUME_MAX);

          m_pOutPort->SetDirectSound (sm_DirectSound_root, m_pDSBuf8);

          //  Get default dls
          DMUS_OBJECTDESC dmusdesc;

          ZeroMemory(& dmusdesc, sizeof (DMUS_OBJECTDESC));
          dmusdesc.dwSize = sizeof (DMUS_OBJECTDESC);
          dmusdesc.guidClass = CLSID_DirectMusicCollection;
          dmusdesc.guidObject = GUID_DefaultGMCollection;
          dmusdesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_OBJECT;
 
          ATLASSERT (m_pLoader != NULL);
          sig = m_pLoader->GetObject (& dmusdesc, IID_IDirectMusicCollection, (LPVOID *)& m_pCollection);
          ATLASSERT (SUCCEEDED (sig));

          m_vecCollectionDLS.push_back (_T ("default dls"));

          EnumDLSInstructment (NULL);
          AdjustInstructmentsIndex (47);
          return  ;
        }
      }
    }
    //  never reach here. 
    ATLASSERT (FALSE);
  }
  
  void EnumDLSInstructment (/* not release */ std::vector <CString> **instructments) {
    ATLASSERT (m_pCollection != NULL);

    m_vecInstrInDLS.clear ();
    DWORD id = 0;

    while  (TRUE)  {
      DWORD patch;
      TCHAR szBuf[MAX_PATH], szBuf2[MAX_PATH];
      HRESULT sig = m_pCollection->EnumInstrument (id ++, & patch, szBuf, sizeof (szBuf)/ sizeof (szBuf[0]));
      ATLASSERT (sig == S_FALSE || (sig == S_OK));   
      if  (sig == S_FALSE)  break  ;
      _stprintf (szBuf2, _T ("%d %s"), id, szBuf); 
      m_vecInstrInDLS.push_back (szBuf2);
    } 
    if (instructments != NULL)
      * instructments = & m_vecInstrInDLS;
  }
  
  //  adjust  InstructIndex 
  void AdjustInstructmentsIndex (DWORD instruct_id = 0)  {

    ATLASSERT (m_pCollection != NULL);
    BYTE bSelInstruct = (BYTE) (m_SelInstructments = instruct_id);

    ReleaseInstructment ();

    WCHAR szBuf[MAX_PATH];   
    DWORD patch;
    HRESULT sig = m_pCollection->EnumInstrument (m_SelInstructments, & patch, szBuf, sizeof (szBuf)/ sizeof (szBuf[0]));
    ATLASSERT (SUCCEEDED (sig));

    sig = m_pCollection->GetInstrument (patch, & m_pInstrument);
    ATLASSERT (SUCCEEDED (sig));
    sig = m_pInstrument->SetPatch (1);
    ATLASSERT (SUCCEEDED (sig));

    DMUS_NOTERANGE t = { 0, 127 };                        // standard midi range, 
    sig = m_pOutPort->DownloadInstrument (m_pInstrument,
                              & m_pDLSInstrument, & t, 1);// octave group := 1 ...
    ATLASSERT (SUCCEEDED (sig));

    bSelInstruct = 1;

    sig = m_pOutPort->Activate (FALSE);
    ATLASSERT (SUCCEEDED (sig));

    sig = m_pOutPort->Activate (TRUE);
    ATLASSERT (SUCCEEDED (sig));

    SendMidiMsg (EncodeMidiMessage (0xC0, 0, 1, 0));
  }
  
  // adjust  CollectionIndex 
  int AdjustCollectionIndex (DWORD collection_id = 0, DWORD instruct_id = 0)  {
    if  (TRUE /*isDirectMuscSoftSynthesizer () != FALSE*/)  {
      ReleaseInstructment ();
      ReleaseCollection ();

      // get collection 
      int res = 0;
      HRESULT sig =0;
      // Load default DLS .
      if  (collection_id  == 0)
        LoadDefaultDLSCollection ();
      else  
        res = LoadDLSCollection (collection_id);
      if (res != 0) 
        LoadDefaultDLSCollection ();
      m_SelInstructments = instruct_id;
      m_SelDLSIndex = collection_id;
      
      EnumDLSInstructment (NULL);
      AdjustInstructmentsIndex (m_SelInstructments);

      return res;
    }
    //  never reach here .
    ATLASSERT (FALSE);
  }

  void GetEffectDesc_Really ( IDIRECTEFFECTS_POS_ pos)  {
    DSFX_vec &t= DSFX_vec_[pos];
    HRESULT sig = -1;
    //   UnactivateOutport ();
# define  PRIVATE_UNWIND(_pos, _dummy)                                                \
    case (_pos):                                                                      \
      sig = t.object.efs_##_dummy##->GetAllParameters (& t.desc.efs_##_dummy##);      \
    break;
    switch (pos )  {
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_CHORUS, chorus);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_COMPRESSOR, compressor);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_DISTORTION, distortion);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_ECHO, echo);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_FLANGER, flanger);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_GARGLE, gargle);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_PARAMEQ, paramEq);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_REVERB, reverb);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_REVERB_LEVEL2, reverb_level2);
# undef PRIVATE_UNWIND
      default: ATLASSERT (FALSE); break;
    }
    ATLASSERT (SUCCEEDED (sig));
  }

  void SetEffectDesc_Really ( IDIRECTEFFECTS_POS_ pos)  {
    DSFX_vec &t= DSFX_vec_[pos];
# define  PRIVATE_UNWIND(_pos, _dummy)                                          \
    case (_pos):                                                                \
      t.object.efs_##_dummy->SetAllParameters (& t.desc.efs_##_dummy);          \
    break;
    switch (pos )  {
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_CHORUS, chorus);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_COMPRESSOR, compressor);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_DISTORTION, distortion);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_ECHO, echo);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_FLANGER, flanger);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_GARGLE, gargle);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_PARAMEQ, paramEq);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_REVERB, reverb);
      PRIVATE_UNWIND (IDIRECTEFFECTS_POS_REVERB_LEVEL2, reverb_level2);
# undef PRIVATE_UNWIND
      default: ATLASSERT (FALSE); break;
    }
  }

  //  group .
  void GetEffectDesc ( IDIRECTEFFECTS_POS_ pos, DSFX_desc &pp)  {
    memcpy  ( & pp , & DSFX_vec_[pos].desc, DSFX_vec_[pos].init_param->dwSize);
  }
  void SetEffectEnable (IDIRECTEFFECTS_POS_ pos, BOOL enable) {
    DSEFFECTDESC effect_gp[CURRENT_USE_EFFECT_TOTAL];
    DSFX_vec *effect_p[CURRENT_USE_EFFECT_TOTAL];
    DWORD dwScanSet = 0;
    BOOL enable_old = DSFX_vec_[pos].enable;
    DSFX_vec_[pos].enable = enable;
    ZeroMemory ( effect_gp, sizeof (effect_gp));
    if  (enable_old == FALSE && (enable == FALSE))
      return  ;
    if  (enable_old != FALSE && enable != FALSE)   
      return  ;

    for ( int id = 0; id != CURRENT_USE_EFFECT_TOTAL; id++)  {
      if  ( DSFX_vec_[id].enable != FALSE)  {
         // scan into array .
        effect_gp[dwScanSet].dwFlags = DSFX_LOCSOFTWARE;
        effect_gp[dwScanSet].dwSize = sizeof (DSEFFECTDESC);
        memcpy ( & effect_gp[dwScanSet].guidDSFXClass, 
             DSFX_vec_[id].init_param->GUID_object,
              sizeof (GUID));
        effect_p[dwScanSet] = & DSFX_vec_[id];
        dwScanSet ++;
      }  else  {
           // diable it. 
        release_com<IUnknown> (DSFX_vec_[id].object.com_pointer);
      }
    }
    //   make Unactivate
    HRESULT sig = m_pOutPort->Activate (FALSE);
    ATLASSERT (SUCCEEDED (sig));
    DWORD result[CURRENT_USE_EFFECT_TOTAL];
    if  ( dwScanSet == 0)  {
      sig = m_pDSBuf8->SetFX (0, NULL, NULL);
      ATLASSERT (SUCCEEDED (sig));
      sig = m_pOutPort->Activate (TRUE);
      ATLASSERT (SUCCEEDED (sig));
      return   ;
    }
    sig = m_pDSBuf8->SetFX (dwScanSet, & effect_gp[0], & result[0]);
    ATLASSERT (SUCCEEDED (sig));

    //  GetEffectObject 
    for ( int id = 0; id != dwScanSet; id++)  {
      DSFX_vec *p = effect_p[id];
              HRESULT sig = m_pDSBuf8->GetObjectInPath (* (p->init_param->GUID_object), 0, 
               *(p->init_param->IID_object), (void **)&p->object.common_pointer);
      ATLASSERT (SUCCEEDED (sig));
    }
    sig = m_pOutPort->Activate (TRUE);
    ATLASSERT (SUCCEEDED (sig)); 
  }
  void SetEffectDesc ( IDIRECTEFFECTS_POS_ pos, DSFX_desc *p)  {
    memcpy  ( & DSFX_vec_[pos].desc, p, DSFX_vec_[pos].init_param->dwSize);
    SetEffectEnable (pos, TRUE);
    SetEffectDesc_Really (pos);
  }
  void SetReverb_L2Quality (LONG i) {
    if ( DSFX_vec_[IDIRECTEFFECTS_POS_REVERB_LEVEL2].object.efs_reverb_level2 != NULL) {
      HRESULT sig = DSFX_vec_[IDIRECTEFFECTS_POS_REVERB_LEVEL2].object.efs_reverb_level2->SetQuality (i);
      ATLASSERT (SUCCEEDED (sig));
    }
  }
  void SetReverb_L2Type (DWORD type) {
    if ( DSFX_vec_[IDIRECTEFFECTS_POS_REVERB_LEVEL2].object.efs_reverb_level2 != NULL) {
      HRESULT sig = DSFX_vec_[IDIRECTEFFECTS_POS_REVERB_LEVEL2].object.efs_reverb_level2->SetPreset (type);
      ATLASSERT (SUCCEEDED (sig));
    }
  }

  int Get_Reverb_L2 (struct DSFX_Reverb2 *eb2)  {
    IDirectSoundFXI3DL2Reverb8 *p = DSFX_vec_[IDIRECTEFFECTS_POS_REVERB_LEVEL2].object.efs_reverb_level2;
    // Get base param.
    HRESULT sig = p->GetAllParameters (& eb2->desc);
    ATLASSERT (SUCCEEDED (sig));
    sig = p->GetQuality ((LONG *)& eb2->Quality);
    ATLASSERT (SUCCEEDED (sig));
    //sig = p->GetPreset ((DWORD *)& eb2->Type);
    //ATLASSERT (SUCCEEDED (sig));
    return 0;
  }

public:
  BOOL LoadCollectionString (TCHAR *dls_filename) {
    m_vecCollectionDLS.push_back ( dls_filename);
  }
  void ResetCollectionString (void) {
    m_vecCollectionDLS.clear ();
    m_vecCollectionDLS.push_back (_T("default dls"));
  }
  void DeleteCollectionString (int id) {
    ATLASSERT (id < m_vecCollectionDLS.size ());
    if  ( id == 0) return   ;
    m_vecCollectionDLS.erase (m_vecCollectionDLS.begin()+ id);
  }
  void LoadDefaultDLSCollection (void)  {
    HRESULT sig = -1;
    DMUS_OBJECTDESC dmusdesc;
 
    ReleaseInstructment ();
    ReleaseCollection ();

    ZeroMemory(& dmusdesc, sizeof (DMUS_OBJECTDESC));
    dmusdesc.dwSize = sizeof (DMUS_OBJECTDESC);
    dmusdesc.guidClass = CLSID_DirectMusicCollection;
    dmusdesc.guidObject = GUID_DefaultGMCollection;
    dmusdesc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_OBJECT;
 
    ATLASSERT (m_pLoader != NULL);
    sig = m_pLoader->GetObject (& dmusdesc, IID_IDirectMusicCollection, (void**)& m_pCollection);
    ATLASSERT (SUCCEEDED (sig));
    // send midi 
  }

  HRESULT SendMidiMsg (DWORD dwMsg) {
    HRESULT sig = -1;
    REFERENCE_TIME rt;
    //if  ( isOutportActivate () == FALSE)
    //  ActivateOutport ();
    rt = 0;
    sig = m_pClock->GetTime (&rt);
    //ATLASSERT (SUCCEEDED (sig));
    sig = m_pDMBuf8->PackStructured (rt, m_OutParams.dwChannelGroups, dwMsg);
    //ATLASSERT (SUCCEEDED (sig));
    sig = m_pOutPort->PlayBuffer (m_pDMBuf8);
    //ATLASSERT (SUCCEEDED (sig));
    sig = m_pDMBuf8->Flush ();
    //ATLASSERT (SUCCEEDED (sig));
   
    return sig;
  }

  // For instrument's loop note. 
  HRESULT ClearNoise (void) {
    HRESULT sig = m_pOutPort->Activate (FALSE);
    ATLASSERT (SUCCEEDED (sig));
    sig = m_pOutPort->Activate (TRUE);
    ATLASSERT (SUCCEEDED (sig));
    return 0; //ewruen
  }

  int LoadDLSCollection (int dls_id)  {
    ATLASSERT (m_pCollection == NULL);

    if  (dls_id == 0)  {
      //   load default's DLS collection  
      LoadDefaultDLSCollection  ();
      return  0;
    } else  {
      HRESULT sig = -1;
      DMUS_OBJECTDESC dmusdesc;
      // Sets to 0 the DMUS structure
    
      ReleaseInstructment ();
      ReleaseCollection ();

      ZeroMemory(& dmusdesc,sizeof(DMUS_OBJECTDESC));
      
  # if defined (_UNICODE)
      wcscpy (dmusdesc.wszFileName, m_vecCollectionDLS[dls_id]);
  # else
      mbstowcs (dmusdesc.wszFileName, m_vecCollectionDLS[dls_id], DMUS_MAX_FILENAME);
  # endif
      dmusdesc.dwSize = sizeof(DMUS_OBJECTDESC);
      dmusdesc.guidClass = CLSID_DirectMusicCollection;
      dmusdesc.guidObject = GUID_DefaultGMCollection;
      dmusdesc.dwValidData = DMUS_OBJ_CLASS  | DMUS_OBJ_FILENAME  | DMUS_OBJ_FULLPATH;

      ATLASSERT (m_pLoader != NULL);
      sig = m_pLoader->GetObject (& dmusdesc, IID_IDirectMusicCollection, (LPVOID FAR *)& m_pCollection);
      if   (SUCCEEDED (sig))  return  0;
      else LoadDefaultDLSCollection ();
      return -1;
    }

  }
  // https://www.midi.org/specifications/item/table-1-summary-of-midi-message
  //
  // typedef struct DMUS_MIDI_PMSGG {
  // DMUS_PMSG_PART
  // BYTE  bStatus;
  // BYTE  bByte1;
  // BYTE  bByte2;
  // BYTE  bPad[1];
  // } DMUS_MIDI_PMSG;
 
  DWORD EncodeMidiMessage (BYTE cmd, BYTE chan, BYTE data, BYTE data2) {
    DWORD msg;
    msg = cmd | chan;
    msg |=  (  data << 8);
    msg |= (data2 << 16);
    return msg;
  }
  DWORD EncodeMidiMessage (BYTE status, BYTE data, BYTE data2) {
    DWORD msg = status;
    msg |=  (  data << 8);
    msg |= (data2 << 16);
    return msg;
  }
public:
  void ReleaseCollection (void)  {
    if (m_pLoader && m_pCollection)  {
      m_pLoader->ReleaseObjectByUnknown (m_pCollection);
      release_com<IDirectMusicCollection8> (m_pCollection);
    }
  }
  void ReleaseInstructment (void)  {
    if (m_pDLSInstrument && m_pOutPort)  {
      m_pOutPort->UnloadInstrument (m_pDLSInstrument);
      release_com<IDirectMusicDownloadedInstrument8> (m_pDLSInstrument);
    }
    release_com<IDirectMusicInstrument8> (m_pInstrument);
  }
};
// init root. int static private
IDirectMusic *CDirectMusic2::sm_DirectMusic_root = nullptr;
IDirectSound8 *CDirectMusic2::sm_DirectSound_root = nullptr;
//  guid settings.  
CDirectMusic2::DSFX_InitParam CDirectMusic2::sm_DSFX_InitParam[CURRENT_USE_EFFECT_TOTAL] = { 
  { & IID_IDirectSoundFXChorus8,     & GUID_DSFX_STANDARD_CHORUS,     sizeof (DSFXChorus)  },
  { & IID_IDirectSoundFXCompressor8, & GUID_DSFX_STANDARD_COMPRESSOR, sizeof (DSFXCompressor) },
  { & IID_IDirectSoundFXDistortion8, & GUID_DSFX_STANDARD_DISTORTION, sizeof (DSFXDistortion) },
  { & IID_IDirectSoundFXEcho8,       & GUID_DSFX_STANDARD_ECHO,       sizeof (DSFXEcho) },
  { & IID_IDirectSoundFXFlanger8,    & GUID_DSFX_STANDARD_FLANGER,    sizeof (DSFXFlanger) },
  { & IID_IDirectSoundFXGargle8,     & GUID_DSFX_STANDARD_GARGLE,     sizeof (DSFXGargle) },
  { & IID_IDirectSoundFXParamEq8,    & GUID_DSFX_STANDARD_PARAMEQ,    sizeof (DSFXParamEq) },
  { & IID_IDirectSoundFXWavesReverb8, & GUID_DSFX_WAVES_REVERB,       sizeof (DSFXWavesReverb) },
  { & IID_IDirectSoundFXI3DL2Reverb8, & GUID_DSFX_STANDARD_I3DL2REVERB, sizeof (DSFXI3DL2Reverb) }
};

# endif 