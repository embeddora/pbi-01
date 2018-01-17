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

/* Save vsync state. ���������� ��������� vsync */
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


/* ���������� ���������� */
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

/* ��� ������ �� ����� */
char buf[1000];

GL_Window *safewindow;

/* ������������� OpenGL */
BOOL newRender::SetupOpenGL(GL_Window *window)
{
	safewindow=window;

	/* pfd ��������� Windows ��� ������������� OpenGL */
	PIXELFORMATDESCRIPTOR pfd =
	{
		/* ������ ����� Pixel Format Descriptor'� */
		sizeof (PIXELFORMATDESCRIPTOR),
		/* ����� ������ */
		1,
		/* ������ ������ ������������ ��������� � ���� */
		PFD_DRAW_TO_WINDOW | 
		/* ������ ������ ������������ OpenGL */
		PFD_SUPPORT_OPENGL |
		/* ������ ������ ������������ Double Buffering */
		PFD_DOUBLEBUFFER,
		/* ������ ������ ������������ RGBA ����� */
		PFD_TYPE_RGBA,
		/* �������� ������� ����� */
		window->init.bitsPerPixel,	
		/* ���������� ���� ����� */
		0, 0, 0, 0, 0, 0,
		/* ��� ����� ������ (���� ����) */
		0,
		/* ���������� ��� ������ */
		0,
		/* ���� Accumulation Buffer'� */
		0,	
		/* Accumulation ���� ���������� */
		0, 0, 0, 0,
		/* ������� Z-Buffer'� (Depth Buffer) */
		MainSettings.DEPTH,
		/* ���� Stencil Buffer'� (���� ����) */
		0,	
		/* ���� Auxiliary Buffer'� */
		0,	
		/* ������� ���� ��� ��������� */
		PFD_MAIN_PLANE,	
		/* ��������������� */
		0,	
		/* ���������� ����� ���� */
		0, 0, 0	
	};

	/* �������� ��������� ������ */
	GLuint PixelFormat;	

	/* ���� ����������� Pixel Format */
	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);

	/* �� �����? */
	if (0 == PixelFormat)	
	{
		/* ����������� �������� ��������� */
		ReleaseDC (window->hWnd, window->hDC);
		
		/* �������� ��� */
		window->hDC = 0;

		/* ������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ����� ���� */
		window->hWnd = 0;

		/* ���������� False (����/�������) */
		return FALSE;
	}

	/* �������� ���������� ���� ������ */
	if (FALSE == SetPixelFormat (window->hDC, PixelFormat, &pfd))
	{
		/* Jesli n�������,  to ����������� �������� ��������� */
		ReleaseDC (window->hWnd, window->hDC);

		/* �������� ��� */
		window->hDC = 0;

		/* ������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ����� ���� */
		window->hWnd = 0;

		/* ���������� False (����) */
		return FALSE;
	}

	/* ������� �������� �������� ������� */
	window->hRC = wglCreateContext (window->hDC);

	/* �� ��� ��������? */
	if (0 == window->hRC)
	{
		/* Jesli n�������, ����������� �������� ��������� */
		ReleaseDC (window->hWnd, window->hDC);

		/* �������� ��� */
		window->hDC = 0;

		/* ������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ����� ���� */
		window->hWnd = 0;

		/* ���������� False (����) */
		return FALSE;
	}

	/* ������ ���� ������� ������� ������� */
	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
	{
		/* Jesli n�������, ������� �������� ������� */
		wglDeleteContext (window->hRC);

		/* �������� ��� */
		window->hRC = 0;

		/* ����������� �������� ��������� */
		ReleaseDC (window->hWnd, window->hDC);

		/* �������� ��� */
		window->hDC = 0;

		/* ������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ����� ���� */
		window->hWnd = 0;

		/* ���������� False (����) */
		return FALSE;
	}

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������������� OpenGL ������ �������.");

	VendorName=(char *)glGetString(GL_VENDOR);

	MainConsole.Add(0,"������������� ����� �����: %s",VendorName.c_str());

	RendererName=(char *)glGetString(GL_RENDERER);

	MainConsole.Add(0,"��� ����� ����� (��������): %s",RendererName.c_str());

	VersionName=(char *)glGetString(GL_VERSION);

	MainConsole.Add(0,"������ OpenGL: %s",VersionName.c_str());

	extgl_Initialize();

	Extensions=(char *)glGetString(GL_EXTENSIONS);

	if (wglGetExtensionsStringARB)//+++ added by mazukevich
	{
		Extensions += (char *)wglGetExtensionsStringARB(window->hDC);

		MainConsole.Add(0,"���������� OpenGL, �������������� ����������: %s",Extensions.c_str());
	}

	if (wglGetSwapIntervalEXT)//+++ added by mazukevich
	{
		SafeSwapInterval = wglGetSwapIntervalEXT();

		wglSwapIntervalEXT(0);
	}
	
	MainConsole.Add(0,"���������� vsync.");

	return TRUE;
}

