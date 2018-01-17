/* perofrmance warning about casting/typeforcing */
#pragma warning( disable : 4800 ) 

#include "Base_Sound.h"

#include "File.h"

//TIP: "Base_Sound.h" included in "Sound.h"
#include "Sound.h"

#include "Types.h" 

#include "Fmod/fmod.h"

#include "Fmod/fmod_errors.h"

#pragma comment( lib, "src/Fmod/fmodvc.lib" )

/* for std::set */
#include "../stl/stl_set.h"

class CCustomSound
{
private:
	newString	m_strName;
	int*	m_pSound;
	int		m_iChannel;
	bool	m_IsStream;
	int		m_iVolume;

public:
	CCustomSound() : m_pSound(NULL), m_IsStream(false), m_iVolume(255) {};

	~CCustomSound()
	{
		Stop();

		if(m_IsStream)

			FSOUND_Stream_Close((FSOUND_STREAM*)m_pSound);

		else

			FSOUND_Sample_Free((FSOUND_SAMPLE*)m_pSound);

		m_iChannel = 0;
	};

	/* Sound */
	int* Sound(int* pSnd =0)
	{
		if(!pSnd)
			return m_pSound;
		else
			return (m_pSound=pSnd);
	};

	/* Channel */
	int Channel(int ch =0)
	{
		if(!ch)
			return m_iChannel;
		else
			return (m_iChannel=ch);
	};

	/* stream test */
	bool Stream(int val =-1)
	{
		if(val==-1)
			return m_IsStream;
		else
			return (m_IsStream = (bool)val);  
	};
	
	newString Name(newString name ="")
	{
		if(name=="")
			return m_strName;

		return (m_strName=name);
	};

	bool Play()
	{
		MainConsole.Add(0, "Playing: %s", Name().c_str() );

		/* Break pause or start playing */
		if(FSOUND_GetPaused(m_iChannel))

			FSOUND_SetPaused(m_iChannel, false);

		else
		{
			if(m_IsStream)
				m_iChannel = FSOUND_Stream_Play(FSOUND_FREE, (FSOUND_STREAM*)m_pSound);
			else
				m_iChannel = FSOUND_PlaySound(FSOUND_FREE, (FSOUND_SAMPLE*)m_pSound);
		}
		if(m_iChannel == -1)
		{

MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "!!! Sound.Play: %s", FMOD_ErrorString(FSOUND_GetError()));

			return false;
		}
		// set stored sound level
		FSOUND_SetVolume(m_iChannel, m_iVolume);
		return true;
	};

	bool IsPlay()
	{
		if(m_IsStream)
			return true;
		else
			return FSOUND_IsPlaying(m_iChannel);
	}

	bool Stop()
	{
		if(m_IsStream)
			return FSOUND_Stream_Stop((FSOUND_STREAM*)m_pSound);
		else
			return FSOUND_StopSound(m_iChannel);
	};

	bool Pause()
	{
		return FSOUND_SetPaused(m_iChannel, true);
	};

	bool Volume(int value)
	{
	m_iVolume = (float)value/100*255;

		// if sound currently in play - set volume now
		if(m_iChannel)

			FSOUND_SetVolume(m_iChannel, m_iVolume);

		return true;
	};
};

typedef std::set<CCustomSound*> setSound_t;

setSound_t g_SetSound;


/* Callbacks */
unsigned int FMCustomOpenR(const char *name)
{
//TODO: +++ 2.	CFile* pFile = new CFile();
	newFile *  pFile = new newFile();

	if ((pFile->Open(name, NEW_FILE_READ, true)) != NEW_OK)
		
		return 0;

	return (unsigned int)pFile;
};

void FMCustomCloseR(unsigned int handle)
{
	((newFile*)handle)->Close();
};

int FMCustomRead(void *buffer, int size, newFile &stream)
{
	stream.ReadBuf(buffer, size);

	return 0;
};

int FMCustomRSeek(unsigned int handle, int pos, signed char mode)
{
	((newFile*)handle)->Seek(pos,mode);

	return 0;
};

int FMCustomRTell(unsigned int handle)
{
	return ((newFile*)handle)->GetCurPos();
};

/* CSoundManager */
CSoundManager::CSoundManager()
{
	m_IsInit	= false;
};

CSoundManager::~CSoundManager()
{
	if(!IsValid())

		return;

	/* close all currently played sounds */
	for(setSound_t::iterator it=g_SetSound.begin(); it!=g_SetSound.end(); it++)

		delete (*it);

	/* de-initialize fmod lib */
	FSOUND_Close();
};

