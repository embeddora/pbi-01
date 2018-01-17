#pragma once

#include "Vector.h"

#include "Angles.h"

#include <math.h>

#pragma inline_depth( 255 )

#pragma inline_recursion( on )

#pragma auto_inline( on )

#define inline __forceinline 


/* Namespace для структуры вектор */


namespace new_quaternion
{

/* Аргументы */


    template< class ta_a >
    class vecarg
    {
        const ta_a& Argv;
    public:
        inline vecarg( const ta_a& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return Argv.Evaluate( i ); }
    };

/*    template<> 
    class vecarg< const float >
    {
        const ta_a& Argv;
    public:
        inline vecarg( const ta_a& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return Argv; }
    };

    template<> 
    class vecarg< const int >
    {
        const ta_a& Argv;
    public:
        inline vecarg( const ta_a& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return (float)Argv; }
    };
*/

///////////////////////////////////////////////////////////////////////////
// Выражения
///////////////////////////////////////////////////////////////////////////

    template< class ta_a, class ta_b, class ta_eval >
    class vecexp_2
    {
        const vecarg<ta_a>   Arg1;
        const vecarg<ta_b>   Arg2;

    public:
        inline vecexp_2( const ta_a& A1, const ta_b& A2 ) : Arg1( A1 ), Arg2( A2 ) {}
        inline const float Evaluate ( const int i ) const
        { return ta_eval::Evaluate( i, Arg1, Arg2 ); }
    };

    template< class ta_a, class ta_eval >
    class vecexp_1
    {
        const vecarg<ta_a>   Arg1;

    public:
        inline vecexp_1( const ta_a& A1 ) : Arg1( A1 ) {}

        inline const float Evaluate( const int i ) const
        { return ta_eval::Evaluate( i, Arg1.Evaluate( i ) ); }
    };

///////////////////////////////////////////////////////////////////////////
// Базовый класс
///////////////////////////////////////////////////////////////////////////

    template< int ta_dimension, class T >
    struct base : public T
    {
        inline       float&  operator[]( const int i )       { return ((float*)this)[i]; } 
        inline const float   Evaluate  ( const int i ) const { return ((float*)this)[i]; }

        //////////////////////////////////////////////////////////////////
        // Присвоение
        //////////////////////////////////////////////////////////////////
        template< class ta >
        struct assigment
        {
            template< int I, class R >
            struct recurse
            {
                enum { COUNTER = I+1 };

                static inline void Assign( base<ta_dimension,T>& V, const ta& A ) 
                {
                    V[I] = A.Evaluate( I );
                    recurse<COUNTER,int>::Assign( V, A );
                }
            };
/*
            template<> struct recurse<ta_dimension,int>
            {
                static inline void Assign( base<ta_dimension,T>& V, const ta& A ){}
            };

            static inline void Assign( base<ta_dimension,T>& V, const ta& A )
            {
                recurse<0,int>::Assign( V, A );
            }
*/
        };

        template< class ta_type > inline
        const base<ta_dimension,T>& operator = ( const ta_type& A )
        {
            assigment<ta_type>::Assign( *this, A );
            return *this;
        }

        //////////////////////////////////////////////////////////////////
        // Норма
        //////////////////////////////////////////////////////////////////
        template< class ta>
        struct norma2
        {
            template< int I, class R >
            struct recurse
            {
                enum { COUNTER = I+1 };

                static inline float Norma2( const ta& A ) 
                {
                    return A.Evaluate( I ) * A.Evaluate( I ) +
                        recurse<COUNTER,int>::Norma2( A );
                }
            };
/*
            template<> struct recurse<ta_dimension,int>
            {
                static inline float Norma2( const ta& A)
                {
                    return 0;
                }
            };

            static inline float Norma2( const ta& A)
            {
                return recurse<0,int>::Norma2( A );
            }
*/
        };

//+++        template<> inline
        template<int iDummy> inline
        float Norma() const
        {
		 iDummy = 0;
            return (float)sqrt( norma2< base<ta_dimension,T> >::Norma2( *this ));
		}

 
	};

