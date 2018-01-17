#ifndef __BASE_SOUND_H__
#define __BASE_SOUND_H__


class CCustomSound;

class ISoundManager
{
public:
	virtual int				Init() =0;
	virtual bool			IsValid() =0;
	virtual CCustomSound*	Precache_Sound(const char* pstrSound, bool bLoop = false) =0;
	virtual CCustomSound*	Precache_Stream(const char* pstrMusic, bool bLoop = false) =0;
	virtual bool			Play(CCustomSound* pSoundObject) =0;
	virtual bool			IsPlay(CCustomSound* pSoundObject) =0;
	virtual bool			Stop(CCustomSound* pSoundObject) =0;
	virtual bool			Pause(CCustomSound* pSoundObject) =0;
	virtual bool			Close(CCustomSound** pSoundObject) =0;
	virtual bool			Volume(CCustomSound* pSoundObject, int volume) =0;
};

extern ISoundManager* soundman;

#endif /* ifndef-define include guard */
