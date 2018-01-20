#ifndef __SOUND_H__
#define __SOUND_H__

/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract:  
 */

/* ISoundManager type */
#include "Base_Sound.h"

class CSoundManager : public ISoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	/* Init sound device */
	int			Init();

	bool			IsValid() { return m_IsInit; };

	/* Load sound into memory */
	CCustomSound*	Precache_Sound(const char* pstrSound, bool bLoop = false);

	/* Create pointer to stream */
	CCustomSound*	Precache_Stream(const char* pstrMusic, bool bLoop = false);

	// play/stop/pause/close both types
	bool			Play(CCustomSound* pSoundObject);

	bool			IsPlay(CCustomSound* pSoundObject);

	bool			Pause(CCustomSound* pSoundObject);

	bool			Stop(CCustomSound* pSoundObject);

	bool			Close(CCustomSound** pSoundObject);

	/* set/get volume from 0 to 100 */
	bool			Volume(CCustomSound* pSoundObject, int volume);

//DATA
	bool		m_IsInit;
};

extern CSoundManager SoundMan;


#endif /* ifndef-define include guard */
