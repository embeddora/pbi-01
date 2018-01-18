#include "File.h"

#include "Console.h"

#include <sys/io.h>

struct newPackFile
{
	newString Name;

	std::list<newString> FileList;
};

static std::list<newPackFile> PackSystem;

/* UnIMP parts */
extern "C"
{
	#include "unimp/unimp.h"
}

/* File System Parts */
void ParsePackFile(newPackFile &pfile)
{
	if (extract_file((char *)pfile.Name.c_str(), 1, NULL, NULL, NULL) != 0)

		pfile.Name = "";
}

void ParsePathForPackFiles(newString DirPath)
{
#if 0
	_finddata_t fdata;

	long hFile;

	if (DirPath.size()>0)
	{
		if (DirPath[DirPath.size()-1] != '\\')

			DirPath += '\\';

	}/* else DirPath = ""; */

	hFile = _findfirst((DirPath + "*.*").c_str(), &fdata);

	if (hFile == -1L)
		
		return;
	
	do 
	{
		if (fdata.attrib & _A_SUBDIR)
		{
			if (newString(fdata.name) != newString("."))

				if (newString(fdata.name) != newString(".."))

					ParsePathForPackFiles(DirPath + fdata.name);

				continue;
		}
		newPackFile pfile;		

		pfile.Name = DirPath + fdata.name;

		/* Parse pack file and fill data */
		ParsePackFile(pfile);

		if (!pfile.Name.empty())
		{
			PackSystem.push_back(pfile);

			MainConsole.Add(0, "Detected pack file: %s", pfile.Name.c_str());
		}
		
	} while (_findnext( hFile, &fdata ) == 0);

	_findclose(hFile);
#else
	printf("[error] on cirrent platf. the '%s' yet to be defined on \n", __func__);
#endif /* (0) */
}

void InitFileSystem(newString Path)
{
	PackSystem.clear();

	ParsePathForPackFiles(Path);
}


newFile::newFile(void)
{
	StorageType = NEW_STORAGETYPE_DEFAULT;

	pFile = NULL;

	pMemBuf = NULL;
}

newFile::~newFile(void)
{
	Close();
}

newResult newFile::Open(const newString &FileName, newEnum op, BOOL Binary, newEnum SType)
{
	switch(SType)
	{
		case NEW_STORAGETYPE_FILE:

			switch(op)
			{
				case NEW_FILE_READ:

					pFile = fopen(FileName.c_str(), Binary? "rb" : "rt");

					if (!pFile)
						
						return GenError(NEW_FILE_NOT_FOUND, FileName.c_str());

					break;
				case NEW_FILE_WRITE:

					pFile = fopen(FileName.c_str(), Binary? "wb" : "wt");

					break;

				default:
				;
			}
			
			break;

		case NEW_STORAGETYPE_MEMORY:

			break;

		case NEW_STORAGETYPE_PAKFILE:

			break;

		default:

			/* Default from File */
			switch(op)
			{
				case NEW_FILE_READ:

					pFile = fopen(FileName.c_str(), Binary? "rb" : "rt");

					/* if (!pFile) return GenError(NEW_FILE_NOT_FOUND, FileName.c_str()); */
					if (pFile)
						
						StorageType = NEW_STORAGETYPE_FILE;

					break;

				case NEW_FILE_WRITE:

					pFile = fopen(FileName.c_str(), Binary? "wb" : "wt");

					if (pFile)

						StorageType = NEW_STORAGETYPE_FILE;

					break;

				default:
					;
			}

			/* not default - from PackFile */
			if (!pFile) 
			{
				pMemBuf = NULL;

				for (std::list<newPackFile>::iterator it=PackSystem.begin(); it!=PackSystem.end(); it++)
				{
					int error = extract_file((char *)(*it).Name.c_str(), 0, &pMemBuf, (char *)FileName.c_str(), (long unsigned int *) &SizeInBytes);

					if (!error)

						break;
				}
				if (!pMemBuf)

					return GenError(NEW_FILE_NOT_FOUND, FileName.c_str());

				CurPos = 0;

				StorageType = NEW_STORAGETYPE_PAKFILE;
			}

	}
	Name = FileName;

	return NEW_OK;
}
void newFile::Close()
{
	switch(StorageType)
	{
		case NEW_STORAGETYPE_FILE:

			if (pFile)
				
				fclose(pFile);

			pFile = NULL;

			break;

		case NEW_STORAGETYPE_MEMORY:

			break;

		case NEW_STORAGETYPE_PAKFILE:

			break;

		default: ;
	}

	if (pMemBuf)
	{
#if 0
		delete pMemBuf;
#else
	/* 18:39 01182018 */
	printf("[error] in '%s' don't know how to 'delete(void*)' \n", __func__);
#endif /* (0) */

		pMemBuf = NULL;
	}

	Name = "";
}

BOOL newFile::Feof()
{
	switch(StorageType)
	{
		case NEW_STORAGETYPE_FILE:

			if (pFile)
				
				return feof(pFile);

			break;

		case NEW_STORAGETYPE_MEMORY:

			break;

		case NEW_STORAGETYPE_PAKFILE:

			break;

		default: ;
	}

	return (CurPos>=SizeInBytes);
}

void newFile::ConvertToMemoryFile()
{
	if (StorageType != NEW_STORAGETYPE_FILE)

		return;

	int CurPos = ftell(pFile);

	int SizeInBytes = fseek(pFile,0, SEEK_END);

	SizeInBytes = ftell(pFile);

	fseek(pFile,0, SEEK_SET);

	pMemBuf = new BYTE[SizeInBytes];

	fread(pMemBuf, SizeInBytes, 1, pFile);

	fclose(pFile);

	pFile = NULL;

	StorageType = NEW_STORAGETYPE_MEMORY;
}