/* ����������� OpenGL ����, ���������� OpenGL ���� � ����������� ������� */
BOOL newRender::DestroyWindowGL (GL_Window* window)	
{
	if (wglGetSwapIntervalEXT) 

		/* �������������� Vsync */
		wglSwapIntervalEXT(SafeSwapInterval);

	/* ���� ����� �������? */
	if (0 != window->hWnd )
	{	
		/* ���� ����� �������� ����������? */
		if (0 != window->hDC)
		{
			/* ������ ������� ������� ������ �������� � ���� */
			wglMakeCurrent (window->hDC, 0);

			/* ���� ����� ������ ��������? */
			if (window->hRC != 0)
			{
				/* ����������� ������ �������� */
				wglDeleteContext (window->hRC);

				/* �������� ������ �������� */
				window->hRC = 0;
			}

			/* ����������� �������� ��������� */
			ReleaseDC (window->hWnd, window->hDC);

			/* �������� �������� ��������� */
			window->hDC = 0;
		}

		/* ����� � ������� ��������� */
		MainConsole.Add(0,"�������� ��������� OpenGL.");

		/* ���������� ���� */
		DestroyWindow (window->hWnd);

		/* �������� ���������� ���� */
		window->hWnd = 0;
	}

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������� ������� ����.");

	/* ����������? */
	if (window->init.isFullScreen)
	{
		/* �������������� ���������� ������ */
		ChangeDisplaySettings (NULL,0);

		/* �������� ������ ���� */
		ShowCursor (TRUE);
	}	

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"�������������� ���������� ������.");

	/* ��������� True */
	return TRUE;
}

void newRender::Reshape(int width, int height)
{
	RenderMode(NEW_RENDERMODE_3D);

	/* ����������������� ViewPort (������� ���������) */
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));

	MainInput.MouseCenter.Set((float)width/2,(float)height/2);

	/* �������� ������� �������� */
	glMatrixMode (GL_PROJECTION);

	/* ���������� � �� ��������� */
	glLoadIdentity ();

	if (CurrentCamera)

		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (CurrentCamera->Frustum.GetFoV()*(GLfloat)(height)/(GLfloat)(width)/2, (GLfloat)(width)/(GLfloat)(height), CurrentCamera->Frustum.GetzNear(), CurrentCamera->Frustum.GetzFar());	

	else
		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (MainSettings.FOV*(GLfloat)(height)/(GLfloat)(width)/2, (GLfloat)(width)/(GLfloat)(height),	MainSettings.ZNEAR, MainSettings.ZFAR);		
	
	/* �������� ������� ������� */
	glMatrixMode (GL_MODELVIEW);

	/* ���������� � �� ��������� */
	glLoadIdentity ();
}


#if (1) /* it's original stuff - from original STEP8 */

