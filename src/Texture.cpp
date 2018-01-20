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

#include "Texture.h"

#include "Render.h"

#include "Console.h"

#include "File.h"

/* 15:44 18-01-2018  .remove */
#undef  USE_LIBJPEG

#ifdef USE_LIBJPEG

	/* Библиотека работы с файлами формата JPEG */
	extern "C"
	{
		#include "LibJPEG/jpeglib.h"
	}
	#pragma comment( lib, "libjpeg.lib" )

#endif /* USE_LIBJPEG */

void* newTexture::memcpy(void* __dest, const void* __src, int __n)
{
int i;
char *d, *s;

	d = (char *)__dest;
	s = (char *)__src;
	
	for (i=0; i<__n; i++)
	    d[i] = s[i];
	    
	return __dest;
}


newManager<newTexture> TextureMan;

/* Textures &Texture; */
Textures &Texture = TextureMan.Item;

newTexture::~newTexture(void)
{
}

/* Создаёт текстуру заполненую цветом в 3 компоненты */
void newTexture::CreateEmpty(int width,int height, newColor3ub &c)
{
	/* Сохраняем ширину и высоту */
	Width=width;

	Height=height;

	/* Бит на пиксель */
    Bpp=sizeof(newColor3ub)*8;

	/* Выделяем место под текстуру */
	Data.resize(width*height*sizeof(newColor3ub));

	/* Заполняем заданным цветом */
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(newColor3ub))

		memcpy(&*i,&c,sizeof(newColor3ub));

}

void newTexture::CreateEmpty4(int width,int height, newColor4ub &c)
{
	/* Сохраняем ширину и высоту */
	Width=width;

	Height=height;

	/* Бит на пиксель */
	Bpp=sizeof(newColor4ub)*8;

	/* Выделяем место под текстуру */
	Data.resize(width*height*sizeof(newColor4ub));

	/* Заполняем заданным цветом */
	for (std::vector<BYTE>::iterator i=Data.begin();i<Data.end();i+=sizeof(newColor4ub))

		memcpy(&*i,&c,sizeof(newColor4ub));
}

/* Вырезаем из названия файла его расширение */
void ExtractFileExtension(const newString &FileName,newString &Ext)
{
	size_t i=FileName.rfind('.',FileName.size()-1);

	Ext=FileName.substr(i+1,FileName.size()-i-1);

//+++ basically uppercase and lowercase are not the same things on IX (ext2, etc), so we're skipping this transform: 	_strupr((char *)Ext.c_str());
}

/* Загружаем текстуру из файла */
newResult newTexture::LoadFromFile(const newString &FileName)
{
	if (Texture.find(FileName)!=Texture.end())
	{
		if (isLoaded) return GenError(NEW_EXIST,FileName.c_str());
	}
	newString Ext;

	/* Вырезаем из названия расширение */
	ExtractFileExtension(FileName,Ext);

	MainConsole.Add(0,"Загрузка текстуры: %s",FileName.c_str());

	/* Сравниваем и запускаем соответсвующую функцию */
	try
	{
		if (Ext=="TGA")
			
#if 0
			return LoadTGAFile(FileName);
#else
			printf("[error] %s, skipped LoadTGAFile(FileName)\n", __func__);
#endif /* (0) */
		
		if (Ext=="JPG")
			
#if 0
			return LoadJPGFile(FileName);
#else
			printf("[error] %s, skipped LoadJPEGFile(FileName)\n", __func__);
#endif /* (0) */
	}

	catch (...)
	{
		MainConsole.Add(0, "Неизвестная ошибка при загрузке текстуры: %s", FileName.c_str());
	}

	/* Неизвестное расширение, возращаем False */
	return GenError(NEW_UNKNOW_FILE_FORMAT, (char *)FileName.c_str());
}

#pragma pack(push)

#pragma pack(1)