	const int qm[4][4]={ {0,3,2,1},{1,2,3,0},{2,1,0,3},{3,0,1,2}};
	const int qz[4][3]={ {1,1,-1},{-1,1,1},{1,-1,1},{-1,-1,-1}};
	struct mul
	{
		template< class ta_a, class ta_b > inline static 
			const float Evaluate( const int i, const ta_a& A, const ta_b& B )
		{ return     B.Evaluate(3) * A.Evaluate(qm[i][0])
		+ qz[i][0] * B.Evaluate(0) * A.Evaluate(qm[i][1])
		+ qz[i][1] * B.Evaluate(1) * A.Evaluate(qm[i][2])
		+ qz[i][2] * B.Evaluate(2) * A.Evaluate(qm[i][3]); }
	};

	template< class ta_c1, class ta_c2> inline 
		const vecexp_2< const ta_c1, const ta_c2, mul > 
		operator * ( const ta_c1& Pa, const ta_c2& Pb )
	{
		return vecexp_2< const ta_c1, const ta_c2, mul >( Pa, Pb );
	}


///////////////////////////////////////////////////////////////////////////
// Данные
///////////////////////////////////////////////////////////////////////////

    struct desc_xyzw_strq
    {
		float X, Y, Z, W;
    };
};

///////////////////////////////////////////////////////////////////////////
// Quaternion
///////////////////////////////////////////////////////////////////////////

// Кватернион с компонентами типа float, структура занимает 4*sizeof(float) байт
struct newQuaternionf : public new_quaternion::base< 4, new_quaternion::desc_xyzw_strq>
{
    // Определяем тип
	typedef new_quaternion::base< 4, new_quaternion::desc_xyzw_strq > base;

	// Конструкторы
	// Стандартный
	inline newQuaternionf() { X=0; Y=0; Z=0; W=1; };
    
	// По координатам
	inline newQuaternionf( const float x, const float y, const float z, const float w)
	{ X=x; Y=y; Z=z; W=w; };

	// Создать из угла и оси
	inline newQuaternionf(float Angle, newVector3f &Axis)
	{
		double rad=NEW_DEGTORAD(Angle * 0.5f);
		W = (float)cos(rad);
		double scale = sin(rad);
		Axis.Normalize();
		X=float(Axis.X*scale);
		Y=float(Axis.Y*scale);
		Z=float(Axis.Z*scale);
		Normalize();
	}

	// Создать из осевого угла
	inline newQuaternionf(newAxisAngle &src)
	{
		double rad=NEW_DEGTORAD(src.Angle * 0.5f);
		W = (float)cos(rad);
		double scale = sin(rad);
		src.Axis.Normalize();
		X=float(src.Axis.X*scale);
		Y=float(src.Axis.Y*scale);
		Z=float(src.Axis.Z*scale);
		Normalize();
	}

	// Переназначить из угла и оси
	inline void FromAxisAngle(float Angle, newVector3f Axis)
	{
		double rad=NEW_DEGTORAD(Angle * 0.5f);
		W = (float)cos(rad);
		double scale = sin(rad);
		Axis.Normalize();
		X=float(Axis.X*scale);
		Y=float(Axis.Y*scale);
		Z=float(Axis.Z*scale);
		Normalize();
	}

	// Переназначить из осевого угла
	inline void FromAxisAngle(newAxisAngle &src)
	{
		double rad=NEW_DEGTORAD(src.Angle * 0.5f);
		W = (float)cos(rad);
		double scale = sin(rad);
		src.Axis.Normalize();
		X=float(src.Axis.X*scale);
		Y=float(src.Axis.Y*scale);
		Z=float(src.Axis.Z*scale);
		Normalize();
	}