/* ���������� �������� � �����-������ */
void newRender::TextureUpload(newTexture *tex)
{
	/* ������ �������� �� ������ */
	GLuint type,components;

	/* � ����������� �� ��� �� ������� */
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

	/* ���������� ������ OpenGL �������� */
	glGenTextures(1, (GLuint *)&tex->TextureID);

	/* ������ ������ �� �������� */
	glBindTexture(GL_TEXTURE_2D, tex->TextureID);

	/* ��� ������ �������� */
	GLuint kak;

	/* �������� ��� ������ */
	switch (tex->Border_Type)
	{
		/* �������� �� ����������� */
		case NEW_TEXTURE_BORDER_CLAMP:

			kak=GL_CLAMP;

			break;

		/* �������� �����������  */
		case NEW_TEXTURE_BORDER_REPEAT:

			kak=GL_REPEAT;
			
			break;
	}

	/* ������������� ��������� ������ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, kak);

	/* ������������� ���������� ��������: 10 - ����� ������, �� ���������, ...,  0 - ����� ������, �� ������� */
	
	/* �������: 10-� ������ �� ����� ������ � � ���������� ������� ����� ����������� */
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

	/* ���� �������������� �������������� ��������� ��������, �� ���������� �� �� ���������� */
	if (tex->GenerateMipMap)

//+++ this string is commented out by mazukevich:  if ((extgl_Extensions.OpenGL14)||(extgl_Extensions.SGIS_generate_mipmap))

			if (IsPow2(tex->Width) && IsPow2(tex->Height))
			{
				/* ������ ��������� �������� */
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

				/* �������������� �������� ��� ������ ����� ����. (��������� Ati Rage Fury Pro � �.�.) */
				int genmip = 0;

				glGetTexParameteriv(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, &genmip);

				/* ���� ����� ��������� ����������, �� ���������� � ����� */
				if (!genmip)

				{
					if (tex->Filter<4)

						glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

					else

						gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

				}
				else /* �����, �� ��. ����� ����� ����������� ������� */
				{
					glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);

					glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

					/* �������������� ��������� ������ ������� */
					tex->GenerateMipMap = 3;
				}
			}
			else /* ����� ��� ������ */

				if (tex->GenerateMipMap)

					gluBuild2DMipmaps(GL_TEXTURE_2D, components, tex->Width, tex->Height, type, GL_UNSIGNED_BYTE, &tex->Data[0]);

				else

					glTexImage2D(GL_TEXTURE_2D, 0, type, tex->Width, tex->Height, 0, type, GL_UNSIGNED_BYTE, &tex->Data[0]);
			

	TextureDisable();
}
#else /* the following is the stuff ingerited from STEP7 */

/* ���������� �������� � �����-������ */
void newRender::TextureUpload(newTexture *tex)
{
	/* ������ �������� �� ������ */
	GLuint type,components;

	/* � ����������� �� ��� �� ������� */
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

	/* Generating index of OpenGL textgure; ���������� ������ OpenGL �������� */
	glGenTextures(1, (GLuint *)&tex->TextureID);

	/* Bind this index to texture; ������ ������ �� �������� */
	glBindTexture(GL_TEXTURE_2D, tex->TextureID);

	/* Type of texture borders; ��� ������ �������� */
	GLuint kak;

	/* Borders type selection; �������� ��� ������ */
	switch (tex->Border_Type)
	{
		/* Texture doesn't repet; �������� �� ����������� */
		case NEW_TEXTURE_BORDER_CLAMP:

			kak = GL_CLAMP;
			
			break;

		/* Texture repeats; �������� ����������� */
		case NEW_TEXTURE_BORDER_REPEAT:

			kak=GL_REPEAT;
			
			break;
	}

	/* Setting border parameters; ������������� ��������� ������ */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, kak);

	/* Setting border parameters; ������������� ��������� ������ */
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


/* Enabling the texure. �������� �������� */
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

/* Disabling texture. ��������� �������� */
void newRender::TextureDisable()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	CurTexID=0;
}

/* ������ ���������� �������� */
void newRender::TextureOperation(newEnum op)
{	
	switch (op)
	{
		case NEW_TEXTURE_COMBINE_MODULATE:
		{
			/* Modulate		- ��������� */
			glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

			break;
		}

		default: 

			/* cases, to be added: Replace	- ������, Add - ����������, Interpolate	- ���������������� */
			break;
	}
}


newGLRender::newGLRender(void)
{
}

newGLRender::~newGLRender(void)
{
}