typedef struct
{
	unsigned char IDLength;

	unsigned char ColorMapType;

	unsigned char ImageType;

	unsigned short ColorMapStart;

	unsigned short ColorMapLength;

	unsigned char ColorMapDepth;

	unsigned short XOffset;

	unsigned short YOffset;

	unsigned short Width;

	unsigned short Height;

	unsigned char Depth;

	unsigned char ImageDescriptor;

	unsigned char *ColorMap;

	unsigned char *Data;

} TGA_t;

#pragma pack(pop)

void TGA_GetPackets(unsigned char *data, int width, int height, int depth, newFile &stream)
{
	int current_byte=0, run_length, i;

	unsigned char header, buffer8[4];

	unsigned short buffer16;

	int bpp;

	if(depth==16)

		bpp=3;

	else

		bpp=depth/8;

	while(current_byte<width*height*bpp)
	{
		stream.ReadBuf(&header, sizeof(unsigned char) * 1 );

		run_length=(header&0x7F)+1;

		if(header&0x80)
		{
			if(depth==32)

				stream.ReadBuf(buffer8, sizeof(unsigned char) * 4 );

			if(depth==24)

				stream.ReadBuf(buffer8, sizeof(unsigned char) * 3 );

			if(depth==16)

				stream.ReadBuf(&buffer16, sizeof(unsigned short) * 1 );

			if(depth==8)

				stream.ReadBuf(buffer8, sizeof(unsigned char) * 1 );

			for(i=0;i<run_length;i++)
			{
				if(depth==32)
				{
					data[current_byte++]=buffer8[0];

					data[current_byte++]=buffer8[1];

					data[current_byte++]=buffer8[2];

					data[current_byte++]=buffer8[3];
				}

				if(depth==24)
				{
					data[current_byte++]=buffer8[0];

					data[current_byte++]=buffer8[1];

					data[current_byte++]=buffer8[2];
				}

				if(depth==16)
				{
					data[current_byte++]=(buffer16&0x1F)<<3;

					data[current_byte++]=((buffer16>>5)&0x1F)<<3;

					data[current_byte++]=((buffer16>>10)&0x1F)<<3;
				}

				if(depth==8)

					data[current_byte++]=buffer8[0];
			}
		}

		if(!(header&0x80))
		{
			for(i=0;i<run_length;i++)
			{
				if(depth==32)
				{
					stream.ReadBuf(buffer8, sizeof(unsigned char) * 4 );

					data[current_byte++]=buffer8[0];

					data[current_byte++]=buffer8[1];

					data[current_byte++]=buffer8[2];

					data[current_byte++]=buffer8[3];
				}

				if(depth==24)
				{
					stream.ReadBuf(buffer8, sizeof(unsigned char) * 3 );

					data[current_byte++]=buffer8[0];

					data[current_byte++]=buffer8[1];

					data[current_byte++]=buffer8[2];
				}

				if(depth==16)
				{
					stream.ReadBuf(&buffer16, sizeof(unsigned short) * 1 );

					data[current_byte++]=(buffer16&0x1F)<<3;

					data[current_byte++]=((buffer16>>5)&0x1F)<<3;

					data[current_byte++]=((buffer16>>10)&0x1F)<<3;
				}

				if(depth==8)
				{
					stream.ReadBuf(buffer8, sizeof(unsigned char) * 1 );

					data[current_byte++]=buffer8[0];
				}
			}
		}
	}
}

/* Необходимые константы для загрузки TGA */
const int TARGA_NO_COLORMAP = 0;

const int TARGA_COLORMAP = 1;

const int TARGA_EMPTY_IMAGE = 0;

const int TARGA_INDEXED_IMAGE = 1;

const int TARGA_TRUECOLOR_IMAGE = 2;

const int TARGA_BW_IMAGE = 3;

const int TARGA_INDEXED_RLE_IMAGE = 9;

const int TARGA_TRUECOLOR_RLE_IMAGE = 10;

