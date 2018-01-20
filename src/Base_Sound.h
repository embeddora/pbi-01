#ifndef __BASE_SOUND_H__
#define __BASE_SOUND_H__

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

class CCustomSound;

class ISoundManager
{
public:
	virtual int		Init() =0;

	virtual bool		IsValid() =0;

	virtual CCustomSound*	Precache_Sound(const char* pstrSound, bool bLoop = false) =0;

	virtual CCustomSound*	Precache_Stream(const char* pstrMusic, bool bLoop = false) =0;

	virtual bool		Play(CCustomSound* pSoundObject) =0;

	virtual bool		IsPlay(CCustomSound* pSoundObject) =0;

	virtual bool		Stop(CCustomSound* pSoundObject) =0;

	virtual bool		Pause(CCustomSound* pSoundObject) =0;

	virtual bool		Close(CCustomSound** pSoundObject) =0;

	virtual bool		Volume(CCustomSound* pSoundObject, int volume) =0;
};

extern ISoundManager* soundman;

#endif /* ifndef-define include guard */
