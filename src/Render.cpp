#include "Render.h"

#include "Console.h"

#include "Input.h"

#include "GLLight.h"

/* An interface to special substitute of certain parts OpenGL32 and GLu32 libraries */
#include "ExtGL/ExtGL.h"

/* Odd way to plug the OpenGL32  library in. (Acting this way we avoid doig it in  Project->Settings->...) */
#pragma comment( lib, "opengl32.lib" )

/* Odd way to plug the GLu32 library */
#pragma comment( lib, "glu32.lib" )

/* Save vsync state. Сохранение состояния vsync */
int SafeSwapInterval = 0;

/* The following are the functions from library Glu */

/* Compute ceiling of integer quotient of A divided by B: */
#define CEILING( A, B )  ( (A) % (B) == 0 ? (A)/(B) : (A)/(B)+1 )

/* Checking is N is the power of 'zwo'. Returns either this power either zero */
inline int IsPow2(int N)
{
int S = N;

	if (N > 2) 
	{
	int k = 0;

		while(N >>= 1)

			k++;

		return (1<<k)==S;
	}
	else
		return 0;
}


/* To work around optimizer bug in MSVC4.1 */
#if defined(__WIN32__) && !defined(OPENSTEP)

	void dummy(GLuint j, GLuint k) { }

#else

	#define dummy(J, K)

#endif


round2(GLint n)
{
GLint m;
	
	for (m = 1; m < n; m *= 2);
	
	/* m>=n */
	if (m - n <= n - m / 2)
	{
		return m;
	}
	else
	{
		return m / 2;
	}
}

static GLint bytes_per_pixel(GLenum format, GLenum type)
{
	GLint n, m;
	
	switch (format)
	{
		case GL_COLOR_INDEX:

		case GL_STENCIL_INDEX:

		case GL_DEPTH_COMPONENT:

		case GL_RED:

		case GL_GREEN:

		case GL_BLUE:

		case GL_ALPHA:

		case GL_LUMINANCE:

			n = 1;

			break;

		case GL_LUMINANCE_ALPHA:

			n = 2;

			break;

		case GL_RGB:

		case GL_BGR:

			n = 3;

			break;

		case GL_RGBA:

		case GL_BGRA:

#ifdef GL_EXT_abgr

	case GL_ABGR_EXT:

#endif
			n = 4;

			break;

		default:

			n = 0;
	}
	
	switch (type)
	{
		case GL_UNSIGNED_BYTE:

			m = sizeof(GLubyte);

			break;

	case GL_BYTE:

			m = sizeof(GLbyte);

			break;

	case GL_BITMAP:

			m = 1;

			break;

	case GL_UNSIGNED_SHORT:

			m = sizeof(GLushort);

			break;

	case GL_SHORT:

			m = sizeof(GLshort);

			break;

	case GL_UNSIGNED_INT:

			m = sizeof(GLuint);

			break;

	case GL_INT:

		m = sizeof(GLint);

		break;

	case GL_FLOAT:

		m = sizeof(GLfloat);

		break;

	default:

		m = 0;
	}
	
	return n * m;
}

GLint gluScaleImage(GLenum format, GLsizei widthin, GLsizei heightin, GLenum typein, const void *datain,  GLsizei widthout, GLsizei heightout,   GLenum typeout, void *dataout)
{
GLint components, i, j, k;

GLfloat *tempin, *tempout;

GLfloat sx, sy;

GLint unpackrowlength, unpackalignment, unpackskiprows, unpackskippixels;

GLint packrowlength, packalignment, packskiprows, packskippixels;

GLint sizein, sizeout;

GLint rowstride, rowlen;


   /* Determine number of components per pixel */
   switch (format)
   {
	   case GL_COLOR_INDEX:

	   case GL_STENCIL_INDEX:

	   case GL_DEPTH_COMPONENT:

	   case GL_RED:

	   case GL_GREEN:

	   case GL_BLUE:

	   case GL_ALPHA:

	   case GL_LUMINANCE:

		  components = 1;

		  break;

	   case GL_LUMINANCE_ALPHA:

		  components = 2;

		  break;

	   case GL_RGB:

	   case GL_BGR:

		  components = 3;

		  break;

	   case GL_RGBA:

	   case GL_BGRA:

#ifdef GL_EXT_abgr

		case GL_ABGR_EXT:

#endif /* (GL_EXT_abgr) */

		  components = 4;

		  break;

	   default:

		  return NEW_INVALID_ENUM;
   }

   /* Determine bytes per input datum */
   switch (typein)
   {
		case GL_UNSIGNED_BYTE:

			sizein = sizeof(GLubyte);

			break;

		case GL_BYTE:

			sizein = sizeof(GLbyte);

			break;

		case GL_UNSIGNED_SHORT:

			sizein = sizeof(GLushort);

			break;

		case GL_SHORT:

			sizein = sizeof(GLshort);

			break;

		case GL_UNSIGNED_INT:

			sizein = sizeof(GLuint);

			break;

		case GL_INT:

			sizein = sizeof(GLint);

			break;

		case GL_FLOAT:

			sizein = sizeof(GLfloat);

			break;

		case GL_BITMAP:

		/* not implemented yet */
		default:

			return GL_INVALID_ENUM;
   }

	/* Determine bytes per output datum */
	switch (typeout)
	{
		case GL_UNSIGNED_BYTE:

			sizeout = sizeof(GLubyte);

			break;

		case GL_BYTE:

			sizeout = sizeof(GLbyte);

			break;

		case GL_UNSIGNED_SHORT:

			sizeout = sizeof(GLushort);

			break;

		case GL_SHORT:

			sizeout = sizeof(GLshort);

			break;

		case GL_UNSIGNED_INT:

			sizeout = sizeof(GLuint);

			break;

		case GL_INT:

			sizeout = sizeof(GLint);

			break;

		case GL_FLOAT:

			sizeout = sizeof(GLfloat);

			break;

		case GL_BITMAP:

		/* not implemented yet */
		default:

			return GL_INVALID_ENUM;
	}

	/* Get glPixelStore state */
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpackrowlength);

	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackalignment);

	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &unpackskiprows);

	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpackskippixels);

	glGetIntegerv(GL_PACK_ROW_LENGTH, &packrowlength);

	glGetIntegerv(GL_PACK_ALIGNMENT, &packalignment);

	glGetIntegerv(GL_PACK_SKIP_ROWS, &packskiprows);

	glGetIntegerv(GL_PACK_SKIP_PIXELS, &packskippixels);

	/* Allocate storage for intermediate images */
	tempin = (GLfloat *) malloc(widthin * heightin * components * sizeof(GLfloat));

	if (!tempin)
	{
	  return NEW_OUT_OF_MEMORY;
	}

	tempout = (GLfloat *) malloc(widthout * heightout * components * sizeof(GLfloat));

	if (!tempout)
	{
	  free(tempin);

	  return NEW_OUT_OF_MEMORY;
	}

	/* Unpack the pixel data and convert to floating point */
	if (unpackrowlength > 0)
	{
	  rowlen = unpackrowlength;
	}
	else
	{
	  rowlen = widthin;
	}

	if (sizein >= unpackalignment)
	{
	  rowstride = components * rowlen;
	}
	else
	{
	  rowstride = unpackalignment / sizein * CEILING(components * rowlen * sizein, unpackalignment);
	}

	switch (typein)
	{
		case GL_UNSIGNED_BYTE:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLubyte *ubptr = (GLubyte *) datain	+ i * rowstride	+ unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * ubptr++;
				}
			}
			break;

		case GL_BYTE:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
				GLbyte *bptr = (GLbyte *) datain + i * rowstride + unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * bptr++;
				}
			}
			break;

		case GL_UNSIGNED_SHORT:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLushort *usptr = (GLushort *) datain + i * rowstride + unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * usptr++;
				}
			}
			break;

		case GL_SHORT:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLshort *sptr = (GLshort *) datain	+ i * rowstride	+ unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * sptr++;
				}
			}
			break;

		case GL_UNSIGNED_INT:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLuint *uiptr = (GLuint *) datain	+ i * rowstride	+ unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * uiptr++;
				}
			}
			break;

		case GL_INT:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLint *iptr = (GLint *) datain	+ i * rowstride	+ unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = (GLfloat) * iptr++;
				}

			}
			break;

		case GL_FLOAT:

			k = 0;

			for (i = 0; i < heightin; i++)
			{
			GLfloat *fptr = (GLfloat *) datain + i * rowstride	+ unpackskiprows * rowstride + unpackskippixels * components;

				for (j = 0; j < widthin * components; j++)
				{
					dummy(j, k);

					tempin[k++] = *fptr++;
				}
			}
			break;

		default:

			return NEW_INVALID_ENUM;
	}


	/* Scale the image! */
	if (widthout > 1)

		sx = (GLfloat) (widthin - 1) / (GLfloat) (widthout - 1);

	else

		sx = (GLfloat) (widthin - 1);

	if (heightout > 1)

		sy = (GLfloat) (heightin - 1) / (GLfloat) (heightout - 1);

	else

		sy = (GLfloat) (heightin - 1);

