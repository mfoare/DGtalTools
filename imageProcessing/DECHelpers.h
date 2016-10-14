/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file
 * @author Jacques-Olivier Lachaud (\c jacques-olivier.lachaud@univ-savoie.fr )
 * Laboratory of Mathematics (CNRS, UMR 5127), University of Savoie, France
 *
 * @date 2016/10/12
 *
 * Header file for module DECHelpers.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(DECHelpers_RECURSES)
#error Recursive header files inclusion detected in DECHelpers.h
#else // defined(DECHelpers_RECURSES)
/** Prevents recursive inclusion of headers. */
#define DECHelpers_RECURSES

#if !defined DECHelpers_h
/** Prevents repeated inclusion of headers. */
#define DECHelpers_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include "DGtal/base/Common.h"
#include "DGtal/dec/DiscreteExteriorCalculus.h"

//////////////////////////////////////////////////////////////////////////////

namespace DGtal {
  namespace functions {
    namespace dec {
      
      /**
      * Builds a diagonal linear operator from a k-form. These
      * operators arise naturally when differentiating with respect to
      * another variable (e.g. d/dx (vx)^t (vx) = diag(v^2) x).
      *
      * @param[in] kform any kform w.
      * @return the corresponding linear operator diag(w)
      */
      template <typename Calculus, DGtal::Dimension dim, DGtal::Duality duality> 
      DGtal::LinearOperator<Calculus, dim, duality, dim, duality> 
      diagonal(const DGtal::KForm<Calculus, dim, duality>& kform) 
      { 
        typedef DGtal::LinearOperator<Calculus,dim, duality, dim, duality> Operator; 
        typedef typename Calculus::LinearAlgebraBackend::Triplet           Triplet; 
        typedef typename Calculus::Index                                   Index; 
        typedef std::vector<Triplet>                                       Triplets;

        Triplets triplets;
        for (Index index=0; index<kform.length(); index++) 
          triplets.push_back(Triplet(index, index, kform.myContainer(index)));
        
        Operator op( kform.myCalculus );
        op.myContainer.setFromTriplets( triplets.begin(), triplets.end() );
        return op;
      }

      /**
      * Squares the given k-form.
      *
      * @param[in,out] kform any kform.
      */
      template <typename Calculus, DGtal::Dimension dim, DGtal::Duality duality> 
      void
      squares(DGtal::KForm<Calculus, dim, duality>& kform) 
      {
        kform.myContainer.array() = kform.myContainer.array().square();
      }
      
      /**
      * Builds a diagonal linear operator from a k-form and squares it. These
      * operators arise naturally when differentiating with respect to
      * another variable (e.g. d/dx (vx)^t (vx) = diag(v^2) x).
      *
      * @param[in] kform any kform v
      * @return the corresponding linear operator diag(v^2)
      */
      template <typename Calculus, DGtal::Dimension dim, DGtal::Duality duality> 
      DGtal::LinearOperator<Calculus, dim, duality, dim, duality> 
      squaredDiagonal(const DGtal::KForm<Calculus, dim, duality>& kform) 
      {
        auto v2 = kform;
        squares( v2 );
        return diagonal( v2 );
      }

      /**
      * Considers an image \a image to have pixels of size \a
      * pixel_size x \a pixel_size, and writes the value \a val at the
      * specified pixel position \a pt.
      *
      * @param[in,out] any image of sufficient size.
      * @param pt a pixel coordinate (which is multiplied by \a pixel_size within).
      * @param val the value to write in \a pixel_size x \a pixel_size pixels.
      * @param pixel_size the chosen pixel_size (when 1, this is the normal setValue of an image).
      */
      template <typename Image>
      void writePixel( Image& image, typename Image::Point pt, typename Image::Value val,
                       int pixel_size = 1 )
      {
        typedef typename Image::Point      Point;
        typedef typename Point::Coordinate Coordinate;
        pt *= pixel_size;
        for ( int y = 0; y < pixel_size; y++ )
          for ( int x = 0; x < pixel_size; x++ )
            {
              Point q( (Coordinate) x, (Coordinate) y );
              image.setValue( pt + q, val );
            }
      }

      /**
      * Considers an image \a image to have pixels of size \a
      * pixel_size x \a pixel_size, and writes the value \a val at the
      * specified linel position \a pt.
      *
      * @param[in,out] any image of sufficient size.
      * @param pt a linel Khalimsky coordinates.
      * @param val the value to write in \a pixel_size x 1 pixels (if horizontal) or 1 x \a pixel_size pixels (if vertical).
      * @param pixel_size the chosen pixel_size (when 1, this is the normal setValue of an image).
      */
      template <typename Image>
      void writePrimalLinel( Image& image, typename Image::Point pt, typename Image::Value val,
                       int pixel_size = 1 )
      {
        typedef typename Image::Point      Point;
        typedef typename Point::Coordinate Coordinate;
        int pixel_size_x = NumberTraits<Coordinate>::even( pt[ 0 ] ) ? 1 : pixel_size;
        int pixel_size_y = NumberTraits<Coordinate>::even( pt[ 1 ] ) ? 1 : pixel_size;
        pt /= 2;
        pt *= pixel_size;
        for ( int y = 0; y < pixel_size_y; y++ )
          for ( int x = 0; x < pixel_size_x; x++ )
            {
              Point q( (Coordinate) x, (Coordinate) y );
              image.setValue( pt + q, val );
            }
      }
      