/* ���������� ����� � ������ */
void newGetScreen(BYTE *Data)
{
	/* �������� ������ ������ */
	glReadBuffer(GL_FRONT);

	/* ��� Data ��� �������� ������. ������ � �� ���� ����� */
	glReadPixels(0,0,MainSettings.SCREEN_WIDTH,MainSettings.SCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,Data);
}

/* ������ ����� */
BOOL newRender::CreateFont(newFont *font)
{
/* �������� X ���������� ����� */
float	cx;

/* �������� Y ���������� ����� */
float	cy;

/* ������ ����� � �������� */
float   cwx;

/* ������ ����� � �������� */
float   cwy;

	/* ��������� ������ �� ��������� �������: 16�16 */
	cwx         = (1.0f / 256.0f) * 16;

	/* ��������� ������ �� ��������� �������: 16�16 */
	cwy         = (1.0f / 256.0f) * 16;

	/* ������ 256 ������� ������ */
	font->Base=glGenLists(256);

	/* �������� �� ���� ������ */
	for (int loop=0; loop<256; loop++)
	{
		/* X � Y ������� */
		cx=float(loop%16)/16.0f;

		cy=float(loop/16)/16.0f;

		/* �������� ��������� ����  */
		glNewList(font->Base+loop,GL_COMPILE);

		/* ������ ����� �����������������. Nachalo. */
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

		/* ��������� �������� */
		glEnd();

		/* ������������ �� ������ ������� */
		glTranslatef((GLfloat)(font->FTrim[loop][1]+font->Spacing),0,0);
			
		/* ��������� ���� */
		glEndList();

	}/*���� �� ������ */
	
	return TRUE;
}

/* ������� ����� */
void newRender::KillFont(newFont *font)
{
	/* ������� ��� ����� (�����) */
	glDeleteLists(font->Base, 256);
}

/* ������� ������ ������ � ���������� ������; x,y - �������; Text - �� �������� � printf */
void newPrint2D(float x,float y,const char *Text,...)
{
	/* ������ �� ������ ���������� */
	va_list		va;

	/* ���� ���� ������ */
	if("" == Text)

		/* ������ �� ������ */
		return;

	/* �������� ����� � buf, ������� ���������� �� ���������� */
	va_start(va, Text);

	vsprintf(buf, Text, va);

	va_end(va);

	/* ������������� ���� */
	glColor4ubv(&MainRender.CurFont->Color.Color[0]);

	/* �������� �������� */
	MainRender.TextureEnable(MainRender.CurFont->TextureLink);

	/* ��������� ������� 2D ������ */
	MainRender.RenderMode(NEW_RENDERMODE_2D);

	/* ��������� ������� ������� */
	glPushMatrix();

	/* ������ � �������� */
	glLoadIdentity();

	/* �����������, � ����������� �� ������ � ������ ���� */
	glScalef(MainRender.CurFont->Width,MainRender.CurFont->Height, 1);

	/* ���������� �� x � y */
	glTranslated(x/MainRender.CurFont->Width,y/MainRender.CurFont->Height,0);

	/* ������� ���� ������ �� ������� */
	glListBase(MainRender.CurFont->Base);

	/* ������ ���� ����� ������� ��������������� ������ */
	glCallLists((GLsizei)strlen(buf), GL_UNSIGNED_BYTE, buf);

	/* �������������� ������� ������� */
	glPopMatrix();


}