/* #define POINT_SAMPLE */
#ifdef POINT_SAMPLE

	for (i = 0; i < heightout; i++)
	{
		GLint ii = i * sy;

		for (j = 0; j < widthout; j++)
		{
			GLint jj = j * sx;

			GLfloat *src = tempin + (ii * widthin + jj) * components;

			GLfloat *dst = tempout + (i * widthout + j) * components;

			for (k = 0; k < components; k++)
			{
				*dst++ = *src++;
			}
		}
	}
#else
	if (sx < 1.0 && sy < 1.0)
	{
		/* magnify both width and height:  use weighted sample of 4 pixels */
		GLint i0, i1, j0, j1;

		GLfloat alpha, beta;

		GLfloat *src00, *src01, *src10, *src11;

		GLfloat s1, s2;

		GLfloat *dst;

		for (i = 0; i < heightout; i++)
		{
			i0 = i * sy;

			i1 = i0 + 1;

			if (i1 >= heightin)

				i1 = heightin - 1;

				/*	 i1 = (i+1) * sy - EPSILON;*/
				alpha = i * sy - i0;

			for (j = 0; j < widthout; j++)
			{
				j0 = j * sx;

				j1 = j0 + 1;

				if (j1 >= widthin)

					j1 = widthin - 1;

				/*	    j1 = (j+1) * sx - EPSILON; */
				beta = j * sx - j0;

				/* compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
				src00 = tempin + (i0 * widthin + j0) * components;

				src01 = tempin + (i0 * widthin + j1) * components;

				src10 = tempin + (i1 * widthin + j0) * components;

				src11 = tempin + (i1 * widthin + j1) * components;

				dst = tempout + (i * widthout + j) * components;

				for (k = 0; k < components; k++)
				{
					s1 = *src00++ * (1.0 - beta) + *src01++ * beta;

					s2 = *src10++ * (1.0 - beta) + *src11++ * beta;

					*dst++ = s1 * (1.0 - alpha) + s2 * alpha;
				}
			}
		}
	}
	else
	{
		/* shrink width and/or height:  use an unweighted box filter */
		GLint i0, i1;

		GLint j0, j1;

		GLint ii, jj;

		GLfloat sum, *dst;

		for (i = 0; i < heightout; i++)
		{
			i0 = i * sy;

			i1 = i0 + 1;

			if (i1 >= heightin)

				i1 = heightin - 1;
			
			/*	 i1 = (i+1) * sy - EPSILON; */
			for (j = 0; j < widthout; j++)
			{
				j0 = j * sx;

				j1 = j0 + 1;

				if (j1 >= widthin)

					j1 = widthin - 1;
				
				/*	    j1 = (j+1) * sx - EPSILON; */

				dst = tempout + (i * widthout + j) * components;

				/* compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
				for (k = 0; k < components; k++)
				{
					sum = 0.0;

					for (ii = i0; ii <= i1; ii++)
					{
						for (jj = j0; jj <= j1; jj++)
						{
							sum += *(tempin + (ii * widthin + jj) * components + k);
						}
					}

					sum /= (j1 - j0 + 1) * (i1 - i0 + 1);

					*dst++ = sum;
				}
			}
		}
	}
#endif /* (POINT_SAMPLE) */


	/* Return output image */
	if (packrowlength > 0)
	{
		rowlen = packrowlength;
	}
	else
	{
		rowlen = widthout;
	}

	if (sizeout >= packalignment)
	{
		rowstride = components * rowlen;
	}
	else
	{
		rowstride = packalignment / sizeout	* CEILING(components * rowlen * sizeout, packalignment);
	}

	switch (typeout)
	{
		case GL_UNSIGNED_BYTE:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLubyte *ubptr = (GLubyte *) dataout + i * rowstride + packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*ubptr++ = (GLubyte) tempout[k++];
				}
			}
			break;

		case GL_BYTE:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLbyte *bptr = (GLbyte *) dataout	+ i * rowstride		+ packskiprows * rowstride + packskippixels * components;
				
				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*bptr++ = (GLbyte) tempout[k++];
				}
			}
			break;

		case GL_UNSIGNED_SHORT:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLushort *usptr = (GLushort *) dataout	+ i * rowstride	+ packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*usptr++ = (GLushort) tempout[k++];
				}
			}
			break;

		case GL_SHORT:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLshort *sptr = (GLshort *) dataout	+ i * rowstride	+ packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*sptr++ = (GLshort) tempout[k++];
				}
			}
			break;

		case GL_UNSIGNED_INT:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLuint *uiptr = (GLuint *) dataout	+ i * rowstride	+ packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*uiptr++ = (GLuint) tempout[k++];
				}
			}
			break;

		case GL_INT:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLint *iptr = (GLint *) dataout	+ i * rowstride	+ packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*iptr++ = (GLint) tempout[k++];
				}
			}
			break;

		case GL_FLOAT:

			k = 0;

			for (i = 0; i < heightout; i++)
			{
				GLfloat *fptr = (GLfloat *) dataout	+ i * rowstride	+ packskiprows * rowstride + packskippixels * components;

				for (j = 0; j < widthout * components; j++)
				{
					dummy(j, k + i);

					*fptr++ = tempout[k++];
				}
			}
			break;

		default:

			return NEW_INVALID_ENUM;
	}


   /* free temporary image storage */
   free(tempin);

   free(tempout);

   return 0;
} /* gluScaleImage(...) */



