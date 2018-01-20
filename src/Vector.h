#ifndef __VECTOR_H__
#define __VECTOR_H__

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

#include "Types.h"

#include <math.h>

#pragma inline_depth( 55255 )

#pragma inline_recursion( on )

#pragma auto_inline( on )


#define __forceinline //__attribute__((always_inline))
#define inline __forceinline 



namespace new_vector
{

    template< class ta_a >

    class vecarg
    {
        const ta_a& Argv;
    public:
        inline vecarg( const ta_a& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return Argv.Evaluate( i ); }
    };

    template<> 

    class vecarg< const float >
    {
        const float& Argv;
    public:
        inline vecarg( const float& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return Argv; }
    };

    template<> 

    class vecarg< const int >
    {
        const int& Argv;
    public:
        inline vecarg( const int& A ) : Argv( A ) {}
        inline const float Evaluate( const int i ) const 
        { return (float)Argv; }
    };

///////////////////////////////////////////////////////////////////////////
// . . . 
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
// . . . 
///////////////////////////////////////////////////////////////////////////

    template< int ta_dimension, class T >
    struct base : public T
    {
        inline       float&  operator[]( const int i )       { return ((float*)this)[i]; } 
        inline const float   Evaluate  ( const int i ) const { return ((float*)this)[i]; }

        //////////////////////////////////////////////////////////////////
        // . . . 
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
                static inline 	void Assign( base<ta_dimension,T>& V, const ta& A ){}

		    static inline void Assign( base<ta_dimension,T>& V, const ta& A )
		    {
		        recurse<0,int>::Assign( V, A );
		    }

            };
*/

		    /*static inline void Assign( base<ta_dimension,T>& V, const ta& A ) //+++
		    {
		        recurse<0,int>::Assign( V, A );
		    }*/


        };

        template< class ta_type > inline
        const base<ta_dimension,T>& operator = ( const ta_type& A )
        {
//++++            assigment<ta_type>::Assign( *this, A );
            return *this;
        }
        
        //////////////////////////////////////////////////////////////////
        // . . . 
        //////////////////////////////////////////////////////////////////
        template< class ta, class tb >
        struct dot_prod
        {
			
            template< int I, class R >
            struct recurse
            {
                enum { COUNTER = I+1 };

		template< class taA, class tbA >
                static inline float Dot_Prod( const taA& A, const tbA& B ) 
                {
                    return A.Evaluate( I ) * B.Evaluate( I ) +
                        recurse<COUNTER,int>::Dot_Prod( A, B );
                }
            };
	/*		template<>
			struct recurse<ta_dimension,int>
            {
                static inline float Dot_Prod( const ta& A, const tb& B )
                {
                    return 0;
                }
            };

            static inline float Dot_Prod( const ta& A, const tb& B )
            {
                return recurse<0,int>::Dot_Prod( A, B );
            }   */
        };
		template< class ta_type > inline
			float Dot( const ta_type& A ) const
		{
			return dot_prod<base<ta_dimension,T>,ta_type>::Dot_Prod( *this, A );
		}

        //////////////////////////////////////////////////////////////////
        // . . . 
        //////////////////////////////////////////////////////////////////
        template< class ta>
        struct length2
        {
            template< int I, class R >
            struct recurse
            {
                enum { COUNTER = I+1 };

                static inline float Length2( const ta& A ) 
                {
                    return A.Evaluate( I ) * A.Evaluate( I ) +
                        recurse<COUNTER,int>::Length2( A );
                }
            };

  /*          template<> struct recurse<ta_dimension,int>
            {
                static inline float Length2( const ta& A)
                {
                    return 0;
                }
            };

            static inline float Length2( const ta& A)
            {
                return recurse<0,int>::Length2( A );
            }
*/
        };

//        template<> inline
        template<int iDummy> inline
        float Length() const
        {
		    iDummy = 0;
            return (float)sqrt( length2< base<ta_dimension,T> >::Length2( *this ));
		}
		
		///////////////////////////////////////////////////////////////////////////
		// . . . 
		///////////////////////////////////////////////////////////////////////////
		struct cross
		{
			template< class ta_a, class ta_b > inline static 
				const float Evaluate( const int i, const ta_a& A, const ta_b& B )
			{ return A.Evaluate((i+ta_dimension-2)%ta_dimension) * B.Evaluate((i+ta_dimension-1)%ta_dimension) - A.Evaluate((i+ta_dimension-1)%ta_dimension) * B.Evaluate((i+ta_dimension-2)%ta_dimension); }
		};
		


	};

	struct mulf
	{
		template< class ta_a, class ta_b > inline static 
			const float Evaluate( const int i, const ta_a& A, const ta_b& B )
		{ return A.Evaluate(i) * B.Evaluate(i); }
	};

	template< class ta_c1> inline 
		const vecexp_2< const ta_c1, const float, mulf > 
		operator * ( const ta_c1& Pa, const float& Pb )
	{
		return vecexp_2< const ta_c1, const float, mulf >( Pa, Pb );
	}

	struct divf
	{
		template< class ta_a, class ta_b > inline static 
			const float Evaluate( const int i, const ta_a& A, const ta_b& B )
		{ return A.Evaluate(i) / B.Evaluate(i); }
	};

	template< class ta_c1> inline 
		const vecexp_2< const ta_c1, const float, divf > 
		operator / ( const ta_c1& Pa, const float& Pb )
	{
		return vecexp_2< const ta_c1, const float, divf >( Pa, Pb );
	}

	template< class ta_c1, class ta_c2 > inline 
		const float
		operator | ( const ta_c1& Pa, const ta_c2& Pb )
	{
//		return ta_c1::dot_prod< const  ta_c1,	 const  ta_c2  >	::Dot_Prod( Pa, Pb );
	}

	///////////////////////////////////////////////////////////////////////////
	// . . . 
	///////////////////////////////////////////////////////////////////////////
