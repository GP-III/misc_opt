# if _MSC_VER >= 1000
#   pragma once
# endif // _MSC_VER >= 1000

# if !defined (_CFILE_EPS_INCLUED_HEADER_CXX_)
#  define _CFILE_EPS_INCLUED_HEADER_CXX_

# include "stdwtl.h"

class CFile_eps {
public :
  static const int eps_default_chunk_size = 80000;
  static const int eps_realloc_size = 4000;

  struct eps_header {
    char magic_id[4]; // 'E' 'P' 'S' '^Z' [0x1A]
    DWORD total_ms;  
    DWORD chunk_nums; 
  };
  struct eps_chunk {
    BYTE sig;              //  MSB: 1?down:up other:NOTE 
    BYTE vel;              //   vel 
    WORD rTimeToThis;      //  rel time to this 
  };

  int m_cap_size;
  int m_cur_ptr;

  DWORD m_totaltime;
  DWORD m_timeAgo;
  DWORD m_readmax;

  eps_chunk *m_peps_chunk;

  CFile_eps (void) {
    m_timeAgo = 0;
    m_peps_chunk = (eps_chunk *) malloc (sizeof (eps_chunk) * eps_default_chunk_size);
    m_cap_size = eps_default_chunk_size;
    m_cur_ptr = 0;
  }

  void AppendChunk (BOOL press, BYTE note, BYTE vel, DWORD timeCur) {
    ++ m_cur_ptr;
    if (m_cur_ptr > m_cap_size)  {
      //resize size. 
      m_cap_size += eps_realloc_size;
      void * t = realloc (m_peps_chunk, m_cap_size);
      ATLASSERT (t != NULL);
      m_peps_chunk =(eps_chunk *)t ;
    }
    int id = m_cur_ptr-1;
    m_peps_chunk[id].sig = (!! press) ? 0x80 : 0x00;
    m_peps_chunk[id].sig|= (note & 0x7F);
    m_peps_chunk[id].rTimeToThis = timeCur - m_timeAgo;
    m_peps_chunk[id].vel = vel;
    m_timeAgo = timeCur;
  }
  void AppendChunk (eps_chunk &c/* noset rTimeToThis*/ ,  DWORD timeCur) {
    ++ m_cur_ptr;
    if (m_cur_ptr > m_cap_size)  {
      //resize size. 
      m_cap_size += eps_realloc_size;
      void * t = realloc (m_peps_chunk, m_cap_size);
      ATLASSERT (t != NULL);
      m_peps_chunk =(eps_chunk *)t ;
    }
    int id = m_cur_ptr-1;
    m_peps_chunk[id].sig = c.sig;
    m_peps_chunk[id].rTimeToThis = timeCur - m_timeAgo;
    m_peps_chunk[id].vel = c.vel;
    m_timeAgo = timeCur;
  }

  int ReadChunk (BOOL &press, BYTE &note, BYTE &vel, DWORD &timeCur) {
    ++ m_cur_ptr;
    if (m_cur_ptr > m_readmax)  {
      if  (m_cur_ptr == (m_readmax + 1))
        return -1;
      //resize size.
      m_cur_ptr++;
      return  -2;
    }
    int id = m_cur_ptr-1;
    press =  !! (m_peps_chunk[id].sig & 0x80);
    note = m_peps_chunk[id].sig & 0x7F;
    timeCur = m_peps_chunk[id].rTimeToThis;
    vel  = m_peps_chunk[id].vel;
    return  0;
  }

  DWORD calc_total (void) {
    DWORD time = 0;
    for (int id= 0; id != m_cur_ptr; id++) 
      time += m_peps_chunk[id].rTimeToThis;
    return time;
  }

  void WriteEPS (TCHAR *filename)  {
    FILE *fd = _tfopen (filename, _T ("wb"));
    ATLASSERT (fd != NULL);
    // Write file header 
    eps_header hd;
    hd.chunk_nums = m_cur_ptr;
    hd.magic_id[0] = 'E';
    hd.magic_id[1] = 'P';
    hd.magic_id[2] = 'S';
    hd.magic_id[3] = 0x1A;
    hd.total_ms = calc_total ();

    fwrite (& hd, sizeof (hd), 1, fd);

    // write chunk.
    for (int id =0; id != hd.chunk_nums; id++)  
      fwrite (& m_peps_chunk[id], sizeof (sizeof (eps_chunk)), 1, fd);
    fclose (fd);
  }

  int ReadEPS (TCHAR *filename)  {
    __try {
      FILE *fd = _tfopen (filename, _T ("rb"));
      if (fd == NULL)
        goto __cleanup;

      // read header
      eps_header hd;
      fread (& hd, sizeof (hd), 1, fd);

      if  (! (hd.magic_id[0] == 'E'
        && hd.magic_id[1] == 'P'
        && hd.magic_id[2] == 'S'
        && hd.magic_id[3] == 0x1A))
      {
        AtlMessageBox (NULL, _T ("invalid EPS file"), _T ("ReadEPS error"), MB_ICONERROR);
        goto __cleanup;
      }

      // check size .
      if ( hd.chunk_nums > m_cap_size)  { 
        m_cap_size = hd.chunk_nums;
        void * t = realloc (m_peps_chunk, m_cap_size);
        ATLASSERT (t != NULL);
        m_peps_chunk =(eps_chunk *)t ;
      }

      m_readmax = hd.chunk_nums;
      m_totaltime = hd.total_ms;

      fread (m_peps_chunk, sizeof (eps_chunk) * hd.chunk_nums, 1, fd);
      fclose (fd);
      return 0;
__cleanup:
      if (fd != NULL)
        fclose (fd);
      return -1;
    } __except ( EXCEPTION_EXECUTE_HANDLER)  {
      AtlMessageBox (NULL, _T ("ReadEpsFile Failed"), _T ("SEH-chain except"), MB_ICONERROR);
      return -1;
    }
  }
  void ResetRecord (DWORD time) {
    m_timeAgo = time;
    m_cur_ptr = 0;
  }
  void ResetPlay (DWORD time) {
    m_cur_ptr = 0;
  }
  void SetPos (DWORD ms) {
    DWORD time = 0;
    for (int id= 0; id != m_readmax; id++) {
      time += m_peps_chunk[id].rTimeToThis;
      if (time >= ms) {
        m_cur_ptr = id;
        return  ;
      }
    }
    ATLASSERT (FALSE); 
  }
};

# endif // !defined(_CFILE_EPS_INCLUED_HEADER_CXX_)