GLint gluBuild2DMipmaps(GLenum target, GLint components,  GLsizei width, GLsizei height, GLenum format,	  GLenum type, const void *data)
{
GLint w, h, maxsize;

void *image, *newimage;

GLint neww, newh, level, bpp;

int error;

GLboolean done;

GLint retval = 0;

GLint unpackrowlength, unpackalignment, unpackskiprows, unpackskippixels;

GLint packrowlength, packalignment, packskiprows, packskippixels;

	if (width < 1 || height < 1)

		return NEW_INVALIDE_VALUE;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxsize);

	w = round2(width);

	if (w > maxsize)
	{
		w = maxsize;
	}

	h = round2(height);

	if (h > maxsize)
	{
		h = maxsize;
	}

	bpp = bytes_per_pixel(format, type);

	if (bpp == 0)
	{
		/* probably a bad format or type enum */
		return NEW_INVALID_ENUM;
	}

	/* Get current glPixelStore values */
	glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpackrowlength);

	glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackalignment);

	glGetIntegerv(GL_UNPACK_SKIP_ROWS, &unpackskiprows);

	glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpackskippixels);

	glGetIntegerv(GL_PACK_ROW_LENGTH, &packrowlength);

	glGetIntegerv(GL_PACK_ALIGNMENT, &packalignment);

	glGetIntegerv(GL_PACK_SKIP_ROWS, &packskiprows);

	glGetIntegerv(GL_PACK_SKIP_PIXELS, &packskippixels);

	/* set pixel packing */
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glPixelStorei(GL_PACK_SKIP_ROWS, 0);

	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	done = GL_FALSE;

	if (w != width || h != height)
	{
		/* must rescale image to get "top" mipmap texture image */
		image = malloc((w + 4) * h * bpp);

		if (!image)
		{
			return NEW_OUT_OF_MEMORY;
		}

		error = gluScaleImage(format, width, height, type, data, w, h, type, image);

		if (error)
		{
			retval = error;

			done = GL_TRUE;
		}
	}
	else
	{
		image = (void *) data;
	}


	level = 0;

	while (!done)
	{
		if (image != data)
		{
			/* set pixel unpacking */
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		}

		glTexImage2D(target, level, components, w, h, 0, format, type, image);

		if (w == 1 && h == 1)

			break;

		neww = (w < 2) ? 1 : w / 2;

		newh = (h < 2) ? 1 : h / 2;

		newimage = malloc((neww + 4) * newh * bpp);

		if (!newimage)
		{
			return NEW_OUT_OF_MEMORY;
		}

		error = gluScaleImage(format, w, h, type, image, neww, newh, type, newimage);

		if (error)
		{
			retval = error;

			done = GL_TRUE;
		}

		if (image != data)
		{
			free(image);
		}

		image = newimage;

		w = neww;

		h = newh;

		level++;
	}

	if (image != data)
	{
		free(image);
	}

   /* Restore original glPixelStore state */
   glPixelStorei(GL_UNPACK_ROW_LENGTH, unpackrowlength);

   glPixelStorei(GL_UNPACK_ALIGNMENT, unpackalignment);

   glPixelStorei(GL_UNPACK_SKIP_ROWS, unpackskiprows);

   glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpackskippixels);

   glPixelStorei(GL_PACK_ROW_LENGTH, packrowlength);

   glPixelStorei(GL_PACK_ALIGNMENT, packalignment);

   glPixelStorei(GL_PACK_SKIP_ROWS, packskiprows);

   glPixelStorei(GL_PACK_SKIP_PIXELS, packskippixels);

   return retval;
}

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xmin, xmax, ymin, ymax;
	
	ymax = zNear * tan(fovy * NEW_PI / 360.0);

	ymin = -ymax;

	xmin = ymin * aspect;

	xmax = ymax * aspect;
	
	/* don't call glFrustum() because of error semantics (covglu) */
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}


/* Глобальная переменная */
newRender MainRender;

newRender::newRender(void)
{
	Frame_Num=0;

	ISfps=FALSE;

	CurrentRenderMode=NEW_NONE;

	CurTexID=0;

	CurrentCamera=NULL;
}

newRender::~newRender(void)
{
}

/* Для печати на экран */
char buf[1000];

GL_Window *safewindow;

/* Устанавливаем OpenGL */
BOOL newRender::SetupOpenGL(GL_Window *window)
{
	safewindow=window;

	/* pfd указывает Windows как устанавливать OpenGL */
	PIXELFORMATDESCRIPTOR pfd =
	{
		/* Размер этого Pixel Format Descriptor'а */
		sizeof (PIXELFORMATDESCRIPTOR),
		/* Номер версии */
		1,
		/* Формат должен поддерживать отрисовку в окно */
		PFD_DRAW_TO_WINDOW | 
		/* Формат должен поддерживать OpenGL */
		PFD_SUPPORT_OPENGL |
		/* Формат должен поддерживать Double Buffering */
		PFD_DOUBLEBUFFER,
		/* Формат должен поддерживать RGBA режим */
		PFD_TYPE_RGBA,
		/* Выбираем глубину цвета */
		window->init.bitsPerPixel,	
		/* Игнорируем биты цвета */
		0, 0, 0, 0, 0, 0,
		/* Нет альфа буфера (пока нету) */
		0,
		/* Игнорируем бит сдвига */
		0,
		/* Нету Accumulation Buffer'а */
		0,	
		/* Accumulation биты игнорируем */
		0, 0, 0, 0,
		/* Глубина Z-Buffer'а (Depth Buffer) */
		MainSettings.DEPTH,
		/* Нету Stencil Buffer'а (пока нету) */
		0,	
		/* Нету Auxiliary Buffer'а */
		0,	
		/* Главный слой для отрисовки */
		PFD_MAIN_PLANE,	
		/* Зарезирвировано */
		0,	
		/* Игнорируем маску слоёв */
		0, 0, 0	
	};

	/* Содержит выбранный формат */
	GLuint PixelFormat;	

	/* Ищем совместимый Pixel Format */
	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);

	/* Мы нашли? */
	if (0 == PixelFormat)	
	{
		/* Освобождаем контекст устройсва */
		ReleaseDC (window->hWnd, window->hDC);
		
		/* Обнуляем его */
		window->hDC = 0;

		/* Убиваем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем хэндл окна */
		window->hWnd = 0;

		/* Возвращаем False (Ложь/Неудача) */
		return FALSE;
	}

	/* Пытаемся установить этот формат */
	if (FALSE == SetPixelFormat (window->hDC, PixelFormat, &pfd))
	{
		/* Jesli nеудачно,  to Освобождаем контекст устройсва */
		ReleaseDC (window->hWnd, window->hDC);

		/* Обнуляем его */
		window->hDC = 0;

		/* Убиваем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем хэндл окна */
		window->hWnd = 0;

		/* Возвращаем False (Ложь) */
		return FALSE;
	}

	/* Пробуем получить контекст рендера */
	window->hRC = wglCreateContext (window->hDC);

	/* Мы его получили? */
	if (0 == window->hRC)
	{
		/* Jesli nеудачно, Освобождаем контекст устройсва */
		ReleaseDC (window->hWnd, window->hDC);

		/* Обнуляем его */
		window->hDC = 0;

		/* Убиваем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем хэндл окна */
		window->hWnd = 0;

		/* Возвращаем False (Ложь) */
		return FALSE;
	}

	/* Делаем этот контект рендера текущим */
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		/* Jesli nеудачно, Удаляем контекст рендера */
		wglDeleteContext (window->hRC);

		/* Обнуляем его */
		window->hRC = 0;

		/* Освобождаем контекст устройсва */
		ReleaseDC (window->hWnd, window->hDC);

		/* Обнуляем его */
		window->hDC = 0;

		/* Убиваем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем хэндл окна */
		window->hWnd = 0;

		/* Возвращаем False (Ложь) */
		return FALSE;
	}

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Инициализация OpenGL прошла успешно.");

	VendorName=(char *)glGetString(GL_VENDOR);

	MainConsole.Add(0,"Производитель видео карты: %s",VendorName.c_str());

	RendererName=(char *)glGetString(GL_RENDERER);

	MainConsole.Add(0,"Тип видео карты (рендерер): %s",RendererName.c_str());

	VersionName=(char *)glGetString(GL_VERSION);

	MainConsole.Add(0,"Версия OpenGL: %s",VersionName.c_str());

	extgl_Initialize();

	Extensions=(char *)glGetString(GL_EXTENSIONS);

	if (wglGetExtensionsStringARB)//+++ added by mazukevich
	{
		Extensions += (char *)wglGetExtensionsStringARB(window->hDC);

		MainConsole.Add(0,"Расширения OpenGL, поддерживаемые рендерером: %s",Extensions.c_str());
	}

	if (wglGetSwapIntervalEXT)//+++ added by mazukevich
	{
		SafeSwapInterval = wglGetSwapIntervalEXT();

		wglSwapIntervalEXT(0);
	}
	
	MainConsole.Add(0,"Отключение vsync.");

	return TRUE;
}