void newRender::RenderMode(newEnum mode)
{
	switch (mode)
	{
		/* NEW_RENDERMODE_3D - ��������� �����, ������� ��������� */
		case NEW_RENDERMODE_3D:

			/* ��������� ������� ����� */
			if (CurrentRenderMode!=NEW_RENDERMODE_3D)
			{
				/* ����������� ���������� */
				RenderMode(NEW_RENDERMODE_RESTORE_LAST);

				/* ���� ��� ������, �� ������� */
				CurrentRenderMode=NEW_RENDERMODE_3D;
			}
			break;

		/* NEW_RENDERMODE_2D - ���������� �����, �����, ������� � �.�. */
		case NEW_RENDERMODE_2D:

			/* ��������� ������� ����� */
			if (CurrentRenderMode!=NEW_RENDERMODE_2D)
			{
				/* ����������� ���������� */
				RenderMode(NEW_RENDERMODE_RESTORE_LAST);

				/* ��������� �������� */
				glPushAttrib(GL_BLEND | GL_DRAW_BUFFER | GL_COLOR_BUFFER_BIT |
					GL_LOGIC_OP | GL_DEPTH_BUFFER_BIT |
					GL_DEPTH_TEST | GL_ENABLE_BIT | GL_LIGHTING_BIT | 
					GL_STENCIL_BUFFER_BIT );

				/* ��������� ������� �������� */
				glMatrixMode(GL_PROJECTION);

				glPushMatrix();

				/* ������ � ��������� */
				glLoadIdentity();

				/* ������������ 2D �����, � ����������� �� �������� �������� */
				glOrtho(0,FontDisplayWidth,FontDisplayHeight,0,-100,100);

				/* ��������� ������� ������� */
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();

				/* ������ � ��������� */
				glLoadIdentity();

				/* ��������� ���� ������� */
				glDisable(GL_DEPTH_TEST);

				/* �������� ������������ */
				glEnable(GL_BLEND);

				/* �������� ������������ */
				glBlendFunc(GL_ONE,GL_ONE);

				/* ��������� ������ �  ������ ������� */
				glDepthMask(FALSE);

				/* ��������� ���� */
				glDisable(GL_LIGHTING);

				/* ������������� ������� ����� */
				CurrentRenderMode=NEW_RENDERMODE_2D;
			}

			break;

		/* NEW_RENDERMODE_RESTORE_LAST - �������������� ���������� ����� */
		case NEW_RENDERMODE_RESTORE_LAST:

			/* ���� ����� 2D */
			if (CurrentRenderMode==NEW_RENDERMODE_2D)
			{
				/* �������������� ������� �������� */
				glMatrixMode(GL_PROJECTION);

				glPopMatrix();

				/* �������������� ������� ������� */
				glMatrixMode(GL_MODELVIEW);

				glPopMatrix();

				/* �������������� �������� */
				glPopAttrib();
			}

			/* ������� �����: ����� */
			CurrentRenderMode=NEW_NONE;

			break;
	}
}

/* ������������� ���������� fps (������ � �������); Time - ����� � ��������, �������� ����� ����������� fps */
void newRender::StartCalculateFPS(float Time)
{
	/* fps ����������� */
	ISfps=TRUE;	fps=0;

	/* �������� ����� ����� */
	LastFPSFrame=Frame_Num;

	/* �������� ������� */
	FPSTimer.newStartTiming();

	/* �������� � ����� ����� ������������ */
	FPSTime=(UINT64)Time*10000;
}

/* ���������� fps */
void newRender::UpdateFPS(UINT64 &DeltaTime)
{
	/* ���� fps �� �����������, �� ������� */
	if (!ISfps)
		
		return;

	/* ������� ��������� ����� */
	FPSTimer.newTimeElapsed();

	/* ���� ��������� ����� ������ ��������� */
	if (FPSTimer.ElapsedTime>FPSTime)
	{
		/* ��������� fps. �����������_������*10000/���������_����� */ 
		fps=(UINT)((Frame_Num-LastFPSFrame)*10000/FPSTimer.ElapsedTime);

		/* �������� ������� */
		FPSTimer.newStartTiming();

		/* ���������� ���� */
		LastFPSFrame=Frame_Num;

		Tri_per_second = Tri_accum;

		Tri_accum = 0;
	}
}

void newRender::RestoreAll()
{
	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������������� ����� ����� ������...");

	/* �������������� �������� */
	for (Textures::iterator it=Texture.begin();it!=Texture.end();it++)
	{
		TextureUpload(&(*it).second);
	}

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"�������� ������������.");

	CompileDispalyLists();

	/* �������������� ������ */
	for (Fonts::iterator it2=Font.begin();it2!=Font.end();it2++)
	{
		newFont *font = &(*it2).second;

		MainRender.CreateFont(font);
	}

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������ ������������.");

	/* �������������� ������� */
	ReshapeConsole();

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������� ������������.");

	/* ����� � ������� ��������� */
	MainConsole.Add(0,"������������� ������ �������.");	
}

