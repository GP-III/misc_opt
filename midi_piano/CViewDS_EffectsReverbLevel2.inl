# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if defined (__INL_PANE_ENABLE)

typedef CWinTraits<WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,WS_EX_STATICEDGE>    CViewDS_EffectsReverbLevel2BaseTraits;

class CViewDS_EffectsReverbLevel2Base:public CWindowImpl<CViewDS_EffectsReverbLevel2Base,CWindow,CViewDS_EffectsReverbLevel2BaseTraits>,
    public CScrollImpl<CViewDS_EffectsReverbLevel2Base>
{
public :
  //  fixed min size
  //  some settings 
  static  const  int width = 392;
  static  const  int height = 460;
  static  const  COLORREF background_color = 0xF0F0F0;

  typedef    CWindowImpl<CViewDS_EffectsReverbLevel2Base,CWindow,CViewDS_EffectsReverbLevel2BaseTraits> baseClass;
  typedef    CScrollImpl<CViewDS_EffectsReverbLevel2Base> baseClass2;
  typedef CViewDS_EffectsReverbLevel2Base  thisClass;
  
  // attach DirectMusic Object.
  CDirectMusic2 *m_DirectMusic;  

  // ctrl .
  CEdit m_GuiET_Room;
  CEdit m_GuiET_RoomHF;
  CEdit m_GuiET_RoomRolloffFactor;
  CEdit m_GuiET_DecayTime;
  CEdit m_GuiET_DecayHFRatio;
  CEdit m_GuiET_Reflections;
  CEdit m_GuiET_ReflectionsDelay;
  CEdit m_GuiET_Reverb;
  CEdit m_GuiET_ReverbDelay;
  CEdit m_GuiET_Diffustion;
  CEdit m_GuiET_Density;
  CEdit m_GuiET_HFReference; 
  
  CTrackBarCtrl m_GuiTK_Room;
  CTrackBarCtrl m_GuiTK_RoomHF;
  CTrackBarCtrl m_GuiTK_RoomRolloffFactor;
  CTrackBarCtrl m_GuiTK_DecayTime;
  CTrackBarCtrl m_GuiTK_DecayHFRatio;
  CTrackBarCtrl m_GuiTK_Reflections;
  CTrackBarCtrl m_GuiTK_ReflectionsDelay;
  CTrackBarCtrl m_GuiTK_Reverb;
  CTrackBarCtrl m_GuiTK_ReverbDelay;
  CTrackBarCtrl m_GuiTK_Diffustion;
  CTrackBarCtrl m_GuiTK_Density;
  CTrackBarCtrl m_GuiTK_HFReference;
  
  CComboBox m_GuiCB_Quality;
  CComboBox m_GuiCB_ReverbType;
  
  CButton m_GuiCK_Enable;

  DSFXI3DL2Reverb m_ReverbLevel2Param;

  CViewDS_EffectsReverbLevel2Base (void)  {
    m_ReverbLevel2Param.lRoom = DSFX_I3DL2REVERB_ROOM_DEFAULT;
    m_ReverbLevel2Param.lRoomHF = DSFX_I3DL2REVERB_ROOMHF_DEFAULT;
    m_ReverbLevel2Param.flRoomRolloffFactor = DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_DEFAULT;
    m_ReverbLevel2Param.flDecayTime = DSFX_I3DL2REVERB_DECAYTIME_DEFAULT;
    m_ReverbLevel2Param.flDecayHFRatio = DSFX_I3DL2REVERB_DECAYHFRATIO_DEFAULT;
    m_ReverbLevel2Param.lReflections = DSFX_I3DL2REVERB_REFLECTIONS_DEFAULT;
    m_ReverbLevel2Param.flReflectionsDelay = DSFX_I3DL2REVERB_REFLECTIONSDELAY_DEFAULT;
    m_ReverbLevel2Param.lReverb = DSFX_I3DL2REVERB_REVERB_DEFAULT;
    m_ReverbLevel2Param.flReverbDelay = DSFX_I3DL2REVERB_REVERBDELAY_DEFAULT;
    m_ReverbLevel2Param.flDiffusion = DSFX_I3DL2REVERB_DIFFUSION_DEFAULT;
    m_ReverbLevel2Param.flDensity = DSFX_I3DL2REVERB_DENSITY_DEFAULT;
    m_ReverbLevel2Param.flHFReference = DSFX_I3DL2REVERB_HFREFERENCE_DEFAULT;

    m_DirectMusic = NULL;
  }

  DECLARE_WND_CLASS(_T ("CViewDS_EffectsReverbLevel2Base"))

  BEGIN_MSG_MAP(thisClass)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_HSCROLL, OnSlider)
    COMMAND_HANDLER2 (m_GuiCK_Enable, BN_CLICKED, OnSetEnable)
    COMMAND_HANDLER2 (m_GuiCB_Quality, CBN_SELCHANGE, OnSelCOmboItem)
    COMMAND_HANDLER2 (m_GuiCB_ReverbType, CBN_SELCHANGE, OnSelCOmboItem)
    CHAIN_MSG_MAP(baseClass2)
  END_MSG_MAP()

  LRESULT OnSetEnable (WORD code, WORD /*wID*/, HWND hWindow, BOOL& /*bHandled*/) {
    if (:: SendMessage (hWindow, BM_GETCHECK, 0, 0) == BST_UNCHECKED)  
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB_LEVEL2, FALSE);
    else  {
      m_DirectMusic->SetEffectEnable (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB_LEVEL2, TRUE);
      m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB_LEVEL2, (CDirectMusic2::DSFX_desc *)& m_ReverbLevel2Param);
    }
    return 0;
  }

  void SetAttach (CDirectMusic2 *cmusic) {
    m_DirectMusic = cmusic;
  }
  void DoPaint (CDCHandle dc) {
    CRect t;
    GetClientRect (t);
    t.OffsetRect ( m_ptOffset);
    dc.FillSolidRect (& t, background_color);
  }

  void UpdateParamText (void) {
    CString str[12];
    str[0].Format ( _T ("%d"), m_ReverbLevel2Param.lRoom);
    str[1].Format ( _T ("%d"), m_ReverbLevel2Param.lRoomHF);
    str[2].Format ( _T ("%f"), m_ReverbLevel2Param.flRoomRolloffFactor);
    str[3].Format ( _T ("%f"), m_ReverbLevel2Param.flDecayTime);
    str[4].Format ( _T ("%f"), m_ReverbLevel2Param.flDecayHFRatio);
    str[5].Format ( _T ("%d"), m_ReverbLevel2Param.lReflections);
    str[6].Format ( _T ("%f"), m_ReverbLevel2Param.flReflectionsDelay);
    str[7].Format ( _T ("%d"), m_ReverbLevel2Param.lReverb);
    str[8].Format ( _T ("%f"), m_ReverbLevel2Param.flReverbDelay);
    str[9].Format ( _T ("%f"), m_ReverbLevel2Param.flDiffusion);
    str[10].Format ( _T ("%f"), m_ReverbLevel2Param.flDensity);
    str[11].Format ( _T ("%f"), m_ReverbLevel2Param.flHFReference);

    m_GuiET_Room.SetWindowText (str[0]);
    m_GuiET_RoomHF.SetWindowText (str[1]);
    m_GuiET_RoomRolloffFactor.SetWindowText (str[2]);
    m_GuiET_DecayTime.SetWindowText (str[3]);
    m_GuiET_DecayHFRatio.SetWindowText (str[4]);
    m_GuiET_Reflections.SetWindowText (str[5]);
    m_GuiET_ReflectionsDelay.SetWindowText (str[6]);
    m_GuiET_Reverb.SetWindowText (str[7]);
    m_GuiET_ReverbDelay.SetWindowText (str[8]);
    m_GuiET_Diffustion.SetWindowText (str[9]);
    m_GuiET_Density.SetWindowText (str[10]);
    m_GuiET_HFReference.SetWindowText (str[11]);
  }

  void UpdateParamSlider (void) {
    m_GuiTK_Room.SetPos ( (int)( ((float)( m_ReverbLevel2Param.lRoom - DSFX_I3DL2REVERB_ROOM_MIN)) / (float)(DSFX_I3DL2REVERB_ROOM_MAX - DSFX_I3DL2REVERB_ROOM_MIN) * 10000.0 ));
    m_GuiTK_RoomHF.SetPos ( (int)( ((float)( m_ReverbLevel2Param.lRoomHF - DSFX_I3DL2REVERB_ROOMHF_MIN)) / (float)(DSFX_I3DL2REVERB_ROOMHF_MAX - DSFX_I3DL2REVERB_ROOMHF_MIN) * 10000.0 ));
    m_GuiTK_RoomRolloffFactor.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flRoomRolloffFactor - DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN)) / (float)(DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX - DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN) * 100.0 ));
    m_GuiTK_DecayTime.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flDecayTime - DSFX_I3DL2REVERB_DECAYTIME_MIN)) / (float)(DSFX_I3DL2REVERB_DECAYTIME_MAX - DSFX_I3DL2REVERB_DECAYTIME_MIN) * 199.0 ));
    m_GuiTK_DecayHFRatio.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flDecayHFRatio - DSFX_I3DL2REVERB_DECAYHFRATIO_MIN)) / (float)(DSFX_I3DL2REVERB_DECAYHFRATIO_MAX - DSFX_I3DL2REVERB_DECAYHFRATIO_MIN) * 19.0 ));
    m_GuiTK_Reflections.SetPos ( (int)( ((float)( m_ReverbLevel2Param.lReflections - DSFX_I3DL2REVERB_REFLECTIONS_MIN)) / (float)(DSFX_I3DL2REVERB_REFLECTIONS_MAX - DSFX_I3DL2REVERB_REFLECTIONS_MIN) * 11000.0 ));
    m_GuiTK_ReflectionsDelay.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flReflectionsDelay - DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN)) / (float)(DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX - DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN) * 30.0 ));
    m_GuiTK_Reverb.SetPos ( (int)( ((float)( m_ReverbLevel2Param.lReverb - DSFX_I3DL2REVERB_REVERB_MIN)) / (float)(DSFX_I3DL2REVERB_REVERB_MAX - DSFX_I3DL2REVERB_REVERB_MIN) * 12000.0 ));
    m_GuiTK_ReverbDelay.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flReverbDelay - DSFX_I3DL2REVERB_REVERBDELAY_MIN)) / (float)(DSFX_I3DL2REVERB_REVERBDELAY_MAX - DSFX_I3DL2REVERB_REVERBDELAY_MIN) * 1000.0 ));
    m_GuiTK_Diffustion.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flDiffusion - DSFX_I3DL2REVERB_DIFFUSION_MIN)) / (float)(DSFX_I3DL2REVERB_DIFFUSION_MAX - DSFX_I3DL2REVERB_DIFFUSION_MIN) * 1000.0 ));  
    m_GuiTK_Density.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flDensity - DSFX_I3DL2REVERB_DENSITY_MIN)) / (float)(DSFX_I3DL2REVERB_DENSITY_MAX - DSFX_I3DL2REVERB_DENSITY_MIN) * 1000.0 ));
    m_GuiTK_HFReference.SetPos ( (int)( ((float)( m_ReverbLevel2Param.flHFReference - DSFX_I3DL2REVERB_HFREFERENCE_MIN)) / (float)(DSFX_I3DL2REVERB_HFREFERENCE_MAX - DSFX_I3DL2REVERB_HFREFERENCE_MIN) * 19980.0 ));
  }

  LRESULT OnSelCOmboItem (WORD code, WORD /*wID*/, HWND hWindow, BOOL& bHandled) {
    if (hWindow == m_GuiCB_Quality)  {
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetReverb_L2Quality ( m_GuiCB_Quality.GetCurSel ());
      else return 0;
    } else if (hWindow == m_GuiCB_ReverbType)  {
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED)
        m_DirectMusic->SetReverb_L2Type (  m_GuiCB_ReverbType.GetCurSel ());
      else return 0;
    } else  {
      bHandled = FALSE;
      return  0;
    }
    // reset show.
    CDirectMusic2 :: DSFX_Reverb2 eb2;
    if ( m_DirectMusic->Get_Reverb_L2 (& eb2) == 0) {
      memcpy (& m_ReverbLevel2Param, & eb2.desc, sizeof (m_ReverbLevel2Param));

      // m_GuiCB_ReverbType.SetCurSel (eb2.Type - DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT);
      m_GuiCB_Quality.SetCurSel (eb2.Quality);

      // update text .
      UpdateParamText ();
      UpdateParamSlider ();
    }
    return 0;
  }
  LRESULT OnSlider(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    int code = LOWORD (wParam);
    if (code == TB_ENDTRACK || code == TB_THUMBTRACK)  {
      CString s;
      BOOL bSet = TRUE;
      HWND hWindow = (HWND) lParam;

      if  ( hWindow == m_GuiTK_Room)  {
        int pos = m_GuiTK_Room.GetPos ();
        float c = ((float)pos) / 10000.0f;
        int b = (int) (c * (DSFX_I3DL2REVERB_ROOM_MAX - DSFX_I3DL2REVERB_ROOM_MIN));
        m_ReverbLevel2Param.lRoom =b+ DSFX_I3DL2REVERB_ROOM_MIN;
        if  ( m_ReverbLevel2Param.lRoom > DSFX_I3DL2REVERB_ROOM_MAX)
          m_ReverbLevel2Param.lRoom = DSFX_I3DL2REVERB_ROOM_MAX;
        if  ( m_ReverbLevel2Param.lRoom < DSFX_I3DL2REVERB_ROOM_MIN)
          m_ReverbLevel2Param.lRoom = DSFX_I3DL2REVERB_ROOM_MIN;
        s.Format (_T ("%d"), m_ReverbLevel2Param.lRoom);
        m_GuiET_Room.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_RoomHF)  {
        int pos = m_GuiTK_RoomHF.GetPos ();
        float c = ((float)pos) / 10000.0f;
        int b = (int) (c * (DSFX_I3DL2REVERB_ROOMHF_MAX - DSFX_I3DL2REVERB_ROOMHF_MIN));
        m_ReverbLevel2Param.lRoomHF =b+ DSFX_I3DL2REVERB_ROOMHF_MIN;
        if  ( m_ReverbLevel2Param.lRoomHF > DSFX_I3DL2REVERB_ROOMHF_MAX)
          m_ReverbLevel2Param.lRoomHF = DSFX_I3DL2REVERB_ROOMHF_MAX;
        if  ( m_ReverbLevel2Param.lRoomHF < DSFX_I3DL2REVERB_ROOMHF_MIN)
          m_ReverbLevel2Param.lRoomHF = DSFX_I3DL2REVERB_ROOMHF_MIN;
        s.Format (_T ("%d"), m_ReverbLevel2Param.lRoomHF);
        m_GuiET_RoomHF.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_RoomRolloffFactor)  {
        int pos = m_GuiTK_RoomRolloffFactor.GetPos ();
        float c = ((float)pos) / 100.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX - DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN));
        m_ReverbLevel2Param.flRoomRolloffFactor =b+ DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN;
        if  ( m_ReverbLevel2Param.flRoomRolloffFactor > DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX)
          m_ReverbLevel2Param.flRoomRolloffFactor = DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MAX;
        if  ( m_ReverbLevel2Param.flRoomRolloffFactor < DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN)
          m_ReverbLevel2Param.flRoomRolloffFactor = DSFX_I3DL2REVERB_ROOMROLLOFFFACTOR_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flRoomRolloffFactor);
        m_GuiET_RoomRolloffFactor.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_DecayTime)  {
        int pos = m_GuiTK_DecayTime.GetPos ();
        float c = ((float)(pos - 1)) / 199.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_DECAYTIME_MAX - DSFX_I3DL2REVERB_DECAYTIME_MIN));
        m_ReverbLevel2Param.flDecayTime =b+ DSFX_I3DL2REVERB_DECAYTIME_MIN;
        if  ( m_ReverbLevel2Param.flDecayTime > DSFX_I3DL2REVERB_DECAYTIME_MAX)
          m_ReverbLevel2Param.flDecayTime = DSFX_I3DL2REVERB_DECAYTIME_MAX;
        if  ( m_ReverbLevel2Param.flDecayTime < DSFX_I3DL2REVERB_DECAYTIME_MIN)
          m_ReverbLevel2Param.flDecayTime = DSFX_I3DL2REVERB_DECAYTIME_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flDecayTime);
        m_GuiET_DecayTime.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_DecayHFRatio)  {
        int pos = m_GuiTK_DecayHFRatio.GetPos ();
        float c = ((float)(pos - 1)) / 19.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_DECAYHFRATIO_MAX - DSFX_I3DL2REVERB_DECAYHFRATIO_MIN));
        m_ReverbLevel2Param.flDecayHFRatio =b+ DSFX_I3DL2REVERB_DECAYHFRATIO_MIN;
        if  ( m_ReverbLevel2Param.flDecayHFRatio > DSFX_I3DL2REVERB_DECAYHFRATIO_MAX)
          m_ReverbLevel2Param.flDecayHFRatio = DSFX_I3DL2REVERB_DECAYHFRATIO_MAX;
        if  ( m_ReverbLevel2Param.flDecayHFRatio < DSFX_I3DL2REVERB_DECAYHFRATIO_MIN)
          m_ReverbLevel2Param.flDecayHFRatio = DSFX_I3DL2REVERB_DECAYHFRATIO_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flDecayHFRatio);
        m_GuiET_DecayHFRatio.SetWindowText (s);
      }  else if  ( hWindow == m_GuiTK_Reflections)  {
        int pos = m_GuiTK_Reflections.GetPos ();
        float c = ((float)(pos)) / 11000.0f;
        int b = (int) (c * (DSFX_I3DL2REVERB_REFLECTIONS_MAX - DSFX_I3DL2REVERB_REFLECTIONS_MIN));
        m_ReverbLevel2Param.lReflections =b+ DSFX_I3DL2REVERB_REFLECTIONS_MIN;
        if  ( m_ReverbLevel2Param.lReflections > DSFX_I3DL2REVERB_REFLECTIONS_MAX)
          m_ReverbLevel2Param.lReflections = DSFX_I3DL2REVERB_REFLECTIONS_MAX;
        if  ( m_ReverbLevel2Param.lReflections < DSFX_I3DL2REVERB_REFLECTIONS_MIN)
          m_ReverbLevel2Param.lReflections = DSFX_I3DL2REVERB_REFLECTIONS_MIN;
        s.Format (_T ("%d"), m_ReverbLevel2Param.lReflections);
        m_GuiET_Reflections.SetWindowText (s);
      }  else if  ( hWindow == m_GuiTK_ReflectionsDelay)  {
        int pos = m_GuiTK_ReflectionsDelay.GetPos ();
        float c = ((float)(pos)) / 30.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX - DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN));
        m_ReverbLevel2Param.flReflectionsDelay =b+ DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN;
        if  ( m_ReverbLevel2Param.flReflectionsDelay > DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX)
          m_ReverbLevel2Param.flReflectionsDelay = DSFX_I3DL2REVERB_REFLECTIONSDELAY_MAX;
        if  ( m_ReverbLevel2Param.flReflectionsDelay < DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN)
          m_ReverbLevel2Param.flReflectionsDelay = DSFX_I3DL2REVERB_REFLECTIONSDELAY_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flReflectionsDelay);
        m_GuiET_ReflectionsDelay.SetWindowText (s);
      }  else if  ( hWindow == m_GuiTK_Reverb)  {
        int pos = m_GuiTK_Reverb.GetPos ();
        float c = ((float)(pos)) / 12000.0f;
        int b = (int) (c * (DSFX_I3DL2REVERB_REVERB_MAX - DSFX_I3DL2REVERB_REVERB_MIN));
        m_ReverbLevel2Param.lReverb =b+ DSFX_I3DL2REVERB_REVERB_MIN;
        if  ( m_ReverbLevel2Param.lReverb > DSFX_I3DL2REVERB_REVERB_MAX)
          m_ReverbLevel2Param.lReverb = DSFX_I3DL2REVERB_REVERB_MAX;
        if  ( m_ReverbLevel2Param.lReverb < DSFX_I3DL2REVERB_REVERB_MIN)
          m_ReverbLevel2Param.lReverb = DSFX_I3DL2REVERB_REVERB_MIN;
        s.Format (_T ("%d"), m_ReverbLevel2Param.lReverb);
        m_GuiET_Reverb.SetWindowText (s);
      }  else if  ( hWindow == m_GuiTK_ReverbDelay)  {
        int pos = m_GuiTK_ReverbDelay.GetPos ();
        float c = ((float)(pos)) / 1000.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_REVERBDELAY_MAX - DSFX_I3DL2REVERB_REVERBDELAY_MIN));
        m_ReverbLevel2Param.flReverbDelay =b+ DSFX_I3DL2REVERB_REVERBDELAY_MIN;
        if  ( m_ReverbLevel2Param.flReverbDelay > DSFX_I3DL2REVERB_REVERBDELAY_MAX)
          m_ReverbLevel2Param.flReverbDelay = DSFX_I3DL2REVERB_REVERBDELAY_MAX;
        if  ( m_ReverbLevel2Param.flReverbDelay < DSFX_I3DL2REVERB_REVERBDELAY_MIN)
          m_ReverbLevel2Param.flReverbDelay = DSFX_I3DL2REVERB_REVERBDELAY_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flReverbDelay);
        m_GuiET_ReverbDelay.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Diffustion)  {
        int pos = m_GuiTK_Diffustion.GetPos ();
        float c = ((float)(pos)) / 1000.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_DIFFUSION_MAX - DSFX_I3DL2REVERB_DIFFUSION_MIN));
        m_ReverbLevel2Param.flDiffusion =b+ DSFX_I3DL2REVERB_DIFFUSION_MIN;
        if  ( m_ReverbLevel2Param.flDiffusion > DSFX_I3DL2REVERB_DIFFUSION_MAX)
          m_ReverbLevel2Param.flDiffusion = DSFX_I3DL2REVERB_DIFFUSION_MAX;
        if  ( m_ReverbLevel2Param.flDiffusion < DSFX_I3DL2REVERB_DIFFUSION_MIN)
          m_ReverbLevel2Param.flDiffusion = DSFX_I3DL2REVERB_DIFFUSION_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flDiffusion);
        m_GuiET_Diffustion.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_Density)  {
        int pos = m_GuiTK_Density.GetPos ();
        float c = ((float)(pos)) / 1000.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_DENSITY_MAX - DSFX_I3DL2REVERB_DENSITY_MIN));
        m_ReverbLevel2Param.flDensity =b+ DSFX_I3DL2REVERB_DENSITY_MIN;
        if  ( m_ReverbLevel2Param.flDensity > DSFX_I3DL2REVERB_DENSITY_MAX)
          m_ReverbLevel2Param.flDensity = DSFX_I3DL2REVERB_DENSITY_MAX;
        if  ( m_ReverbLevel2Param.flDensity < DSFX_I3DL2REVERB_DENSITY_MIN)
          m_ReverbLevel2Param.flDensity = DSFX_I3DL2REVERB_DENSITY_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flDensity);
        m_GuiET_Density.SetWindowText (s);
      } else if  ( hWindow == m_GuiTK_HFReference)  {
        int pos = m_GuiTK_HFReference.GetPos ();
        float c = ((float)(pos - 20)) / 19980.0f;
        float b = (float) (c * (DSFX_I3DL2REVERB_HFREFERENCE_MAX - DSFX_I3DL2REVERB_HFREFERENCE_MIN));
        m_ReverbLevel2Param.flHFReference =b+ DSFX_I3DL2REVERB_HFREFERENCE_MIN;
        if  ( m_ReverbLevel2Param.flHFReference > DSFX_I3DL2REVERB_HFREFERENCE_MAX)
          m_ReverbLevel2Param.flHFReference = DSFX_I3DL2REVERB_HFREFERENCE_MAX;
        if  ( m_ReverbLevel2Param.flHFReference < DSFX_I3DL2REVERB_HFREFERENCE_MIN)
          m_ReverbLevel2Param.flHFReference = DSFX_I3DL2REVERB_HFREFERENCE_MIN;
        s.Format (_T ("%f"), m_ReverbLevel2Param.flHFReference);
        m_GuiET_HFReference.SetWindowText (s);
      } else  {
        bHandled = FALSE;
        return 0;
      }   
      if  ( m_GuiCK_Enable.GetCheck () == BST_CHECKED) 
        m_DirectMusic->SetEffectDesc (CDirectMusic2 ::IDIRECTEFFECTS_POS_REVERB_LEVEL2, (CDirectMusic2::DSFX_desc *)& m_ReverbLevel2Param);
      return 0;
    }
    bHandled = FALSE;
    return 0;
  }

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
    
    SetClassLongPtr (m_hWnd, GCLP_HBRBACKGROUND, (LONG_PTR) GetStockObject (NULL_BRUSH)) ;
    HINSTANCE hInst = _Module.GetModuleInstance ();
    HWND hWindow = m_hWnd;

    HFONT hFont = CreateFont(-11, 0, 0, 0, 400, FALSE, FALSE, FALSE, 1, 400, 0, 0, 0, _T ("MS Shell Dlg"));
    
    // static  Group.
    HWND T = CreateWindowEx (0, WC_STATIC, _T (""), WS_VISIBLE | WS_CHILD | SS_ETCHEDHORZ, 17, 375, 357, 2, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Room(mB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 47, 51, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (WS_EX_TRANSPARENT, WC_BUTTON, _T ("IDirectSound8 Effects - Reverb-Level2 Settings"), WS_VISIBLE | WS_CHILD | 0x00000007, 8, 5, 377, 447, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Quality"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 388, 33, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("RoomHF(mB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 73, 65, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("RoomRolloffFactor(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 99, 104, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("DecayTime(s)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 125, 66, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("DecayHFRatio"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 156, 72, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("ReverbType"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 416, 60, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Reverb(mB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 239, 57, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Reflections(mB)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 182, 75, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("ReverbDelay(s)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 270, 75, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("ReflectionsDelay(s)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 208, 93, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("HFReference(hz)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 353, 83, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Density(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 322, 51, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);
    T = CreateWindowEx (0, WC_STATIC, _T ("Diffusion(%)"), WS_VISIBLE | WS_CHILD | WS_GROUP | SS_LEFT, 18, 296, 57, 15, hWindow, (HMENU)-1, hInst, 0);
    SendMessage (T, WM_SETFONT, (WPARAM)hFont, FALSE);

    // Edit Group. 
    m_GuiET_Room = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 46, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Room.SetFont (hFont, FALSE);
    m_GuiET_RoomHF = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 72, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_RoomHF.SetFont (hFont, FALSE);
    m_GuiET_RoomRolloffFactor = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 134, 99, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_RoomRolloffFactor.SetFont (hFont, FALSE);
    m_GuiET_DecayTime = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 104, 124, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_DecayTime.SetFont (hFont, FALSE);
    m_GuiET_DecayHFRatio = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 117, 154, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_DecayHFRatio.SetFont (hFont, FALSE);
    m_GuiET_Reflections = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 114, 182, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Reflections.SetFont (hFont, FALSE);
    m_GuiET_ReflectionsDelay = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 131, 206, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_ReflectionsDelay.SetFont (hFont, FALSE);
    m_GuiET_Reverb = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 114, 237, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Reverb.SetFont (hFont, FALSE);
    m_GuiET_ReverbDelay = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 116, 268, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_ReverbDelay.SetFont (hFont, FALSE);
    m_GuiET_Diffustion = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 114, 296, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Diffustion.SetFont (hFont, FALSE);
    m_GuiET_Density = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 129, 320, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_Density.SetFont (hFont, FALSE);
    m_GuiET_HFReference = CreateWindowEx (0, WC_EDIT, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY, 113, 351, 65, 20, hWindow, (HMENU)-1, hInst, 0);
    m_GuiET_HFReference.SetFont (hFont, FALSE);

    // CSlider Group.
    m_GuiTK_Room = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 42, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Room.SetFont (hFont, FALSE);
    m_GuiTK_RoomHF = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 68, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_RoomHF.SetFont (hFont, FALSE);
    m_GuiTK_RoomRolloffFactor = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 210, 96, 164, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_RoomRolloffFactor.SetFont (hFont, FALSE);
    m_GuiTK_DecayTime = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 174, 122, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_DecayTime.SetFont (hFont, FALSE);
    m_GuiTK_DecayHFRatio = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 188, 151, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_DecayHFRatio.SetFont (hFont, FALSE);
    m_GuiTK_Reflections = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 185, 179, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Reflections.SetFont (hFont, FALSE);
    m_GuiTK_ReflectionsDelay = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 201, 205, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_ReflectionsDelay.SetFont (hFont, FALSE);
    m_GuiTK_Reverb = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 185, 234, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Reverb.SetFont (hFont, FALSE);
    m_GuiTK_ReverbDelay = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 186, 265, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_ReverbDelay.SetFont (hFont, FALSE);
    m_GuiTK_Diffustion = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 185, 293, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Diffustion.SetFont (hFont, FALSE);
    m_GuiTK_Density = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 200, 319, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_Density.SetFont (hFont, FALSE);
    m_GuiTK_HFReference = CreateWindowEx (0, TRACKBAR_CLASS, _T (""), WS_VISIBLE | WS_CHILD | WS_TABSTOP | TBS_BOTH | TBS_NOTICKS, 183, 348, 180, 26, hWindow, (HMENU)-1, hInst, 0);
    m_GuiTK_HFReference.SetFont (hFont, FALSE);

    // Check 
    m_GuiCK_Enable = CreateWindowEx (0, WC_BUTTON, _T ("Enable Reverb-Level2 Effects"), WS_VISIBLE | WS_CHILD | WS_TABSTOP | 0x00000003, 18, 24, 167, 13, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCK_Enable.SetFont (hFont, FALSE);

    // Combo   
    m_GuiCB_Quality = CreateWindowEx (0, WC_COMBOBOX, _T (""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 69, 385, 243, 21, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCB_Quality.SetFont (hFont, FALSE);
    m_GuiCB_ReverbType = CreateWindowEx (0, WC_COMBOBOX, _T (""), WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS, 87, 413, 243, 21, hWindow, (HMENU)-1, hInst, 0);
    m_GuiCB_ReverbType.SetFont (hFont, FALSE);

    m_GuiTK_Room.SetRange (0, 10000);
    m_GuiTK_RoomHF.SetRange (0, 10000);
    m_GuiTK_RoomRolloffFactor.SetRange (0, 100);
    m_GuiTK_DecayTime.SetRange (1, 200);
    m_GuiTK_DecayHFRatio.SetRange (1, 20);
    m_GuiTK_Reflections.SetRange (0, 11000);
    m_GuiTK_ReflectionsDelay.SetRange (0, 30);
    m_GuiTK_Reverb.SetRange (0, 12000);
    m_GuiTK_ReverbDelay.SetRange (0, 1000);
    m_GuiTK_Diffustion.SetRange (0, 1000);
    m_GuiTK_Density.SetRange (0, 1000);
    m_GuiTK_HFReference.SetRange (20, 20000);
   
    // Combo 
    m_GuiCB_Quality.Clear ();
    m_GuiCB_Quality.InsertString (-1, _T ("0"));
    m_GuiCB_Quality.InsertString (-1, _T ("1"));
    m_GuiCB_Quality.InsertString (-1, _T ("2"));
    m_GuiCB_Quality.InsertString (-1, _T ("3"));
    m_GuiCB_Quality.SetCurSel (DSFX_I3DL2REVERB_QUALITY_DEFAULT);

    m_GuiCB_ReverbType.Clear ();
    m_GuiCB_ReverbType.InsertString (-1, _T ("Default"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Generic"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Paddedcell"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Room"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("BathRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("LivingRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("StoneRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Auditorium"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Concerthall"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Cave"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Arena"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Hanger"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Carpetedhallway"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Hallway"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("StoneCorridor"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Alley"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Forest"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("City"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Mountains"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Quarry"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Plain"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Parkinglot"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Sewerpipe"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Underwater"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("SmallRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("MediumRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("LargeRoom"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Mediumhall"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Largehall"));
    m_GuiCB_ReverbType.InsertString (-1, _T ("Plate"));
    m_GuiCB_ReverbType.SetCurSel (DSFX_I3DL2_ENVIRONMENT_PRESET_DEFAULT);

    UpdateParamSlider ();
    UpdateParamText ();
    return 0;
  }
};

# endif // !defined(_CVIEWDS_EFFECTSREVERBLEVEL2_INCLUED_HEADER_CXX_