/* Уничтожение OpenGL окна, Уничтожаем OpenGL окно и освобождаем ресурсы */
BOOL newRender::DestroyWindowGL (GL_Window* window)	
{
	if (wglGetSwapIntervalEXT) 

		/* Востанавливаем Vsync */
		wglSwapIntervalEXT(SafeSwapInterval);

	/* Окно имеет ресурсы? */
	if (0 != window->hWnd )
	{	
		/* Окно имеет контекст устройства? */
		if (0 != window->hDC)
		{
			/* Ставим текущий активны рендер контекст в ноль */
			wglMakeCurrent (window->hDC, 0);

			/* Окно имеет рендер контекст? */
			if (window->hRC != 0)
			{
				/* Осбобождаем рендер контекст */
				wglDeleteContext (window->hRC);

				/* Обнуляем рендер контекст */
				window->hRC = 0;
			}

			/* Освобождаем контекст устойства */
			ReleaseDC (window->hWnd, window->hDC);

			/* Обнуляем контекст устойства */
			window->hDC = 0;
		}

		/* Вывод в консоль сообщения */
		MainConsole.Add(0,"Удаление контекста OpenGL.");

		/* Уничтожаем окно */
		DestroyWindow (window->hWnd);

		/* Обнуляем дискриптор окна */
		window->hWnd = 0;
	}

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Успешно удалено окно.");

	/* Полноэкран? */
	if (window->init.isFullScreen)
	{
		/* Востанавливаем разрешение экрана */
		ChangeDisplaySettings (NULL,0);

		/* Включаем курсор мыши */
		ShowCursor (TRUE);
	}	

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Востанавливаем разрешение экрана.");

	/* Возращаем True */
	return TRUE;
}

void newRender::Reshape(int width, int height)
{
	RenderMode(NEW_RENDERMODE_3D);

	/* Переустанавливаем ViewPort (область видимости) */
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));

	MainInput.MouseCenter.Set((float)width/2,(float)height/2);

	/* Выбираем матрицу проекции */
	glMatrixMode (GL_PROJECTION);

	/* Сбрасываем её на единичную */
	glLoadIdentity ();

	if (CurrentCamera)

		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (CurrentCamera->Frustum.GetFoV()*(GLfloat)(height)/(GLfloat)(width)/2, (GLfloat)(width)/(GLfloat)(height), CurrentCamera->Frustum.GetzNear(), CurrentCamera->Frustum.GetzFar());	

	else
		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width)/2, (GLfloat)(width)/(GLfloat)(height),	MainSettings.ZNEAR, MainSettings.ZFAR);		
	
	/* Выбираем видовую матрицу */
	glMatrixMode (GL_MODELVIEW);

	/* Сбрасываем её на единичную */
	glLoadIdentity ();
}


#if (1) /* it's original stuff - from original STEP8 */

/* Закачиваем текстуру в видео-память */
void newRender::TextureUpload(newTexture *tex)
{
	/* Строим текстуру из данных */
	GLuint type,components;

	/* В зависимости от бит на пиксель */
	switch (tex->Bpp)
	{
		case 8:

			if (tex->Type==NEW_TEXTURE_FONT)

				type=GL_ALPHA8;
			else
				type=GL_ALPHA8;

			components=1;

			break;

		case 24:

			type=(tex->isBRGA)?GL_BGR:GL_RGB;components=3; break;

		case 32:

			type=(tex->isBRGA)?GL_BGRA:GL_RGBA;components=4; break;
	};

	/* Генерируем индекс OpenGL текстуры */
	glGenTextures(1, (GLuint *)&tex->TextureID);

	/* Биндим индекс на текстуру */
	glBindTexture(GL_TEXTURE_2D, tex->TextureID);

	/* Тип границ текстуры */
	GLuint kak;

	/* Выбираем тип границ */
	switch (tex->Border_Type)
	{
		/* Текстура не повторяемся */
		case NEW_TEXTURE_BORDER_CLAMP:

			kak=GL_CLAMP;

			break;

		/* Текстура повторяется  */
		case NEW_TEXTURE_BORDER_REPEAT:

			kak=GL_REPEAT;
			
			break;
	}

	/* Устанавливаем параметры границ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	/* Устанавливаем фильтрацию текстуры: 10 - самая лучшая, но медленная, ...,  0 - самая плохая, но быстрая */
	
	/* Заметки: 10-й фильтр не самый лучший и в дальнейшем фильтры будут добавляться */
	GLenum MagMinFilter[][2] =
	{
		{ GL_NEAREST,GL_NEAREST},
		{ GL_LINEAR,GL_NEAREST},
		{ GL_NEAREST,GL_LINEAR},
		{ GL_LINEAR,GL_LINEAR},
		{ GL_NEAREST,GL_NEAREST_MIPMAP_NEAREST},
		{ GL_NEAREST,GL_LINEAR_MIPMAP_NEAREST},
		{ GL_NEAREST,GL_NEAREST_MIPMAP_LINEAR},
		{ GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST},
		{ GL_LINEAR,GL_LINEAR_MIPMAP_NEAREST},
		{ GL_LINEAR,GL_NEAREST_MIPMAP_LINEAR},
		{ GL_LINEAR,GL_LINEAR_MIPMAP_LINEAR}
	};

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagMinFilter[tex->Filter][0]);	

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MagMinFilter[tex->Filter][1]);

	/* Если поддерживается автоматическая генерация мипмапов, то генерируем их на видеокарте */
	if (tex->GenerateMipMap)