const int TARGA_BW_RLE_IMAGE = 11;

newResult TGA_GetData(char *file, TGA_t *TGA)
{
	newFile stream;

	/* if((stream=fopen(file, "rb"))==NULL) */
	newResult res;

	if ( (res = stream.Open(file, NEW_FILE_READ, TRUE)) != NEW_OK)
	{
		MainConsole.Add(0,"Не найден файл: %s",file);

		return res;
	}

	stream.ReadBuf(&TGA->IDLength, sizeof(unsigned char) * 1 );

	stream.ReadBuf(&TGA->ColorMapType, sizeof(unsigned char) * 1 );

	stream.ReadBuf(&TGA->ImageType, sizeof(unsigned char) * 1 );

	stream.ReadBuf(&TGA->ColorMapStart, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->ColorMapLength, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->ColorMapDepth, sizeof(unsigned char) * 1 );

	stream.ReadBuf(&TGA->XOffset, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->XOffset, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->Width, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->Height, sizeof(unsigned short) * 1 );

	stream.ReadBuf(&TGA->Depth, sizeof(unsigned char) * 1 );

	stream.ReadBuf(&TGA->ImageDescriptor, sizeof(unsigned char) * 1 );

	stream.Seek(TGA->IDLength, SEEK_CUR);

	switch(TGA->ImageType)
	{
		case 1:

			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
				TGA->ColorMap=(unsigned char *)malloc(TGA->ColorMapLength*(TGA->ColorMapDepth/8));

				stream.ReadBuf(TGA->ColorMap, sizeof(unsigned char) * TGA->ColorMapLength*(TGA->ColorMapDepth/8) );
			}
			else
			{
				/* MessageBox(NULL, "Only 24bit paletted images are supported.", "TGA", MB_OK); */

				stream.Close();

				return GenError(NEW_UNKNOW, "Only 24bit paletted images are supported.");
			}
			break;

		case 2:

			break;

		case 3:

			break;

		case 9:

			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
				TGA->ColorMap=(unsigned char *)malloc(TGA->ColorMapLength*(TGA->ColorMapDepth/8));

				stream.ReadBuf(TGA->ColorMap, sizeof(unsigned char) * TGA->ColorMapLength*(TGA->ColorMapDepth/8) );
			}
			else
			{
				/* MessageBox(NULL, "Only 24bit paletted images are supported.", "TGA", MB_OK); */

				stream.Close();

				return GenError(NEW_UNKNOW, "Only 24bit paletted images are supported.");
			}
			break;

		case 10:

			break;

		case 11:

			break;

		default:

			/* MessageBox(NULL, "Unsupported image type.", "TGA", MB_OK); */

			stream.Close();

			return GenError(NEW_UNKNOW, "Unsupported image type.");;

			break;
	}

	switch(TGA->Depth)
	{
		case 32:

			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*4);

			if(TGA->ImageType==2)
			{
				stream.ReadBuf(TGA->Data, sizeof(unsigned char) * TGA->Width*TGA->Height*4 );

				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);

				break;
			}
			break;

		case 24:

			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);

			if(TGA->ImageType==2)
			{
				stream.ReadBuf(TGA->Data, sizeof(unsigned char) * TGA->Width*TGA->Height*3 );

				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);

				break;
			}
			break;

		case 16:

			TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);

			if(TGA->ImageType==2)
			{
			unsigned short *buffer=(unsigned short *)malloc(sizeof(unsigned short)*TGA->Width*TGA->Height);

			int i;

				stream.ReadBuf(buffer, sizeof(unsigned short) * TGA->Width*TGA->Height );

				for(i=0;i<TGA->Width*TGA->Height;i++)
				{
					TGA->Data[3*i]=(buffer[i]&0x1F)<<3;

					TGA->Data[3*i+1]=((buffer[i]>>5)&0x1F)<<3;

					TGA->Data[3*i+2]=((buffer[i]>>10)&0x1F)<<3;
				}

				free(buffer);

				TGA->Depth=24;

				break;
			}

			if(TGA->ImageType==10)
			{
				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);

				TGA->Depth=24;

				break;
			}
			break;

		case 8:

			if(TGA->ColorMapType==1&&TGA->ColorMapDepth==24)
			{
			unsigned char *buffer;

			int i;

				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height*3);

				buffer=(unsigned char *)malloc(TGA->Width*TGA->Height);

				if(TGA->ImageType==9)

					TGA_GetPackets(buffer, TGA->Width, TGA->Height, TGA->Depth, stream);

				else

					stream.ReadBuf(buffer, sizeof(unsigned char) * TGA->Width*TGA->Height );

				for(i=0;i<TGA->Width*TGA->Height;i++)
				{
					TGA->Data[3*i]=TGA->ColorMap[3*buffer[i]];

					TGA->Data[3*i+1]=TGA->ColorMap[3*buffer[i]+1];

					TGA->Data[3*i+2]=TGA->ColorMap[3*buffer[i]+2];
				}

				free(buffer);

				free(TGA->ColorMap);

				TGA->Depth=24;

				break;
			}

			if(TGA->ImageType==3)
			{
				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height);

				stream.ReadBuf(TGA->Data, sizeof(unsigned char) * TGA->Width*TGA->Height);

				break;
			}

			if(TGA->ImageType==11)
			{
				TGA->Data=(unsigned char *)malloc(TGA->Width*TGA->Height);

				TGA_GetPackets(TGA->Data, TGA->Width, TGA->Height, TGA->Depth, stream);

				break;
			}
			break;

		default:

