#ifndef __FILE_H__
#define __FILE_H__

#include "Types.h"

#include "Math.h"

#include "Console.h"

void memcpy(void *, void *, size_t);

class newFile
{
public:

	newFile(void);

	~newFile(void);

	newString Name;

	newResult Open(const newString &FileName, newEnum op = NEW_FILE_READ, BOOL Binary = TRUE, newEnum SType = NEW_STORAGETYPE_DEFAULT);

	void Close();

	BOOL Feof();

	unsigned long GetCurPos()
	{
		if (StorageType == NEW_STORAGETYPE_FILE)

			return ftell(pFile);
		else
			return CurPos;
	};

	newString ReadString(UINT size=0)
	{
	newString temp;

		if (size)
		{			
			temp.resize(size);

			ReadBuf((char *)temp.c_str(),size);

			return temp;
		}
		else
		{
		char b;

			Read(b);

			while (b!=0)
			{
				temp += b;

				Read(b);
			}
			return temp;
		}
		return "";
	}

	BOOL ReadTextString(newString &res)
	{
	unsigned char h;

		res="";

		Read(h);

		while (((h==9)||(h==' '))&&!Feof())
			
			Read(h);

		while ((h!='\n')&&!Feof())
		{
			if (h!=13)

				res+=h;

			Read(h);

			if (h==9)

				h=' ';
		}
		if (Feof())

			return FALSE;
		else

			return TRUE;
	}

	template <class T>
	void Read(T *buf, UINT size=0)
	{
		if (size)
		{
			if (NEW_STORAGETYPE_FILE == StorageType)

				fread(buf, size, 1, pFile);

			else
				if (!Feof())
				{
					memcpy(buf, (BYTE *)pMemBuf + CurPos, size);

					CurPos += size;
				}
		}
	}

	template <class T>
	void Read(T &buf, size_t size = 1)
	{
	UINT sizet = sizeof(T);

		ReadBuf(&buf, sizet * size);
		/*
		if (size)
		{
			if (NEW_STORAGETYPE_FILE == StorageType)

				fread(&buf, sizet,size, pFile);

			else if (!Feof())
			{
				memcpy(&buf, (BYTE *)pMemBuf + CurPos, size * sizet);

				MainConsole.Add(0, "Pos = %i, Size = %i", CurPos, size * sizet);

				CurPos += size * sizet;
			}
		}
		*/
	}

	template <class T>
	void Write(const T &buf, size_t size = 1)
	{
		if (size)
		{
			if (NEW_STORAGETYPE_FILE == StorageType)

				fwrite(&buf, sizeof(T),size, pFile);

			else
				if (!Feof())
				{
					memcpy((BYTE *)pMemBuf + CurPos, &buf, size * sizeof(T));

					CurPos += size * sizeof(T);
				}
		}
	}

	void ReadBuf(void *buf, size_t size)
	{
		if (size)
		{
			if (NEW_STORAGETYPE_FILE == StorageType)

				fread(buf, size,1, pFile);

			else //if (!Feof())
			{
				memcpy(buf, (BYTE *)pMemBuf + CurPos, size);

				CurPos += size;
			}
		}
	}

	void Seek(UINT Position, int From)
	{
		if (NEW_STORAGETYPE_FILE == StorageType)

			fseek(pFile, Position, From);

		else
		{
			switch(From)
			{
				case SEEK_SET:

					CurPos = Position;

					break;

				case SEEK_CUR:

					CurPos += Position;

					break;

				case SEEK_END:

					CurPos = SizeInBytes - Position;

					break;
			}
		}

	}

	void ConvertToMemoryFile();

	void *GetMemoryData() { return pMemBuf;}

	UINT GetSizeInBytes() { return SizeInBytes;}

	newEnum GetStorageType() { return StorageType; }

	FILE * GetFile() {return pFile;}

private:

	newEnum StorageType;

	FILE *pFile;

	void *pMemBuf;

	UINT SizeInBytes;

	UINT CurPos;
};

void InitFileSystem(newString Path);

#endif /* ifndef-define include guard */