//+++ this string is commented out by mazukevich:  if ((extgl_Extensions.OpenGL14)||(extgl_Extensions.SGIS_generate_mipmap))

			if (IsPow2(tex->Width) && IsPow2(tex->Height))
			{
				/* Ставим генерацию мипмэпов */
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

				/* Параноидальная проверка для старых видео карт. (конкретно Ati Rage Fury Pro и т.д.) */
				int genmip = 0;

				glGetTexParameteriv(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, &genmip);

				/* Если вдруг генерация невозможна, то генерируем в софте */
				if (!genmip)

				{
					if (tex->Filter<4)

						glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

					else

						gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

				}
				else /* Иначе, всё ок. Видео карта сгенерирует мипмэпы */
				{
					glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

					glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

					/* Автоматическая генерация прошла успешно */
					tex->GenerateMipMap = 3;
				}
			}
			else /* Иначе как обычно */

				if (tex->GenerateMipMap)

					gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

				else

					glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);
			

	TextureDisable();
}
#else /* the following is the stuff ingerited from STEP7 */

/* Закачиваем текстуру в видео-память */
void newRender::TextureUpload(newTexture *tex)
{
	/* Строим текстуру из данных */
	GLuint type,components;

	/* В зависимости от бит на пиксель */
	switch (tex->Bpp)
	{
		/* 8 bits per pixel */
		case 8:

			if ( NEW_TEXTURE_FONT == tex->Type)

				type=GL_ALPHA8;
			else
				type=GL_ALPHA8;

			components=1;

			break;

		/* 24 bits per pixel */
		case 24:

			type=GL_RGB;
			
			components=3;
			
			break;

		/* 32 bits per pixel */
		case 32:

			type=GL_RGBA;
			
			components=4;

			break;
	};

	/* Generating index of OpenGL textgure; Генерируем индекс OpenGL текстуры */
	glGenTextures(1, (GLuint *)&tex->TextureID);

	/* Bind this index to texture; Биндим индекс на текстуру */
	glBindTexture(GL_TEXTURE_2D, tex->TextureID);

	/* Type of texture borders; Тип границ текстуры */
	GLuint kak;

	/* Borders type selection; Выбираем тип границ */
	switch (tex->Border_Type)
	{
		/* Texture doesn't repet; Текстура не повторяемся */
		case NEW_TEXTURE_BORDER_CLAMP:

			kak = GL_CLAMP;
			
			break;

		/* Texture repeats; Текстура повторяется */
		case NEW_TEXTURE_BORDER_REPEAT:

			kak=GL_REPEAT;
			
			break;
	}

	/* Setting border parameters; Устанавливаем параметры границ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);

	/* Setting border parameters; Устанавливаем параметры границ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	/* Setting texture filtering (10 - best but slowest, ... , 0 - worst but fastest) */
	switch (tex->Filter)
	{
	case 10:
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 9 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 8 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 7 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 6 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 5 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 4 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	

			gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 3 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

			glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 2 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	

			glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 1 :
		{	
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	

			glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	case 0 :
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	

			glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

			break;
		}
	}

	TextureDisable();
}
#endif /* (0) */


/* Enabling the texure. Включает текстуру */
void newRender::TextureEnable(newTexture *tex)
{
	if (NULL == tex)

		return;

	if (CurTexID != tex->TextureID)
	{
		glBindTexture(GL_TEXTURE_2D, tex->TextureID);

		CurTexID=tex->TextureID;
	}
}

/* Disabling texture. Выключает текстуру */
void newRender::TextureDisable()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	CurTexID=0;
}

/* Способ смешивания текстуры */
void newRender::TextureOperation(newEnum op)
{	
	switch (op)
	{
		case NEW_TEXTURE_COMBINE_MODULATE:
		{
			/* Modulate		- умножение */
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			break;
		}

		default: 

			/* cases, to be added: Replace	- замена, Add - добавление, Interpolate	- Интерполирования */
			break;
	}
}


newGLRender::newGLRender(void)
{
}

newGLRender::~newGLRender(void)
{
}

/* Сбрасывает экран в буффер */
void newGetScreen(BYTE *Data)
{
	/* Выбираем буффер экрана */
	glReadBuffer(GL_FRONT);

	/* Под Data уже выделена память. Читаем в неё весь экран */
	glReadPixels(0,0,MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,Data);
}

/* Создаём шрифт */
BOOL newRender::CreateFont(newFont *font)
{
/* Содержит X координату буквы */
float	cx;

/* Содержит Y координату буквы */
float	cy;

/* Ширина буквы в пикселях */
float   cwx;

/* Высота буквы в пикселях */
float   cwy;

	/* Вычисляем ширину По умолчанию квадрат: 16х16 */
	cwx         = (1.0f / 256.0f) * 16;

	/* Вычисляем высоту По умолчанию квадрат: 16х16 */
	cwy         = (1.0f / 256.0f) * 16;

	/* Создаём 256 дисплей листов */
	font->Base=glGenLists(256);

	/* Циклимся по всем листам */
	for (int loop=0; loop<256; loop++)
	{
		/* X и Y позиция */
		cx=float(loop%16)/16.0f;

		cy=float(loop/16)/16.0f;

		/* Начинаем заполнять лист  */
		glNewList(font->Base+loop,GL_COMPILE);

		/* Рисуем букву четырехугольником. Nachalo. */
		glBegin(GL_QUADS);
		{
			glTexCoord2f(cx+(font->FTrim[loop][0])/256.0f,cy+cwy);			

			glVertex2i(0,16);

			glTexCoord2f(cx+(font->FTrim[loop][0]+font->FTrim[loop][1])/256.0f,cy+cwy);

			glVertex2i(font->FTrim[loop][1],16);

			glTexCoord2f(cx+(font->FTrim[loop][0]+font->FTrim[loop][1])/256.0f,cy);

			glVertex2i(font->FTrim[loop][1],0);

			glTexCoord2f(cx+(font->FTrim[loop][0])/256.0f,cy);

			glVertex2i(0,0);
		}//+++ ATTENTION: this parenthesis is added by mazukevich. Probably later gonna remove it, but for now - let it stay here 

		/* Закончили рисовать */
		glEnd();

		/* Перемещаемся на другую позицию */
		glTranslatef((GLfloat)(font->FTrim[loop][1]+font->Spacing),0,0);
			
		/* Закончили лист */
		glEndList();

	}/*Цикл по буквам */
	
	return TRUE;
}

/* Убиваем шрифт */
void newRender::KillFont(newFont *font)
{
	/* Убиваем все буквы (листы) */
	glDeleteLists(font->Base, 256);
}