//+++			MessageBox(NULL, "Unsupported image depth.", "TGA", MB_OK);
			printf("Unsupported image depth -- TGA");

			break;
	}

	stream.Close();

	MainConsole.Add(0,"Текстура: ОК");

	return NEW_OK;
}



/* Функция загрузки TGA файла */
newResult newTexture::LoadTGAFile(const newString &FileName)
{
static TGA_t TGA;

newResult res;

	if ((res = TGA_GetData((char *)FileName.c_str(),&TGA)) != NEW_OK)
	{
		return res;
	}

	/* Бит на пиксель */
	Bpp=TGA.Depth;

	/* Ширина */
	Width = TGA.Width;

	/* Высота */
	Height = TGA.Height;

	/* Размер изображения в байтах */
	UINT ImageSize=Width*Height*(Bpp/8);

	/* Выделяем память под текстуру */
	Data.resize(ImageSize);

	/* Копируем загруженые данные */
	memcpy((void *)&Data[0],TGA.Data,ImageSize);

	/* Имя текстуры */
	Name = FileName;

	isLoaded = TRUE;

	return NEW_OK;
}

#ifdef USE_LIBJPEG


newFile *pFile = NULL;

METHODDEF(void) my_init_source(j_decompress_ptr pcinfo)
{
	/* pFile->ConvertToMemoryFile(); */

	pcinfo->src->next_input_byte = (JOCTET *)pFile->GetMemoryData();

	pcinfo->src->bytes_in_buffer = pFile->GetSizeInBytes();
}

METHODDEF(boolean) my_fill_input_buffer(j_decompress_ptr pcinfo)
{
	return TRUE;
}

METHODDEF(boolean) my_resync_to_restart(j_decompress_ptr pcinfo, int desired)
{
	return NULL;
}

METHODDEF(void) my_skip_input_data(j_decompress_ptr pcinfo, long num_bytes)
{
	
}

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	/* Always display the message */
	(*cinfo->err->output_message) (cinfo);
	
	/* Let the memory manager delete any temp files before we die .	jpeg_destroy(cinfo); */
}

