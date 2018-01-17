#ifndef __SOUND_H__
#define __SOUND_H__

/* ISoundManager type */
#include "Base_Sound.h"

class CSoundManager : public ISoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	// init sound device
	int				Init();
	bool			IsValid() { return m_IsInit; };
	// load sound into memory
	CCustomSound*	Precache_Sound(const char* pstrSound, bool bLoop = false);
	// create pointer to stream
	CCustomSound*	Precache_Stream(const char* pstrMusic, bool bLoop = false);
	// play/stop/pause/close both types
	bool			Play(CCustomSound* pSoundObject);
	bool			IsPlay(CCustomSound* pSoundObject);
	bool			Pause(CCustomSound* pSoundObject);
	bool			Stop(CCustomSound* pSoundObject);
	bool			Close(CCustomSound** pSoundObject);
	// set/get volume from 0 to 100
	bool			Volume(CCustomSound* pSoundObject, int volume);

//DATA
	bool		m_IsInit;
};

extern CSoundManager SoundMan;


#endif /* ifndef-define include guard */