/* Функция вывода текста в двухмерном режиме; x,y - позиция; Text - по аналогии с printf */
void newPrint2D(float x,float y,const char *Text,...)
{
	/* Ссылка на список аргументов */
	va_list		va;

	/* Если нету текста */
	if("" == Text)

		/* Ничего не делаем */
		return;

	/* Копируем текст в buf, заменяя переменные их значениями */
	va_start(va, Text);

	vsprintf(buf, Text, va);

	va_end(va);

	/* Устанавливаем цвет */
	glColor4ubv(&MainRender.CurFont->Color.Color[0]);

	/* Включаем текстуру */
	MainRender.TextureEnable(MainRender.CurFont->TextureLink);

	/* Проверяем наличие 2D режима */
	MainRender.RenderMode(NEW_RENDERMODE_2D);

	/* Сохраняем видовую матрицу */
	glPushMatrix();

	/* Делаем её едениной */
	glLoadIdentity();

	/* Увеличиваем, в зависимости от ширины и высоты букв */
	glScalef(MainRender.CurFont->Width,MainRender.CurFont->Height, 1);

	/* Перемещаем на x и y */
	glTranslated(x/MainRender.CurFont->Width,y/MainRender.CurFont->Height,0);

	/* Смещаем базу листов на текущую */
	glListBase(MainRender.CurFont->Base);

	/* Рисуем весь текст вызовом соответствующих листов */
	glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);

	/* Востанавливаем видовую матрицу */
	glPopMatrix();


}

void newRender::RenderMode(newEnum mode)
{
	switch (mode)
	{
		/* NEW_RENDERMODE_3D - Техмерный режим, обычное состояние */
		case NEW_RENDERMODE_3D:

			/* Проверяем текущий режим */
			if (CurrentRenderMode!=NEW_RENDERMODE_3D)
			{
				/* Востановить предыдущий */
				RenderMode(NEW_RENDERMODE_RESTORE_LAST);

				/* Если был другой, то сменить */
				CurrentRenderMode=NEW_RENDERMODE_3D;
			}
			break;

		/* NEW_RENDERMODE_2D - Двухмерный режим, текст, спрайты и т.д. */
		case NEW_RENDERMODE_2D:

			/* Проверяем текущий режим */
			if (CurrentRenderMode!=NEW_RENDERMODE_2D)
			{
				/* Востановить предыдущий */
				RenderMode(NEW_RENDERMODE_RESTORE_LAST);

				/* Сохраняем атрибуты */
				glPushAttrib(GL_BLEND | GL_DRAW_BUFFER | GL_COLOR_BUFFER_BIT |
					GL_LOGIC_OP | GL_DEPTH_BUFFER_BIT |
					GL_DEPTH_TEST | GL_ENABLE_BIT | GL_LIGHTING_BIT | 
					GL_STENCIL_BUFFER_BIT );

				/* Сохраняем матрицу проекции */
				glMatrixMode(GL_PROJECTION);

				glPushMatrix();

				/* Делаем её единичной */
				glLoadIdentity();

				/* Устанвливаем 2D режим, в зависимости от заданных размеров */
				glOrtho(0,FontDisplayWidth,FontDisplayHeight,0,-100,100);

				/* Сохраняем видовую матрицу */
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();

				/* Делаем её еденичной */
				glLoadIdentity();

				/* Отключаем тест глубины */
				glDisable(GL_DEPTH_TEST);

				/* Включаем прозрачность */
				glEnable(GL_BLEND);

				/* Операция прозрачности */
				glBlendFunc(GL_ONE,GL_ONE);

				/* Запрещаем запись в  буффер глубины */
				glDepthMask(FALSE);

				/* Выключаем свет */
				glDisable(GL_LIGHTING);

				/* Устанавливаем текущий режим */
				CurrentRenderMode=NEW_RENDERMODE_2D;
			}

			break;

		/* NEW_RENDERMODE_RESTORE_LAST - Востанавливает предыдущий режим */
		case NEW_RENDERMODE_RESTORE_LAST:

			/* Если режим 2D */
			if (CurrentRenderMode==NEW_RENDERMODE_2D)
			{
				/* Востанавливаем матрицу проекции */
				glMatrixMode(GL_PROJECTION);

				glPopMatrix();

				/* Востанавливаем видовую матрицу */
				glMatrixMode(GL_MODELVIEW);

				glPopMatrix();

				/* Востанавливаем атрибуты */
				glPopAttrib();
			}

			/* Текущий режим: Пусто */
			CurrentRenderMode=NEW_NONE;

			break;
	}
}

/* Инициализация вычисления fps (кадров в секунду); Time - время в секундах, интервал между вычислением fps */
void newRender::StartCalculateFPS(float Time)
{
	/* fps вычисляется */
	ISfps=TRUE;	fps=0;

	/* Засекаем номер кадра */
	LastFPSFrame=Frame_Num;

	/* Стартуем счетчик */
	FPSTimer.newStartTiming();

	/* Интервал в тиках между вычислениями */
	FPSTime=(UINT64)Time*10000;
}

/* Обновление fps */
void newRender::UpdateFPS(UINT64 &DeltaTime)
{
	/* Если fps не вычисляется, то выходим */
	if (!ISfps)
		
		return;

	/* Смотрим прошедшее время */
	FPSTimer.newTimeElapsed();

	/* Если прошедшее время больше интервала */
	if (FPSTimer.ElapsedTime>FPSTime)
	{
		/* Вычисляем fps. Колличество_кадров*10000/Прошедшее_время */ 
		fps=(UINT)((Frame_Num-LastFPSFrame)*10000/FPSTimer.ElapsedTime);

		/* Стартуем счетчик */
		FPSTimer.newStartTiming();

		/* Запоминаем кадр */
		LastFPSFrame=Frame_Num;

		Tri_per_second = Tri_accum;

		Tri_accum = 0;
	}
}

void newRender::RestoreAll()
{
	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Востановление после смены режима...");

	/* Востанавливаем текстуры */
	for (Textures::iterator it=Texture.begin();it!=Texture.end();it++)
	{
		TextureUpload(&(*it).second);
	}

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Текстуры востановлены.");

	CompileDispalyLists();

	/* Востанавливаем шрифты */
	for (Fonts::iterator it2=Font.begin();it2!=Font.end();it2++)
	{
		newFont *font = &(*it2).second;

		MainRender.CreateFont(font);
	}

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Шрифты востановлены.");

	/* Востанавливаем консоль */
	ReshapeConsole();

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Консоль востановлена.");

	/* Вывод в консоль сообщения */
	MainConsole.Add(0,"Востановление прошло успешно.");	
}