METHODDEF(void) my_emit_message (j_common_ptr cinfo, int msg_level)
{
struct jpeg_error_mgr * err = cinfo->err;
	
	if (msg_level < 0)
	{
		/* It's a warning message.  Since corrupt files may generate many warnings, the policy implemented here is to show only the first warning, unless trace_level >= 3.	*/
		if (err->num_warnings == 0 || err->trace_level >= 3)

			(*err->output_message) (cinfo);

		/* Always count warnings in num_warnings. */
		err->num_warnings++;
	}
	else
	{
		/* It's a trace message.  Show it if trace_level >= msg_level. */
		if (err->trace_level >= msg_level)

			(*err->output_message) (cinfo);
	}
}

/* Routine that actually outputs a trace or error message */
METHODDEF(void) my_output_message (j_common_ptr cinfo)
{
char buffer[JMSG_LENGTH_MAX];
	
	/* Create the message */
	(*cinfo->err->format_message) (cinfo, buffer);

	/* Send it to stderr, adding a newline . MainConsole.Add(0, "WARNING: %s", buffer); */
}

/* Format a message string for the most recent JPEG error or message */
METHODDEF(void) my_format_message (j_common_ptr cinfo, char * buffer)
{
struct jpeg_error_mgr * err = cinfo->err;

int msg_code = err->msg_code;

const char * msgtext = NULL;

const char * msgptr;

char ch;

boolean isstring;
	
	/* Look up message string in proper table */
	if (msg_code > 0 && msg_code <= err->last_jpeg_message)
	{
		msgtext = err->jpeg_message_table[msg_code];
	}
	else
		if (err->addon_message_table != NULL &&	msg_code >= err->first_addon_message &&		msg_code <= err->last_addon_message)
		{
			msgtext = err->addon_message_table[msg_code - err->first_addon_message];
		}
	
	/* Defend against bogus message number */
	if (msgtext == NULL)
	{
		err->msg_parm.i[0] = msg_code;

		msgtext = err->jpeg_message_table[0];
	}
	
	/* Check for string parameter, as indicated by %s in the message text */
	isstring = FALSE;

	msgptr = msgtext;

	while ((ch = *msgptr++) != '\0')
	{
		if (ch == '%')
		{
			if ('s' == *msgptr)
				
				isstring = TRUE;

			break;
		}
	}
	
	/* Format the message into the passed buffer */
	if (isstring)

		sprintf(buffer, msgtext, err->msg_parm.s);

	else

		sprintf(buffer, msgtext,err->msg_parm.i[0], err->msg_parm.i[1],err->msg_parm.i[2], err->msg_parm.i[3],err->msg_parm.i[4], err->msg_parm.i[5],err->msg_parm.i[6], err->msg_parm.i[7]);	
}