      /**
      * Considers an image \a image to have pixels of size \a
      * pixel_size x \a pixel_size, and writes the value \a val at the
      * specified linel position \a pt.
      *
      * @param[in,out] any image of sufficient size.
      * @param pt a linel Khalimsky coordinates.
      * @param val the value to write in \a pixel_size x 1 pixels (if horizontal) or 1 x \a pixel_size pixels (if vertical).
      * @param pixel_size the chosen pixel_size (when 1, this is the normal setValue of an image).
      */
      template <typename Image>
      void writeDualLinel( Image& image, typename Image::Point pt, typename Image::Value val,
                           int pixel_size = 1 )
      {
        typedef typename Image::Point      Point;
        typedef typename Point::Coordinate Coordinate;
        int pixel_size_x = NumberTraits<Coordinate>::even( pt[ 0 ] ) ? 0 : pixel_size-1;
        int pixel_size_y = NumberTraits<Coordinate>::even( pt[ 1 ] ) ? 0 : pixel_size-1;
        pt /= 2;
        pt *= pixel_size;
        for ( int y = pixel_size_y; y < pixel_size; y++ )
          for ( int x = pixel_size_x; x < pixel_size; x++ )
            {
              Point q( (Coordinate) x, (Coordinate) y );
              image.setValue( pt + q, val );
            }
      }
      
      template <typename Calculus, typename Image>
      void dualForm2ToImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm2& u, 
        Image& image,
        std::function< typename Image::Value( double ) > functor,
        double cut_low = 0.0, double cut_up = 1.0, int pixel_size = 1 )
      {
        typedef typename Calculus::Index  Index;
        typedef typename Calculus::SCell  SCell;
        typedef typename Calculus::Scalar Scalar;
        typedef typename Calculus::KSpace KSpace;
        typedef typename KSpace::Point    Point;
        typedef typename KSpace::Integer  Integer;
        double min_u = NumberTraits<Scalar>::castToDouble( u.myContainer[ 0 ] );
        double max_u = min_u;
        for ( Index index = 0; index < u.myContainer.rows(); index++)
          {
            double v = NumberTraits<Scalar>::castToDouble( u.myContainer[ index ] );
            min_u = std::min( min_u, v );
            max_u = std::max( max_u, v );
          }
        if ( min_u < cut_low ) min_u = cut_low;
        if ( max_u > cut_up  ) max_u = cut_up;
        for ( Index index = 0; index < u.myContainer.rows(); index++)
          {
            SCell cell = u.getSCell( index );
            double v = NumberTraits<Scalar>::castToDouble( u.myContainer[ index ] );
            double w = std::min( cut_up, std::max( cut_low, v ) );
            if ( min_u != max_u ) w = ( w - min_u ) / ( max_u - min_u );
            writePixel( image, calculus.myKSpace.sCoords( cell ), functor( w ), pixel_size );
          }
      }

      template <typename Calculus, typename Image>
      void dualForm1ToImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm1& v, 
        Image& image,
        std::function< typename Image::Value( double ) > functor,
        std::function< bool ( double ) > predicate,
        double cut_low = 0.0, double cut_up = 1.0, int pixel_size = 1 )
      {
        typedef typename Calculus::Index  Index;
        typedef typename Calculus::SCell  SCell;
        typedef typename Calculus::Scalar Scalar;
        typedef typename Calculus::KSpace KSpace;
        typedef typename KSpace::Point    Point;
        typedef typename KSpace::Integer  Integer;
        double min_v = NumberTraits<Scalar>::castToDouble( v.myContainer[ 0 ] );
        double max_v = min_v;
        for ( Index index = 0; index < v.myContainer.rows(); index++)
          {
            double w = NumberTraits<Scalar>::castToDouble( v.myContainer[ index ] );
            min_v = std::min( min_v, w );
            max_v = std::max( max_v, w );
          }
        if ( min_v < cut_low ) min_v = cut_low;
        if ( max_v > cut_up  ) max_v = cut_up;
        for ( Index index = 0; index < v.myContainer.rows(); index++)
          {
            SCell cell = v.getSCell( index );
            double u = NumberTraits<Scalar>::castToDouble( v.myContainer[ index ] );
            if ( ! predicate( u ) ) continue; 
            double w = std::min( cut_up, std::max( cut_low, u ) );
            if ( min_v != max_v ) w = ( w - min_v ) / ( max_v - min_v );
            Point kpt = calculus.myKSpace.sKCoords( cell );
            writeDualLinel  ( image, kpt, functor( w ), pixel_size );
          }
      }