/* Создание консоли */
void newRender::CreateConsole()
{
	/* Цвет белый */
	MainSettings.CONSOLE.COLOR.RGBColor.r=MainSettings.CONSOLE.COLOR.RGBColor.g=MainSettings.CONSOLE.COLOR.RGBColor.b=255;

	/* Текстуры нет */
	MainConsole.Tex=NULL;

	/* Если есть текстура, загружаем */
	if (!(MainSettings.CONSOLE.TEXTURE_FILE==""))
	{
		/* Генерируем текстуру, и ставим имя */
		MainConsole.Tex=TextureMan.newNewItem("Console skin");

		/* Загружаем */
		if (MainConsole.Tex->LoadFromFile(MainSettings.CONSOLE.TEXTURE_FILE)!=NEW_OK)
		{
			/* Если не удалось, то делаем пустую */
			MainConsole.Tex->CreateEmpty(16,16,MainSettings.CONSOLE.COLOR);
		}
		/* Загружаем в видео память */
		TextureUpload(MainConsole.Tex);
	}
	
	/* Шрифта нет */
	MainConsole.ConFont=NULL;

	/* Загружаем текстуру для шрифта; Первый параметр - путь до текстуры; Второй параметр - имя шрифта в массиве; Третий параметр - порог видимости пикселя */
	if (newLoadFontAndCheckSpace(MainSettings.CONSOLE.FONT_FILE,"Lucida Console",0))
	{
		/* Создаём шрифт */
		MainRender.CreateFont(MainRender.CurFont);

		MainConsole.ConFont=CurFont;
	}
	/* Подгоняем консоль под новые настройки */
	ReshapeConsole();
}

/* Функция обновления консоли с новыми настройками */
void newRender::ReshapeConsole()
{
	/* Двухмерный режим */
	RenderMode(NEW_RENDERMODE_2D);

	glLoadIdentity();

	/* Генерируем индекс дисплей листа */
	ConsoleDS=glGenLists(1);

	/* Начинаем заполнять лист  */
	glNewList(ConsoleDS,GL_COMPILE);

	/* Рисуем четырехугольником. Nachalo.  */
	glBegin(GL_QUADS);

	  glTexCoord2f(0,1);			

	  glVertex2i(0,FontDisplayHeight);

	  glTexCoord2f(1,1);

	  glVertex2i(FontDisplayWidth,FontDisplayHeight);

	  glTexCoord2f(1,0);

	  glVertex2i(FontDisplayWidth,0);

	  glTexCoord2f(0,0);

	  glVertex2i(0,0);

	/* Закончили рисовать */
	glEnd();

	/* Закончили лист */
	glEndList();

	/* Если на полный экран, то ставим Y нижнего края экрана */
	if (NEW_CONSOLE_FULLSCREEN == MainSettings.CONSOLE.STATUS)

		MainSettings.CONSOLE.YPOS=(float)FontDisplayHeight;

	/* Принудительно обновление видимых линий */
	BOOL temp=MainConsole.IsDrawing;

	MainConsole.IsDrawing=TRUE;

	MainConsole.UpdateCutten();

	MainConsole.IsDrawing=temp;
}

/* Отрисовка консоли */
void newRender::DrawConsole()
{
	/* Если консоль не рисуема, выходим */
	if (!MainConsole.IsDrawing)
		
		return;

	/* Если консоль не видно, выходим */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_NOTVISIBLE)
		
		return;

	/* Двухмерный режим */
	RenderMode(NEW_RENDERMODE_2D);

	glLoadIdentity();

	/* Если на полный экран, то эмулируем функцию Draw */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)
	{
		glClear(GL_COLOR_BUFFER_BIT);		
	}
	else
	{	
		/* Иначе двигаем консоль на её позицию */
		glTranslatef(0,MainSettings.CONSOLE.YPOS-FontDisplayHeight,0);
	}

	/* Текущий цвет */
	glColor4ubv(&MainSettings.CONSOLE.COLOR.Color[0]);

	/* Текстура */
	TextureEnable(MainConsole.Tex);

	/* Фукнция прозрачности */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Рисуем консоль */
	glCallList(ConsoleDS);

	/* Рисуем текст консоли */
	if (MainConsole.Console.size()>0)
	{
		/* Если шрифт существует */
		if (MainConsole.ConFont)
		{
			/* Сохраняем текущий шрифт */
			static newFont *safe;

			safe=CurFont;

			/* Устанавливаем текущим - консольный шрифт */
			CurFont=MainConsole.ConFont;

			/* Функция прозрачности для шрифта */
			glBlendFunc(GL_ONE, GL_ONE);

			/* Рисуем снизу вверх. Вычисляем текущий нижний край */
			float PosY=MainSettings.CONSOLE.YPOS-CurFont->Height*16-8;

			/* Белый цвет */
			MainConsole.ConFont->Color=newColor4ub(255,255,255,255);

			/* Рисуем текущую строку с курсором */
			static newString temp;

			temp=MainConsole.Current+"_";

			newPrint2D(8,PosY,temp.c_str());

			/* Переменная для Page Up, Page Down */
			UINT cline=0;

			/* Цикл по всем видимым линиям */
			for (newStringList::iterator it=MainConsole.Cutten.begin();it!=MainConsole.Cutten.end();it++)
			{
				/* Если не та страница, то крутим пока не будет нужной */
				if (cline<MainConsole.Line)
				{
					cline++;
					continue;
				}
				/* Отнимаем высоту шрифта каждую строчку */
				PosY-=CurFont->Height*16;

				/* Если это вне экрана, то выходим из цикла */
				if (PosY<0)
					
					break;

				/* Рисуем строку */
				newPrint2D(8,PosY,(*it).c_str());
			}

			/* Востанавливаем шрифт */
			CurFont=safe;
		}
	}

	/* Если на полный экран, то эмулируем функцию Draw */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)
	{
		glFlush();

		/* Меняем буферы (Двойная буфферизация) */
		SwapBuffers (safewindow->hDC);
	}
}

/* Обновление консоли, в зависимости от прошедшего времени */
void newRender::UpdateConsole(UINT64 DeltaTime)
{
	/* В зависимости от статуса двигаем консоль */
	switch (MainSettings.CONSOLE.STATUS)
	{
		case NEW_CONSOLE_FADEOUT:

			MainSettings.CONSOLE.YPOS-=MainSettings.CONSOLE.SPEED*(signed int)DeltaTime/NEW_TIC_SECOND;

			if (MainSettings.CONSOLE.YPOS<0)
			{
				MainSettings.CONSOLE.YPOS=0;

				MainSettings.CONSOLE.STATUS=NEW_CONSOLE_NOTVISIBLE;
			}
			break;

		case NEW_CONSOLE_FADEIN:

			MainSettings.CONSOLE.YPOS+=MainSettings.CONSOLE.SPEED*(signed int)DeltaTime/NEW_TIC_SECOND;

//+++			if (MainSettings.CONSOLE.YPOS>FontDisplayHeight/2)
			if (MainSettings.CONSOLE.YPOS>FontDisplayHeight/1)
			{
//+++				MainSettings.CONSOLE.YPOS=(float)FontDisplayHeight/2;
					MainSettings.CONSOLE.YPOS=(float)FontDisplayHeight/1;

				MainSettings.CONSOLE.STATUS=NEW_CONSOLE_VISIBLE;
			}
			break;
	}
}