/* Загрузка текстуры из файла JPG */
newResult newTexture::LoadJPGFile(const newString &FileName)
{
	newFile stream;

	/* if((stream=fopen(file, "rb"))==NULL) */

	newResult res;

	if ( (res = stream.Open(FileName, NEW_FILE_READ, TRUE)) != NEW_OK)
	{
		/* MainConsole.Add(0,"Не найден файл: %s",FileName.c_str()); */

		return res;
	}
	
	jpeg_decompress_struct cinfo;

	jpeg_error_mgr jerr;

	/* cinfo.client_data = this; */

	cinfo.err = jpeg_std_error(&jerr);

	jerr.error_exit = my_error_exit;

	jerr.emit_message = my_emit_message;

	jerr.output_message = my_output_message;

	jerr.format_message = my_format_message;

	/* jerr.trace_level = 10; */
		
	jpeg_create_decompress(&cinfo);

	jpeg_source_mgr src_mgr;

	if (stream.GetStorageType() != NEW_STORAGETYPE_FILE)
	{
		src_mgr.init_source = my_init_source;

		src_mgr.fill_input_buffer = my_fill_input_buffer;

		src_mgr.skip_input_data = my_skip_input_data;

		src_mgr.resync_to_restart = my_resync_to_restart;

		src_mgr.term_source = my_init_source;
		
		cinfo.src = &src_mgr;
		
		pFile = &stream;
	}
	else
	{
		jpeg_stdio_src(&cinfo, stream.GetFile());
	}

	jpeg_read_header(&cinfo, TRUE);

	int x = cinfo.image_width;

	jpeg_start_decompress(&cinfo);

	/* Бит на пиксель */
	Bpp=cinfo.out_color_components * 8;

	/* Ширина */
	Width = cinfo.output_width;

	/* Высота */
	Height = cinfo.output_height;

	/* Размер изображения в байтах */
	UINT ImageSize=Width*Height*(Bpp/8);

	UINT row_stride = Width * Bpp/8;
	
	/* Выделяем память под текстуру */
	Data.resize(ImageSize);

	/* Копируем загруженые данные.	memcpy((void *)&Data[0],TGA.Data,ImageSize); */

	int nowRead = 0;

	while (nowRead<Height)
	{
	const int maxRowsPerRead = 8;

	JSAMPROW row_pointers[maxRowsPerRead];

		for(int row=0; row<maxRowsPerRead; row++)
		{
			row_pointers[row] = &Data[(nowRead + row) * row_stride];
		}
		
		nowRead += jpeg_read_scanlines(&cinfo, row_pointers, maxRowsPerRead);
	}
	
	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	stream.Close();

	/* Имя текстуры */
	Name = FileName;
	
	isLoaded = TRUE;
	
	MainConsole.Add(0,"Текстура: ОК");
	
	return NEW_OK;

}

BOOL newSaveScreenToJPG()
{
UINT image_width = MainSettings.SCREEN_WIDTH;

UINT image_height = MainSettings.SCREEN_HEIGHT;

/* Выделяем место под изображение Ширина*Высота*Байт_на_пиксель */
BYTE *Data=new BYTE[image_width * image_height * 3];

	/* Копируем изображение с экрана */
	newGetScreen(Data);

	/* Изображение перевернутое, по вертикали . Длинна линии*/
	UINT LineLen=MainSettings.SCREEN_WIDTH*3; 

	/* Для перемещения цвета */
	BYTE *c=new BYTE[LineLen];

	for (UINT i=0;i<MainSettings.SCREEN_HEIGHT/2;i++)
	{
		memcpy(c,Data+i*LineLen,LineLen);

		memcpy(Data+i*LineLen,Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,LineLen);

		memcpy(Data+(MainSettings.SCREEN_HEIGHT-i-1)*LineLen,c,LineLen);
	}
	delete c;
	
	/* Имя файла */
	newString FileName;
	
	/* Длинна названия файла = 15 */
	FileName.resize(15);

	/* Ссылка на название */
	char *da=&FileName[0]; 
	
	/* Цикл для нахождения свободного файла */
	for (i=0;i<1000;i++)
	{
		/* Создаём название. Получаем Screen000.jpg, Screen001.jpg, и т.д. */
		sprintf(da,"Screen%03d.jpg",i);

		/* Открываем файл */
        FILE *file=fopen(da,"r"); 

		/* Если такого файла нету */
		if (!file) 
		{
			/* Нашли, выходим */
			break;
		}

		/* Закрываем файл, ищем следующий */
		fclose(file);
	}

	jpeg_compress_struct cinfo;

	jpeg_error_mgr jerr;

	FILE * outfile;

	JSAMPROW row_pointer[1];

	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(da, "wb")) == NULL)
	{
		return FALSE;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	/* image width and height, in pixels */
	cinfo.image_width = image_width; 

	cinfo.image_height = image_height;

	/* # of color components per pixel */
	cinfo.input_components = 3;

	/* colorspace of input image */
	cinfo.in_color_space = JCS_RGB; 
	
	/* 100% качество */
	int quality = 100;

	jpeg_set_defaults(&cinfo);

	/* TRUE means limit to baseline-JPEG values */
	jpeg_set_quality(&cinfo, quality, TRUE );

	jpeg_start_compress(&cinfo, TRUE);
	
	/* JSAMPLEs per row in image_buffer */
	row_stride = image_width * 3;	
	
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = & Data[cinfo.next_scanline * row_stride];

		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	
	jpeg_finish_compress(&cinfo);

	fclose(outfile);
	
	jpeg_destroy_compress(&cinfo);
	
	/* Освобождаем буффер */
	delete Data;
	
	/* Возращаем True, Всё прошло успешно */
	return TRUE;
}
#endif


