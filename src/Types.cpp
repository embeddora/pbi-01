#include "Types.h"

/* ������� ���� � ������. ����� ����������� ��������� */
UINT newStrWordCount(const newString &src)
{
UINT n=0;

size_t pos=0;

	if (src[pos]==' ')
		
		pos=src.find_first_not_of(' ',pos);

	while (pos<src.length())
	{
		n++;

		pos=src.find_first_of(' ',pos);

		if (pos<src.length()) pos=src.find_first_not_of(' ',pos);
	}

	return n;
}

/* ����� n-��� ����� */
BOOL newGetStrWord(UINT n, const newString &src, newString &dst)
{
UINT k=0;

size_t pos=0,last=0;

	if ((src[pos]==' ')||(src[pos]=='\x9'))
		
		pos=dst.find_first_not_of(" \x9",pos);

	while (pos<src.length())
	{
		k++;

		last=pos;

		pos=src.find_first_of(' ',pos);

		if (k==n)
		{
			dst=src.substr(last,pos-last);

			return TRUE;
		}
		if (pos<src.length()) pos=src.find_first_not_of(' ',pos);
	}
	return FALSE;
}

/* ������ � ���� */
BYTE newStrToBYTE(const newString &value)
{
	return (BYTE)(atoi(value.c_str()));
}

/* ������ � int */
int newStrToInt(const newString &value)
{
	return (atoi(value.c_str()));
}

/* ������ � uint */
UINT newStrToUINT(const newString &value)
{
	return (UINT)(atoi(value.c_str()));
}

/* ������ � float */
float newStrToFloat(const newString &value)
{
	return (float)(atof(value.c_str()));
}

/* ������ ������ � ������� ������� */
newString &newStrMakeUp(newString &res,const newString &src)
{
	res=src;

	_strupr((char *)res.c_str());

	return res;
}

/* ������ ������ � ������� ������� */
void newStrMakeUp(newString &res)
{
	_strupr((char *)res.c_str());
}

/* ������� ��������� � ������ */
int newStrStr(const newString &sub,const newString &main)
{
char *pdest;

	size_t  result,len=main.size();

	pdest = strstr( main.c_str(), sub.c_str() );

	result = pdest - main.c_str();

	if ((result>=0)&&(result<len)) 

		return (int)result;

	else

		return -1;

}

/* ������ ������ �� ����� */
BOOL newReadStr(FILE *file,newString &res)
{
	res="";

	unsigned char h=fgetc(file);

	while (((h==9)||(h==' '))&&!feof(file))
		
		h=fgetc(file);

	while ((h!='\n')&&!feof(file))
	{
		res+=h;

		h=fgetc(file);

		if (h==9)
			
			h=' ';
	}
	if (feof(file))
		
		return FALSE;
	
	else
		
		return TRUE;
}

/* ��������� Int � newString */
newString newIntToStr(int i)
{
char buf[32];

	itoa(i,buf,10);

	return newString(buf);
}