	// Сконвертировать в осевой угол
	inline newAxisAngle& ToAxisAngle(newAxisAngle &dst)
	{
		float s=X*X+Y*Y+Z*Z;
		if (s>0)
		{
			dst.Angle = (float) NEW_RADTODEG( 2.0f * acos(W) );
			double inv_s=1/sqrt(s);
			dst.Axis.X = float(X*inv_s);
			dst.Axis.Y = float(Y*inv_s);
			dst.Axis.Z = float(Z*inv_s);
		}
		else 
		{
			dst.Angle = 0;
			dst.Axis = newVector3f(0, 0, 1);
		}
		return dst;
	}

	// Операторы присвоения
	template< class ta_type > inline
    newQuaternionf ( const ta_type& A )
    { base::operator = ( A ); }
    template< class ta_type > inline
    newQuaternionf& operator = ( const ta_type& A )
    { base::operator = ( A ); return *this; }
	// Оператор умножения с присвоением
	template< class ta_type > inline
    newQuaternionf& operator *= ( const ta_type& A )
    { base::operator = ( *this * A ); return *this; }

	// Нормализация
	inline void Normalize()
	{
		//+++float norm = Norma();
		float norm = 0.1;
		X/= norm;
		Y/= norm;
		Z/= norm;
		W/= norm;
		
	}

	// Переназначить из углов Эйлера, заданных координатами
	inline void FromEuler(float x, float y, float z)
	{
		double ex = NEW_DEGTORAD(x) / 2.0;
		double ey = NEW_DEGTORAD(y) / 2.0;
		double ez = NEW_DEGTORAD(z) / 2.0;

		double	cr = cos(ex);
		double	cp = cos(ey);
		double	cy = cos(ez);

		double	sr = sin(ex);
		double	sp = sin(ey);
		double	sy = sin(ez);

		double	cpcy = cp * cy;
		double	spsy = sp * sy;

		W = float(cr * cpcy + sr * spsy);

		X = float(sr * cpcy - cr * spsy);
		Y = float(cr * sp * cy + sr * cp * sy);
		Z = float(cr * cp * sy - sr * sp * cy);

		Normalize();

	}

	// Переназначить из углов Эйлера
	inline void FromEuler(const newEulerAngle &a)
	{
		double ex = NEW_DEGTORAD(a.Pitch) / 2.0;
		double ey = NEW_DEGTORAD(a.Yaw  ) / 2.0;
		double ez = NEW_DEGTORAD(a.Roll ) / 2.0;

		double	cr = cos(ex);
		double	cp = cos(ey);
		double	cy = cos(ez);

		double	sr = sin(ex);
		double	sp = sin(ey);
		double	sy = sin(ez);

		double	cpcy = cp * cy;
		double	spsy = sp * sy;

		W = float(cr * cpcy + sr * spsy);

		X = float(sr * cpcy - cr * spsy);
		Y = float(cr * sp * cy + sr * cp * sy);
		Z = float(cr * cp * sy - sr * sp * cy);

		Normalize();

	}

	// Оператор комбинирования
	inline void operator |= (const newVector3f &v)
	{
		float qX = X, qY = Y, qZ = Z, qW = W;

		X = qW * v.X            + qY * v.Z - qZ * v.Y;
		Y = qW * v.Y - qX * v.Z            + qZ * v.X;
		Z = qW * v.Z + qX * v.Y - qY * v.X;
		W =          - qX * v.X - qY * v.Y - qZ * v.Z;

	}

	// Сферико-линейная интерполяция
	inline void Slerp(const newQuaternionf& dst, float factor)
	{
		double fAngle = acos(X * dst.X + Y * dst.Y + Z * dst.Z + W * dst.W);
		if ( NEW_FLOAT_EQ( fAngle ,0)) return;
		if ( NEW_FLOAT_EQ( NEW_PI - fAngle ,0)) return;
		NEW_LIMIT_RANGE(0,factor,1);
		double fInvSin = 1.0f/sin(fAngle);
		double fCoeff0 = sin((1.0f-factor)*fAngle)*fInvSin;
		double fCoeff1 = sin(factor*fAngle)*fInvSin;
		X = float(fCoeff0 * X + fCoeff1 * dst.X);
		Y = float(fCoeff0 * Y + fCoeff1 * dst.Y);
		Z = float(fCoeff0 * Z + fCoeff1 * dst.Z);
		W = float(fCoeff0 * W + fCoeff1 * dst.W);
	}