/* �������� ������� */
void newRender::CreateConsole()
{
	/* ���� ����� */
	MainSettings.CONSOLE.COLOR.RGBColor.r=MainSettings.CONSOLE.COLOR.RGBColor.g=MainSettings.CONSOLE.COLOR.RGBColor.b=255;

	/* �������� ��� */
	MainConsole.Tex=NULL;

	/* ���� ���� ��������, ��������� */
	if (!(MainSettings.CONSOLE.TEXTURE_FILE==""))
	{
		/* ���������� ��������, � ������ ��� */
		MainConsole.Tex=TextureMan.newNewItem("Console skin");

		/* ��������� */
		if (MainConsole.Tex->LoadFromFile(MainSettings.CONSOLE.TEXTURE_FILE)!=NEW_OK)
		{
			/* ���� �� �������, �� ������ ������ */
			MainConsole.Tex->CreateEmpty(16,16,MainSettings.CONSOLE.COLOR);
		}
		/* ��������� � ����� ������ */
		TextureUpload(MainConsole.Tex);
	}
	
	/* ������ ��� */
	MainConsole.ConFont=NULL;

	/* ��������� �������� ��� ������; ������ �������� - ���� �� ��������; ������ �������� - ��� ������ � �������; ������ �������� - ����� ��������� ������� */
	if (newLoadFontAndCheckSpace(MainSettings.CONSOLE.FONT_FILE,"Lucida Console",0))
	{
		/* ������ ����� */
		MainRender.CreateFont(MainRender.CurFont);

		MainConsole.ConFont=CurFont;
	}
	/* ��������� ������� ��� ����� ��������� */
	ReshapeConsole();
}

/* ������� ���������� ������� � ������ ����������� */
void newRender::ReshapeConsole()
{
	/* ���������� ����� */
	RenderMode(NEW_RENDERMODE_2D);

	glLoadIdentity();

	/* ���������� ������ ������� ����� */
	ConsoleDS=glGenLists(1);

	/* �������� ��������� ����  */
	glNewList(ConsoleDS,GL_COMPILE);

	/* ������ �����������������. Nachalo.  */
	glBegin(GL_QUADS);

	  glTexCoord2f(0,1);			

	  glVertex2i(0,FontDisplayHeight);

	  glTexCoord2f(1,1);

	  glVertex2i(FontDisplayWidth,FontDisplayHeight);

	  glTexCoord2f(1,0);

	  glVertex2i(FontDisplayWidth,0);

	  glTexCoord2f(0,0);

	  glVertex2i(0,0);

	/* ��������� �������� */
	glEnd();

	/* ��������� ���� */
	glEndList();

	/* ���� �� ������ �����, �� ������ Y ������� ���� ������ */
	if (NEW_CONSOLE_FULLSCREEN == MainSettings.CONSOLE.STATUS)

		MainSettings.CONSOLE.YPOS=(float)FontDisplayHeight;

	/* ������������� ���������� ������� ����� */
	BOOL temp=MainConsole.IsDrawing;

	MainConsole.IsDrawing=TRUE;

	MainConsole.UpdateCutten();

	MainConsole.IsDrawing=temp;
}