int CSoundManager::Init()
{
	if (FSOUND_GetVersion() < FMOD_VERSION)
	{
		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "!!! Error : Wrong DLL version! Should be %.02f", FMOD_VERSION);

		return true;
	}

	FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND); //FSOUND_OUTPUT_WINMM FSOUND_OUTPUT_DSOUND

	const char* sound_out = NULL;

	switch (FSOUND_GetOutput())
	{
		case FSOUND_OUTPUT_NOSOUND:
			sound_out = "NoSound";
			break;

		case FSOUND_OUTPUT_WINMM:
			sound_out = "Windows Multimedia Waveout";
			break;

		case FSOUND_OUTPUT_DSOUND:
			sound_out = "Direct Sound";
			break;

		case FSOUND_OUTPUT_ASIO:
			sound_out = "ASIO";
			break;	 
	};

	MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "Selected sound output: %s", sound_out);

	// setting default driver
	FSOUND_SetDriver(0);

	MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "Selecting default sound driver.");

	{
	unsigned int caps = 0;

		FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
		
		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "Driver capabilities =======================");

		if (!caps)
		{
			MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "- This driver will support software mode only.");

			MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "  It does not properly support 3D sound hardware.");
		}
		if (caps & FSOUND_CAPS_HARDWARE)

		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "- Driver supports hardware 3D sound!");

		if (caps & FSOUND_CAPS_EAX2)

		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "- Driver supports EAX 2!");

		if (caps & FSOUND_CAPS_EAX3)

		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "- Driver supports EAX 3!");

		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "===========================================");
	}

	// init fmod lib
	if (!FSOUND_Init(44100, 32, 0))
	{
		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "!!! FSOUND_Init: %s", FMOD_ErrorString(FSOUND_GetError()));

		//return false;
		return true;
	}

	m_IsInit = true;

	return true;
};

CCustomSound* CSoundManager::Precache_Sound(const char* pstrSound, bool bLoop)
{
	if(!IsValid())

		return NULL;

	/* Load as sound */
	FSOUND_SAMPLE* pSample = FSOUND_Sample_Load(FSOUND_FREE, pstrSound, FSOUND_STEREO, 0);

	if (!pSample)
	{
		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "!!! Sound.Precache: %s (%s)", FMOD_ErrorString(FSOUND_GetError()), pstrSound);

		FSOUND_Sample_Free(pSample);

		return NULL;
	}
	else
	{
		if(bLoop)

			FSOUND_Sample_SetMode(pSample, FSOUND_LOOP_NORMAL);
	}

	/* Create internal */
	CCustomSound* pSnd = new CCustomSound();

	/* store to internal */
	pSnd->Sound((int*)pSample);

	/* store name */
	pSnd->Name(pstrSound);

	/* add to storage */
	g_SetSound.insert(pSnd);

	return pSnd;
};

CCustomSound* CSoundManager::Precache_Stream(const char* pstrMusic, bool bLoop)
{
	if(!IsValid()) return NULL;

	// load as stream
	FSOUND_STREAM* pSample = FSOUND_Stream_OpenFile(pstrMusic, (bLoop)?FSOUND_LOOP_NORMAL:FSOUND_LOOP_OFF, 0);

	if (!pSample)
	{
		MainConsole.Add(NEW_ASEDBG_MESSAGE_LEVEL, "!!! Sound.Load: %s (%s)", FMOD_ErrorString(FSOUND_GetError()), pstrMusic);

		FSOUND_Stream_Close(pSample);

		return NULL;
	}
	// create internal
	CCustomSound* pSnd = new CCustomSound();

	// store to internal
	pSnd->Sound((int*)pSample);

	// store name
	pSnd->Name(pstrMusic);

	// set streamed
	pSnd->Stream(true);

	// add to storage
	g_SetSound.insert(pSnd);
	return pSnd;
};

bool CSoundManager::Play(CCustomSound* pSoundObject)
{
	if(IsValid())
	{
		/* play selected sound */
		setSound_t::iterator it = g_SetSound.find(pSoundObject);

		if(it!=g_SetSound.end())

			return (*it)->Play();
	}

	return false;
};

bool CSoundManager::IsPlay(CCustomSound* pSoundObject)
{
	if(IsValid())
	{
		/* Play selected sound */
		setSound_t::iterator it = g_SetSound.find(pSoundObject);

		if(it!=g_SetSound.end())

			return (*it)->IsPlay();
	}

	return false;
};

bool CSoundManager::Stop(CCustomSound* pSoundObject)
{
	/* Stop selected sound */
	setSound_t::iterator it = g_SetSound.find(pSoundObject);

	if(it!=g_SetSound.end())

		return (*it)->Stop();

	return false;
};

bool CSoundManager::Pause(CCustomSound* pSoundObject)
{
	/* Pause selected sound */
	setSound_t::iterator it = g_SetSound.find(pSoundObject);	

	if(it!=g_SetSound.end())

		return (*it)->Pause();

	return false;
};

bool CSoundManager::Close(CCustomSound** pSoundObject)
{
	/* Destroy selected sound */
	setSound_t::iterator it = g_SetSound.find(*pSoundObject);

	if(it!=g_SetSound.end())
	{
		delete *pSoundObject;

		*pSoundObject=NULL;

		g_SetSound.erase(it);

		return true;
	}
	return false;
};

bool CSoundManager::Volume(CCustomSound* pSoundObject, int volume)
{

	if(IsValid())
	{
		/* Play selected sound */
		setSound_t::iterator it = g_SetSound.find(pSoundObject);

 		if(it!=g_SetSound.end())

			return (*it)->Volume(volume);

	}

	return false;
};