#if 0
	template< class ta_c1, class ta_c2 > inline
		const vecexp_2< const ta_c1, const ta_c2 , sum /* ta_c2::cross */ sum >
		operator ^ ( const ta_c1& Pa, const ta_c2& Pb )
	{
		return vecexp_2< const ta_c1, const ta_c2, sum /* ta_c1::cross */ >( Pa, Pb );
	}
#endif /* (0) */

	///////////////////////////////////////////////////////////////////////////
	// . . . 
	///////////////////////////////////////////////////////////////////////////

	struct sum
	{
		template< class ta_a, class ta_b > inline static 
			const float Evaluate( const int i, const ta_a& A, const ta_b& B )
		{ return A.Evaluate(i) + B.Evaluate(i); }
	};

/////+++++
	template< class ta_c1, class ta_c2 > inline
		const vecexp_2< const ta_c1, const ta_c2 , sum /* ta_c2::cross */  >
		operator ^ ( const ta_c1& Pa, const ta_c2& Pb )
	{
		return vecexp_2< const ta_c1, const ta_c2, sum /* ta_c1::cross */ >( Pa, Pb );
	}
/////+++++

	template< class ta_c1, class ta_c2 > inline 
		const vecexp_2< const ta_c1, const ta_c2, sum > 
		operator + ( const ta_c1& Pa, const ta_c2& Pb )
	{
		return vecexp_2< const ta_c1, const ta_c2, sum >( Pa, Pb );
	}

	///////////////////////////////////////////////////////////////////////////
	// . . . 
	///////////////////////////////////////////////////////////////////////////

	struct raz
	{
		template< class ta_a, class ta_b > inline static 
			const float Evaluate( const int i, const ta_a& A, const ta_b& B )
		{ return A.Evaluate(i) - B.Evaluate(i); }
	};

	template< class ta_c1, class ta_c2 > inline 
		const vecexp_2< const ta_c1, const ta_c2, raz > 
		operator - ( const ta_c1& Pa, const ta_c2& Pb )
	{
		return vecexp_2< const ta_c1, const ta_c2, raz >( Pa, Pb );
	}


	struct desc_xyzw_strq
	{
		float X, Y, Z, W;
	};

	struct desc_xyz_str
	{
		float X, Y, Z;
	};

	struct desc_xy_st
	{
		float X, Y;
	};

};


struct newVector2f : public new_vector::base< 2, new_vector::desc_xy_st >
{
    typedef new_vector::base< 2, new_vector::desc_xy_st > base;

	inline  newVector2f()
    { /*X = 0; Y = 0;*/ }

    inline  newVector2f( const float x, const float y )
    { X = x; Y = y; }

    inline  void Set( const float x, const float y )
    { X = x; Y = y; }

	inline void Normalize()
	{
//+++		float norm = Length();
		float norm = 1.1;
		if (norm) *this /= norm;
		else this->Set(0,0);
	}

	inline BOOL IsZero()
	{
		if (!NEW_FLOAT_EQ(X,0)) return FALSE;
		if (!NEW_FLOAT_EQ(Y,0)) return FALSE;
		return TRUE;
	}

    template< class ta_type > inline
    newVector2f ( const ta_type& A )
    { base::operator = ( A ); }

    template< class ta_type > inline
    newVector2f& operator = ( const ta_type& A )
    { base::operator = ( A ); return *this; }
	template< class ta_type > inline
    newVector2f& operator += ( const ta_type& A )
    { base::operator = ( *this + A ); return *this; }
	template< class ta_type > inline
    newVector2f& operator -= ( const ta_type& A )
    { base::operator = ( *this - A ); return *this; }
	template< class ta_type > inline
    newVector2f& operator *= ( const ta_type& A )
    { base::operator = ( *this * A ); return *this; }
	template< class ta_type > inline
	newVector2f& operator /= ( const ta_type& A )
    { base::operator = ( *this / A ); return *this; }

	operator const float*() const {return &X;}
};

struct newVector3f : public new_vector::base< 3, new_vector::desc_xyz_str>
{
    typedef new_vector::base< 3, new_vector::desc_xyz_str > base;

    inline  newVector3f()
    { /*X = 0; Y = 0; Z = 0;*/ }

	inline  newVector3f( const float x, const float y, const float z)
    { X = x; Y = y; Z = z; }

	inline  void Set( const float x, const float y, const float z)
    { X = x; Y = y; Z = z; }