	// Возвращает вектор направления
	inline void GetDirVector(newVector3f &v)
	{
		Normalize();
		v.X = 2.0f*(X*Z-W*Y);
		v.Y = 2.0f*(Y*Z+W*X);
		v.Z = 1.0f-2.0f*(X*X+Y*Y);
	}
	// Сконвертировать в углы Эйлера
	inline void ToEulerAngle(newEulerAngle &e)
	{
		double sqW = W*W;
		double sqX = X*X;
		double sqY = Y*Y;
		double sqZ = Z*Z;
		e.Roll  = (float) NEW_RADTODEG(atan(2.0 * (X*Y + Z*W)/(sqX - sqY - sqZ + sqW)));
		e.Pitch = (float) NEW_RADTODEG(atan(2.0 * (Y*Z + X*W)/(-sqX - sqY + sqZ + sqW)));
		e.Yaw   = (float) NEW_RADTODEG(asin(-2.0 * (X*Z - Y*W)));
	}
	inline void Apply (newVector3f &v)
	{
		newQuaternionf r ( v.X * W + v.Z * Y - v.Y * Z, 			
			v.Y * W + v.X * Z - v.Z * X, 			
			v.Z * W + v.Y * X - v.X * Y,
			v.X * X + v.Y * Y + v.Z * Z 
			);
		v.X = W * r.X + X * r.W + Y * r.Z - Z * r.Y;
		v.Y = W * r.Y + Y * r.W + Z * r.X - X * r.Z;
		v.Z = W * r.Z + Z * r.W + X * r.Y - Y * r.X;
	}
	
}; // Конец класса

// Сферико-линейная интерполяция
inline newQuaternionf Slerp(const newQuaternionf &src, const newQuaternionf &dst, float factor)
{
    double fAngle = acos(src.X * dst.X + src.Y * dst.Y + src.Z * dst.Z + src.W * dst.W);
    if ( NEW_FLOAT_EQ(fAngle,0))
        return src;
	if ( NEW_FLOAT_EQ( fAngle - NEW_PI ,0)) return src;
		NEW_LIMIT_RANGE(0,factor,1);

    double fInvSin = 1.0f/sin(fAngle);
    double fCoeff0 = sin((1.0f-factor)*fAngle)*fInvSin;
    double fCoeff1 = sin(factor*fAngle)*fInvSin;
	return newQuaternionf(
		float(fCoeff0 * src.X + fCoeff1 * dst.X),
		float(fCoeff0 * src.Y + fCoeff1 * dst.Y),
		float(fCoeff0 * src.Z + fCoeff1 * dst.Z),
		float(fCoeff0 * src.W + fCoeff1 * dst.W));
}

// Применение кватерниона к вектору
inline void newApplyQuaterion(newVector3f &v, const newQuaternionf &q)
{
	newQuaternionf r ( v.X * q.W + v.Z * q.Y - v.Y * q.Z, 			
							v.Y * q.W + v.X * q.Z - v.Z * q.X, 			
							v.Z * q.W + v.Y * q.X - v.X * q.Y,
							v.X * q.X + v.Y * q.Y + v.Z * q.Z 
						  );
	v.X = q.W * r.X + q.X * r.W + q.Y * r.Z - q.Z * r.Y;
	v.Y = q.W * r.Y + q.Y * r.W + q.Z * r.X - q.X * r.Z;
	v.Z = q.W * r.Z + q.Z * r.W + q.X * r.Y - q.Y * r.X;

}