/* Применение камеры к рендереру */
void newRender::ApplyCamera()
{
	/* Если нету камеры, то выходим */
	if (!CurrentCamera)
		
		return;

	/* Если изменился фрустум */
	if (CurrentCamera->Frustum.Reshape)
	{
		/* Выбираем матрицу проекции */
		glMatrixMode (GL_PROJECTION);

		/* Сбрасываем её на единичную */
		glLoadIdentity ();

		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (CurrentCamera->Frustum.GetFoV()*(GLfloat)(MainSettings.SCREEN_HEIGHT)/(GLfloat)(MainSettings.SCREEN_WIDTH)/2, (GLfloat)(MainSettings.SCREEN_WIDTH)/(GLfloat)(MainSettings.SCREEN_HEIGHT), CurrentCamera->Frustum.GetzNear(), CurrentCamera->Frustum.GetzFar());

		/* Выбираем видовую матрицу */
		glMatrixMode (GL_MODELVIEW);

		/* Сбрасываем её на единичную */
		glLoadIdentity ();

		CurrentCamera->Frustum.Reshape=FALSE;
	}

	/* В зависимости от стиля камеры */
    switch (CurrentCamera->CamStyle)
	{
		/* От первого лица */
		case NEW_CAMERASTYLE_FIRST_PERSON:

			/* Вращаем углами Эйлера */
			glRotatef(CurrentCamera->Pobj->rotEuler.Pitch,1,0,0);

			glRotatef(CurrentCamera->Pobj->rotEuler.Yaw,0,1,0);

			/* Переносим в позицию камеры */
			newVector3f &temp=CurrentCamera->Pobj->Position;

			glTranslatef(temp.X, temp.Y, temp.Z);

    		break;
    }
}

void newRender::MakeList(newRenderObject &ro)
{
	ro.ListID = glGenLists(1);

	glNewList(ro.ListID, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	/* glColor4f(1,1,1,1); */

	/* glPushMatrix(); */

	/* glScalef(0.005f,0.005f,0.005f); */

	UINT j=0;

	for (std::vector<UINT>::iterator i=ro.RIndex.begin(); i<ro.RIndex.end(); i++)
	{
		newRenderVertex &v = ro.RVertex[*i];

		glTexCoord2f(v.UV.X, v.UV.Y);

		glVertex3f(v.Coord.X, v.Coord.Y, v.Coord.Z);
	}

	/* glPopMatrix(); */

	glEnd();

	glEndList();
}

void newRender::CallList(newRenderObject &ro)
{
	glCallList(ro.ListID);
}
void newRender::SetColor(newVector3f Color)
{
	glColor3fv(&Color.X);
}
void newRender::SetColor(newVector4f Color)
{
	glColor4fv(&Color.X);
}

newDisplayList* newRender::GetNewDL()
{
	DList.push_back(newDisplayList());

	return &DList.back();
}

void newRender::DeleteDL(newDisplayList *dl)
{
	for (std::list<newDisplayList>::iterator it = DList.begin(); it != DList.end(); it++)

		if ((*it).Index == dl->Index)
			
			DList.erase(it);
}

void newRender::CompileDispalyLists()
{
	for (std::list<newDisplayList>::iterator it = DList.begin(); it != DList.end(); it++)

		(*it).Compile();
}

void newRender::RenderDisplayLists()
{
	/*	glDisable(GL_TEXTURE_2D); */

	for (
		std::list<newDisplayList>::iterator it 	= DList.begin(); 
	it != DList.end(); 
	it++)
	{
		(*it).mtl->Apply();

		(*it).Draw();
	}
	/*	glEnable(GL_TEXTURE_2D); */
}

void newDisplayList::Begin()
{
	glNewList(Index, GL_COMPILE);
}

void newDisplayList::End()
{
	glEndList();
}

void newDisplayList::Compile()
{
	if (!objlist.size())
		
		return;
	
	Index = glGenLists(1);

	glNewList(Index, GL_COMPILE);

	glBegin(GL_TRIANGLES);

	TriCount = 0;

	for (std::list<newRenderObject *>::iterator it = objlist.begin(); it != objlist.end(); it++)
	{
		(*it)->DumpDraw();

		TriCount += (UINT)((*it)->RIndex.size())/3;
	}
	glEnd();

	glEndList();
}

void newDisplayList::Draw()
{
	glCallList(Index);

	MainRender.Tri_per_frame += TriCount;

	if (MainSettings.Render.DrawNormals)
	{
		/* Рисуем нормали */
		glDisable(GL_LIGHTING);

		glColor4f(1,1,1,1);

		glBegin(GL_LINES);

		for (std::list<newRenderObject *>::iterator it = objlist.begin(); it != objlist.end(); it++)
		{
			newRenderObject *ro = (*it);

			for (size_t i=0;i<ro->RVertex.size();i++)
			{
				newVector3f Coord = ro->RVertex[i].Coord;

				glVertex3fv(Coord);

				newVector3f Normal = ro->RVertex[i].Normal;

				newVector3f Norm = Coord + Normal;

				glVertex3fv(Norm);
			}

		}
		glEnd();

		glEnable(GL_LIGHTING);
	}
}

UINT newRender::GetDisplayListCount()
{
	return (UINT)DList.size();
}

void newRender::DumpDraw(newRenderObject &ro)
{
UINT j=0;

	for (std::vector<UINT>::iterator i=ro.RIndex.begin(); i<ro.RIndex.end(); i++)
	{
		newRenderVertex &v = ro.RVertex[*i];

		glTexCoord2f(v.UV.X, v.UV.Y);

		glNormal3f(v.Normal.X, v.Normal.Y, v.Normal.Z);

		glVertex3f(v.Coord.X, v.Coord.Y, v.Coord.Z);
	}
}

newLight * newRender::CreateLight(newEnum ltype)
{
newLight *light;

	/* Detect available types of light source */
	if (NEW_LIGHT_AUTODETECT == ltype)
	{
		/* ATTENTION: not implemented yet */
		ltype = NEW_LIGHT_SIMPLE_OPENGL;
	}

	switch (ltype)
	{
		case NEW_LIGHT_SIMPLE_OPENGL:

			light = new newGLLight();

			light->Type = NEW_LIGHT_SIMPLE_OPENGL;

			break;

		case NEW_LIGHT_AUTODETECT:

			break;

		default:

			break;
	}

	return light;
}

void newRender::DeleteLight(newLight *light)
{
	if (light)
		
		delete light;
}

void newRender::InitLights()
{
/* Global light source */
newVector3f Ambient(0,0,0);

	/* Switch the light on. Включаем освещение */
	glEnable(GL_LIGHTING);	

	/* Set the global light */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);

	/* Double-sided layers' lighting computation. Двухсторонних свет */
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	/* Specular lighting computation. Сложный просчёт specular lighting */
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void newRender::SetMaterial(newMaterial *mtl)
{
	glEnable(GL_COLOR_MATERIAL);

	newVector4f temp(mtl->Ambient.X, mtl->Ambient.Y, mtl->Ambient.Z, 1);

    glMaterialfv(GL_FRONT, GL_AMBIENT, temp);

	if (NEW_FLOAT_EQ(mtl->ShineStrange,0))
	{
		temp.Set(0, 0, 0, 1);

		glMaterialfv(GL_FRONT, GL_SPECULAR, temp);
	}
	else
	{
		temp.Set(mtl->Specular.X, mtl->Specular.Y, mtl->Specular.Z, 1);

		glMaterialfv(GL_FRONT, GL_SPECULAR, temp);

		glMaterialf(GL_FRONT, GL_SHININESS, mtl->Shine*128);

		/* glMaterialf(GL_FRONT, GL_EMISSION, mtl->SelfIlumination); */
	}
}