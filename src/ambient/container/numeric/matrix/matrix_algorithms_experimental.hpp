/*
 * Copyright Institute for Theoretical Physics, ETH Zurich 2014.
 * Distributed under the Boost Software License, Version 1.0.
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef AMBIENT_CONTAINER_NUMERIC_MATRIX_ALGORITHMS_EXPERIMENTAL
#define AMBIENT_CONTAINER_NUMERIC_MATRIX_ALGORITHMS_EXPERIMENTAL

#include "ambient/container/numeric/matrix/matrix.h"
#include "ambient/container/numeric/matrix/detail/kernels.hpp"
#include "ambient/container/numeric/matrix/detail/experimental.hpp"
#include "ambient/container/numeric/matrix/diagonal_matrix.hpp"

#define size_type       typename matrix<T,A>::size_type
#define real_type       typename matrix<T,A>::real_type
#define scalar_type     typename matrix<T,A>::scalar_type
#define difference_type typename matrix<T,A>::difference_type

namespace ambient { namespace numeric {

    template<int alfa, int beta, class MatrixViewA, class MatrixViewB, class MatrixViewC>
    inline void gemv(const MatrixViewA& a, size_t aoffset, 
                     const MatrixViewB& b, size_t boffset, 
                           MatrixViewC& c, size_t coffset, 
                           size_t m, size_t n)
    {
        kernels::gemv<int_type<alfa>,int_type<beta>,MatrixViewA,MatrixViewB,MatrixViewC>(a, aoffset, 
                                                                                         b, boffset, 
                                                                                         c, coffset, 
                                                                                         m, n); 
    }

    template<int ADD, class MA, class MB, class MC, class MF>
    inline void gemv_scale(const MA& a, size_t aoffset, 
                           const MB& b, size_t boffset, 
                                 MC& c, size_t coffset, 
                           const MF& f, size_t foffset, 
                           size_t m, size_t n)
    {
        kernels::gemv_scale<int_type<ADD>,MA,MB,MC,MF>(a, aoffset, 
                                                       b, boffset, 
                                                       c, coffset, 
                                                       f, foffset, 
                                                       m, n); 
    }

    template<typename T, class A>
    inline void gbbrd(matrix<T,A>& a, diagonal_matrix<T>& d, diagonal_matrix<T>& e, matrix<T,A>& u, matrix<T,A>& v){
        kernels::gbbrd<T>(a, d, e, u, v);
    }

    template<typename T, class A>
    inline void gebrd(matrix<T,A>& a, diagonal_matrix<T>& d, diagonal_matrix<T>& e, matrix<T,A>& u, matrix<T,A>& v){
        kernels::gebrd<T>(a, d, e, u, v);
    }

    template<typename T, class A>
    inline void bdsqr(diagonal_matrix<T>& d, diagonal_matrix<T>& e, matrix<T,A>& u, matrix<T,A>& v){
        kernels::bdsqr<T>(d, e, u, v);
    }

    template<typename T, class A>
    inline void gebd2(matrix<T,A>& a, diagonal_matrix<T>& d, diagonal_matrix<T>& e, diagonal_matrix<T>& tq, diagonal_matrix<T>& tp){
        kernels::gebd2<T>(a, d, e, tq, tp);
    }

    template<PLASMA_enum TR, typename T, class A>
    inline void larfg(matrix<T,A>& a, diagonal_matrix<T>& t, diagonal_matrix<T>& d, size_t k){
        kernels::larfg<T,trans_type<TR> >(a, t, d, k);
    }

    template<typename T, class A>
    inline void labrd_update_col(matrix<T,A>& say, const matrix<T,A>& sax, 
                                 matrix<T,A>& sy, const matrix<T,A>& sx, 
                                 diagonal_matrix<T>& tq, 
                                 diagonal_matrix<T>& d, 
                                 int i)
    {
        kernels::labrd_update_col<T>(say, sax, sy, sx, tq, d, i);
    }

    template<int IB, typename T, class A>
    inline void labrd_reduce_col(matrix<T,A>& say, const matrix<T,A>& sax, 
                                 matrix<T,A>& sy, const matrix<T,A>& sx, 
                                 int i)
    {
        kernels::labrd_reduce_col<T, int_type<IB> >(say, sax, sy, sx, i);
    }

    template<typename T, class A>
    inline void labrd_update_row(const matrix<T,A>& say, matrix<T,A>& sax, 
                                 const matrix<T,A>& sy, matrix<T,A>& sx, 
                                 diagonal_matrix<T>& tp, 
                                 diagonal_matrix<T>& e, 
                                 int i)
    {
        kernels::labrd_update_row<T>(say, sax, sy, sx, tp, e, i);
    }

    template<int IB, typename T, class A>
    inline void labrd_reduce_row(const matrix<T,A>& say, matrix<T,A>& sax, 
                                 const matrix<T,A>& sy, matrix<T,A>& sx, 
                                 int i)
    {
        kernels::labrd_reduce_row<T, int_type<IB> >(say, sax, sy, sx, i);
    }

    template<PLASMA_enum UL, size_t OFF, typename T, class A>
    inline void laset2(matrix<T,A>& a, const T& alfa = 0.0){
        kernels::laset2<T,ul_type<UL>,int_type<OFF> >(a, alfa);
    }

    template<PLASMA_enum UL, int IB, typename T, class A>
    inline void copy_band(const matrix<T,A>& src, matrix<T,A>& dst, size_t dj){
        kernels::copy_band<T,ul_type<UL>, int_type<IB> >(src, dst, dj);
    }

    template <int alfa, typename T, class A>
    inline void add_vectors(matrix<T,A>& lhs, size_t loffset, const matrix<T,A>& rhs, size_t roffset, size_t size){ 
        kernels::add_vectors<int_type<alfa>, T>(lhs, loffset, rhs, roffset, size); 
    }

    template<typename T, class A>
    inline void sqrt_inplace(matrix<T,A>& a){
        kernels::template sqrt_inplace<T>(a);
    }

    template<typename T, class A>
    inline void norm_vector(const matrix<T,A>& a, matrix<T,A>& b){ 
        kernels::template norm_vector<T>(a, b);
    }

    template<typename T, class A> 
    inline double max_vector(const matrix<T,A>& a){ 
        ambient::numeric::future<double> ret(0.0);
        kernels::template max_vector<T>(a, ret);
        return (double)ret; 
    }

    template<class Matrix>
    inline void fill_gaussian(Matrix& a){
        kernels::template init_gaussian<typename Matrix::value_type>(a);
    }

} }

#undef size_type
#undef real_type
#undef scalar_type
#undef difference_type
#endif