/* ��������� ������� */
void newRender::DrawConsole()
{
	/* ���� ������� �� �������, ������� */
	if (!MainConsole.IsDrawing)
		
		return;

	/* ���� ������� �� �����, ������� */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_NOTVISIBLE)
		
		return;

	/* ���������� ����� */
	RenderMode(NEW_RENDERMODE_2D);

	glLoadIdentity();

	/* ���� �� ������ �����, �� ��������� ������� Draw */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)
	{
		glClear(GL_COLOR_BUFFER_BIT);		
	}
	else
	{	
		/* ����� ������� ������� �� � ������� */
		glTranslatef(0,MainSettings.CONSOLE.YPOS-FontDisplayHeight,0);
	}

	/* ������� ���� */
	glColor4ubv(&MainSettings.CONSOLE.COLOR.Color[0]);

	/* �������� */
	TextureEnable(MainConsole.Tex);

	/* ������� ������������ */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* ������ ������� */
	glCallList(ConsoleDS);

	/* ������ ����� ������� */
	if (MainConsole.Console.size()>0)
	{
		/* ���� ����� ���������� */
		if (MainConsole.ConFont)
		{
			/* ��������� ������� ����� */
			static newFont *safe;

			safe=CurFont;

			/* ������������� ������� - ���������� ����� */
			CurFont=MainConsole.ConFont;

			/* ������� ������������ ��� ������ */
			glBlendFunc(GL_ONE, GL_ONE);

			/* ������ ����� �����. ��������� ������� ������ ���� */
			float PosY=MainSettings.CONSOLE.YPOS-CurFont->Height*16-8;

			/* ����� ���� */
			MainConsole.ConFont->Color=newColor4ub(255,255,255,255);

			/* ������ ������� ������ � �������� */
			static newString temp;

			temp=MainConsole.Current+"_";

			newPrint2D(8,PosY,temp.c_str());

			/* ���������� ��� Page Up, Page Down */
			UINT cline=0;

			/* ���� �� ���� ������� ������ */
			for (newStringList::iterator it=MainConsole.Cutten.begin();it!=MainConsole.Cutten.end();it++)
			{
				/* ���� �� �� ��������, �� ������ ���� �� ����� ������ */
				if (cline<MainConsole.Line)
				{
					cline++;
					continue;
				}
				/* �������� ������ ������ ������ ������� */
				PosY-=CurFont->Height*16;

				/* ���� ��� ��� ������, �� ������� �� ����� */
				if (PosY<0)
					
					break;

				/* ������ ������ */
				newPrint2D(8,PosY,(*it).c_str());
			}

			/* �������������� ����� */
			CurFont=safe;
		}
	}

	/* ���� �� ������ �����, �� ��������� ������� Draw */
	if (MainSettings.CONSOLE.STATUS==NEW_CONSOLE_FULLSCREEN)
	{
		glFlush();

		/* ������ ������ (������� ������������) */
		SwapBuffers (safewindow->hDC);
	}
}

/* ���������� �������, � ����������� �� ���������� ������� */
void newRender::UpdateConsole(UINT64 DeltaTime)
{
	/* � ����������� �� ������� ������� ������� */
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

/* ���������� ������ � ��������� */
void newRender::ApplyCamera()
{
	/* ���� ���� ������, �� ������� */
	if (!CurrentCamera)
		
		return;

	/* ���� ��������� ������� */
	if (CurrentCamera->Frustum.Reshape)
	{
		/* �������� ������� �������� */
		glMatrixMode (GL_PROJECTION);

		/* ���������� � �� ��������� */
		glLoadIdentity ();

		/* Calculate The Aspect Ratio Of The Window */
		gluPerspective (CurrentCamera->Frustum.GetFoV()*(GLfloat)(MainSettings.SCREEN_HEIGHT)/(GLfloat)(MainSettings.SCREEN_WIDTH)/2, (GLfloat)(MainSettings.SCREEN_WIDTH)/(GLfloat)(MainSettings.SCREEN_HEIGHT), CurrentCamera->Frustum.GetzNear(), CurrentCamera->Frustum.GetzFar());

		/* �������� ������� ������� */
		glMatrixMode (GL_MODELVIEW);

		/* ���������� � �� ��������� */
		glLoadIdentity ();

		CurrentCamera->Frustum.Reshape=FALSE;
	}

	/* � ����������� �� ����� ������ */
    switch (CurrentCamera->CamStyle)
	{
		/* �� ������� ���� */
		case NEW_CAMERASTYLE_FIRST_PERSON:

			/* ������� ������ ������ */
			glRotatef(CurrentCamera->Pobj->rotEuler.Pitch,1,0,0);

			glRotatef(CurrentCamera->Pobj->rotEuler.Yaw,0,1,0);

			/* ��������� � ������� ������ */
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
		/* ������ ������� */
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

	/* Switch the light on. �������� ��������� */
	glEnable(GL_LIGHTING);	

	/* Set the global light */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Ambient);

	/* Double-sided layers' lighting computation. ������������� ���� */
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	/* Specular lighting computation. ������� ������� specular lighting */
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