    template< class ta_type > inline
    newVector3f ( const ta_type& A )
    { base::operator = ( A ); }

    template< class ta_type > inline
    newVector3f& operator = ( const ta_type& A )
    { base::operator = ( A ); return *this; }
	template< class ta_type > inline
    newVector3f& operator += ( const ta_type& A )
    { base::operator = ( *this + A ); return *this; }
	template< class ta_type > inline
    newVector3f& operator -= ( const ta_type& A )
    { base::operator = ( *this - A ); return *this; }
	template< class ta_type > inline
    newVector3f& operator *= ( const ta_type& A )
    { base::operator = ( *this * A ); return *this; }
	template< class ta_type > inline
	newVector3f& operator /= ( const ta_type& A )
    { base::operator = ( *this / A ); return *this; }

	inline void Normalize()
	{
//+++		float norm = Length();
		float norm = 1.1;
		if (norm) *this /= norm;
		else this->Set(0,0,0);
	}

	inline BOOL IsZero()
	{
		if (!NEW_FLOAT_EQ(X,0)) return FALSE;
		if (!NEW_FLOAT_EQ(Y,0)) return FALSE;
		if (!NEW_FLOAT_EQ(Z,0)) return FALSE;
		return TRUE;
	}

	inline void RotateX(float angle)
	{
		float oy=Y,oz=Z;
		angle=NEW_DEGTORAD(angle);
		Y=(float)((oy * cos(angle) )+(oz * (-sin(angle) ) ));
		Z=(float)((oy * sin(angle) )+(oz * (cos(angle) ) ));
	}

	inline void RotateY(float angle)
	{
		float ox=X,oz=Z;
		angle=NEW_DEGTORAD(angle);
		X=(float)((ox * cos(angle) )+(oz * (-sin(angle) ) ));
		Z=(float)((ox * sin(angle) )+(oz * (cos(angle) ) ));
	}

	inline void RotateZ(float angle)
	{
		float ox=Y,oy=Y;
		angle=NEW_DEGTORAD(angle);
		X=(float)((ox * cos(angle) )+(oy * (-sin(angle) ) ));
		Y=(float)((ox * sin(angle) )+(oy * (cos(angle) ) ));
	}

	operator const float*() const {return &X;}
};

struct newVector4f : public new_vector::base< 4, new_vector::desc_xyzw_strq>
{
    typedef new_vector::base< 4, new_vector::desc_xyzw_strq > base;

    inline  newVector4f()
    { /*X = 0; Y = 0; Z = 0; W=0;*/}

	inline  newVector4f( const float x, const float y, const float z, const float w)
    { X = x; Y = y; Z = z; W=w;}

	inline  void Set( const float x, const float y, const float z, const float w)
    { X = x; Y = y; Z = z; W=w;}

	inline void Normalize()
	{
//+++		float norm = Length();
		float norm = 1.1;
		if (norm) *this /= norm;
		else this->Set(0,0,0,0);
	}

    template< class ta_type > inline
    newVector4f ( const ta_type& A )
    { base::operator = ( A ); }

    template< class ta_type > inline
    newVector4f& operator = ( const ta_type& A )
    { base::operator = ( A ); return *this; }
	template< class ta_type > inline
    newVector4f& operator += ( const ta_type& A )
    { base::operator = ( *this + A ); return *this; }
	template< class ta_type > inline
    newVector4f& operator -= ( const ta_type& A )
    { base::operator = ( *this - A ); return *this; }
	template< class ta_type > inline
    newVector4f& operator *= ( const ta_type& A )
    { base::operator = ( *this * A ); return *this; }
	template< class ta_type > inline
	newVector4f& operator /= ( const ta_type& A )
    { base::operator = ( *this / A ); return *this; }

	inline BOOL IsZero()
	{
		if (!NEW_FLOAT_EQ(X,0)) return FALSE;
		if (!NEW_FLOAT_EQ(Y,0)) return FALSE;
		if (!NEW_FLOAT_EQ(Z,0)) return FALSE;
		if (!NEW_FLOAT_EQ(W,0)) return FALSE;
		return TRUE;
	}

	inline void RotateX(float angle)
	{
		float oy=Y,oz=Z;
		angle=NEW_DEGTORAD(angle);
		Y=(float)((oy * cos(angle) )+(oz * (-sin(angle) ) ));
		Z=(float)((oy * sin(angle) )+(oz * (cos(angle) ) ));
	}

	inline void RotateY(float angle)
	{
		float ox=X,oz=Z;
		angle=NEW_DEGTORAD(angle);
		X=(float)((ox * cos(angle) )+(oz * (-sin(angle) ) ));
		Z=(float)((ox * sin(angle) )+(oz * (cos(angle) ) ));
	}

	inline void RotateZ(float angle)
	{
		float ox=Y,oy=Y;
		angle=NEW_DEGTORAD(angle);
		X=(float)((ox * cos(angle) )+(oy * (-sin(angle) ) ));
		Y=(float)((ox * sin(angle) )+(oy * (cos(angle) ) ));
	}

	operator const float*() const {return &X;}
};

#endif /* ifndef-define include guard */