/* Загружаем альфа канал из файла */
newResult newTexture::LoadAlphaFromFile(const newString &FileName)
{
	/* Текстура с альфой */
	newTexture alpha;
	
	/* Если такая уже есть, то берем из массива, иначе загружаем из файла */
	if (Texture.find(FileName)!=Texture.end())
	{
		alpha=Texture[FileName];
	}
	else
		if (alpha.LoadFromFile(FileName)!=NEW_OK)
		{
			return GenError(NEW_UNKNOW,"Ошибка при загрузке альфа канала");
		}

	if ( (Width!=alpha.Width) || (Height!=alpha.Height) )
	{
		MainConsole.Add(0, "Текстура %s не подходит по размеру для текстуры %s в качестве альфа-канала.",alpha.Name.c_str(), Name.c_str());

		return NEW_UNKNOW;
	}

	
	BYTE *src, *dst, *alp;

	newTexture base;

	UINT i;

	/* Переконвертируем в 32 бита, с поддержкой альфа канала */
	switch (Bpp)
	{
		case 24:

			/* Копируем текущую текстуру */
			base=*this;

			/* Очищаем */
			Data.clear();

			/* Создаём заново */
//+++ bring back!  16:21 18012018 			CreateEmpty4(Width,Height, newColor4ub(255,255,255,255) );

			src=&base.Data[0];

			dst=&Data[0];

			alp=&alpha.Data[0];

			/* Копируем пиксели */
			for (i=0;i<Width*Height;i++)
			{
				memcpy(dst,src,sizeof(newColor3ub));

				/* В зависимости от разрешения текстуры с альфой */
				switch (alpha.Bpp)
				{
					case 8:

						dst[3]=alp[0];

						alp++;

						break;

					case 24:

						dst[3]=(BYTE)(((UINT)alp[0]+(UINT)alp[1]+(UINT)alp[2])/3);

						alp+=3;

						break;

					case 32:

						dst[3]=alp[3];

						alp+=4;

						break;
				} /* switch (alpha.Bpp) */
				dst+=4;src+=3;

			} /* for (i=0;i<Width*Height;i++) */

			break; /* switch (Bpp) - case 24 */

		case 32:

			dst=&Data[0];

			alp=&alpha.Data[0];

			/* Копируем пиксели */
			for (i=0;i<Width*Height;i++)
			{
				/* В зависимости от разрешения текстуры с альфой */
				switch (alpha.Bpp)
				{
					case 8:

						dst[3]=alp[0];

						alp++;

						break;

					case 24:

						dst[3]=(BYTE)(((UINT)alp[0]+(UINT)alp[1]+(UINT)alp[2])/3);

						alp+=3;

						break;

					case 32:

						dst[3]=alp[3];

						alp+=4;

						break;
				} /* switch (alpha.Bpp) */
				dst+=4;

			} /* for (i=0;i<Width*Height;i++) */

			break; /* switch (Bpp) - case 32  */

	} /*  switch (Bpp) */
	return NEW_OK;
}

newResult newTexture::Upload()
{
	MainRender.TextureUpload(this);

	return NEW_OK;
}

void newTexture::Enable()
{
	MainRender.TextureEnable(this);
}