      template <typename Calculus, typename Image>
      void threeDualForms2ToImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm2& u0, 
        const typename Calculus::DualForm2& u1, 
        const typename Calculus::DualForm2& u2, 
        Image& image,
        std::function< typename Image::Value( double, double, double ) > functor,
        double cut_low = 0.0, double cut_up = 1.0, int pixel_size = 1 )
      {
        typedef typename Calculus::Index  Index;
        typedef typename Calculus::SCell  SCell;
        typedef typename Calculus::Scalar Scalar;
        double min_u = NumberTraits<Scalar>::castToDouble( u0.myContainer[ 0 ] );
        double max_u = min_u;
        for ( Index index = 0; index < u0.myContainer.rows(); index++)
          {
            double v = NumberTraits<Scalar>::castToDouble( u0.myContainer[ index ] );
            min_u = std::min( min_u, v );
            max_u = std::max( max_u, v );
          }
        for ( Index index = 0; index < u1.myContainer.rows(); index++)
          {
            double v = NumberTraits<Scalar>::castToDouble( u1.myContainer[ index ] );
            min_u = std::min( min_u, v );
            max_u = std::max( max_u, v );
          }
        for ( Index index = 0; index < u2.myContainer.rows(); index++)
          {
            double v = NumberTraits<Scalar>::castToDouble( u2.myContainer[ index ] );
            min_u = std::min( min_u, v );
            max_u = std::max( max_u, v );
          }
        if ( min_u < cut_low ) min_u = cut_low;
        if ( max_u > cut_up  ) max_u = cut_up;
        for ( Index index = 0; index < u0.myContainer.rows(); index++)
          {
            SCell cell = u0.getSCell( index );
            double v0 = NumberTraits<Scalar>::castToDouble( u0.myContainer[ index ] );
            double w0 = std::min( cut_up, std::max( cut_low, v0 ) );
            if ( min_u != max_u ) w0 = ( w0 - min_u ) / ( max_u - min_u );
            double v1 = NumberTraits<Scalar>::castToDouble( u1.myContainer[ index ] );
            double w1 = std::min( cut_up, std::max( cut_low, v1 ) );
            if ( min_u != max_u ) w1 = ( w1 - min_u ) / ( max_u - min_u );
            double v2 = NumberTraits<Scalar>::castToDouble( u2.myContainer[ index ] );
            double w2 = std::min( cut_up, std::max( cut_low, v2 ) );
            if ( min_u != max_u ) w2 = ( w2 - min_u ) / ( max_u - min_u );
            writePixel( image, calculus.myKSpace.sCoords( cell ), functor( w0, w1, w2 ), pixel_size );
          }
      }
      
      /**
      * Standard method to output a 0-form into a grey-level image.
      */
      template <typename Calculus, typename Image>
      void dualForm2ToGreyLevelImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm2& u, 
        Image& image,
        double cut_low = 0.0, double cut_up = 1.0,
        int pixel_size = 1 )
      {
        dualForm2ToImage( calculus, u, image,
                            [] ( double x ) { return (unsigned char) ( round( x * 255.0 ) ); },
                            cut_low, cut_up, pixel_size );
      }

      /**
      * Standard method to output a 1-form into a grey-level image.
      */
      template <typename Calculus, typename Image>
      void dualForm1ToGreyLevelImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm1& v, 
        Image& image,
        double cut_low = 0.0, double cut_up = 1.0,
        int pixel_size = 1 )
      {
        dualForm1ToImage( calculus, v, image,
                            [] ( double x ) { return (unsigned char) ( round( x * 255.0 ) ); },
                            [] ( double x ) { return x < 0.5; },
                            cut_low, cut_up, pixel_size );
      }

      /**
      * Standard method to output a 1-form into a color image.
      */
      template <typename Calculus, typename Image>
      void dualForm1ToRGBColorImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm1& v, 
        Image& image, Color color,
        double cut_low = 0.0, double cut_up = 1.0,
        int pixel_size = 1 )
      {
        dualForm1ToImage( calculus, v, image,
                          [color] ( double x ) { return color; },
                          [] ( double x ) { return x < 0.5; },
                          cut_low, cut_up, pixel_size );
      }

      /**
      * Standard method to output three 0-forms into a RGB Color image.
      */
      template <typename Calculus, typename Image>
      void threeDualForms2ToRGBColorImage
      ( const Calculus& calculus, 
        const typename Calculus::DualForm2& u0, 
        const typename Calculus::DualForm2& u1, 
        const typename Calculus::DualForm2& u2, 
        Image& image,
        double cut_low = 0.0, double cut_up = 1.0,
        int pixel_size = 1 )
      {
        threeDualForms2ToImage
          ( calculus, u0, u1, u2, image,
            [] ( double r, double g, double b )
            { return Color( (unsigned char) ( round( r * 255.0 ) ),
                            (unsigned char) ( round( g * 255.0 ) ),
                            (unsigned char) ( round( b * 255.0 ) ) ); },
            cut_low, cut_up, pixel_size );
      }

    } // namespace dec
  } // namespace functions
} // namespace DGtal

///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.

//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined DECHelpers_h

#undef DECHelpers_RECURSES
#endif // else defined(DECHelpers_RECURSES)
