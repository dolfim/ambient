/*
 * Copyright Institute for Theoretical Physics, ETH Zurich 2015.
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

#ifndef AMBIENT_CONTAINER_NUMERIC_DIAGONAL_MATRIX_HPP
#define AMBIENT_CONTAINER_NUMERIC_DIAGONAL_MATRIX_HPP
#include "ambient/container/numeric/matrix/diagonal_matrix.h" 

namespace ambient { inline namespace numeric {

    #define size_type   typename diagonal_matrix<T>::size_type
    #define value_type  typename diagonal_matrix<T>::value_type

    template<typename T>
    inline diagonal_matrix<T>::diagonal_matrix(size_t rows, size_t cols, value_type init)
    : data_(rows,1,init)
    {
    }

    template<typename T>
    inline size_type diagonal_matrix<T>::num_rows() const {
        return this->data_.num_rows();
    }

    template<typename T>
    inline size_type diagonal_matrix<T>::num_cols() const {
        return this->num_rows();
    }

    template<typename T>
    inline const value_type& diagonal_matrix<T>::operator[](size_t i) const {
        return ambient::load(this->data_)(i,0);
    }

    template<typename T>
    inline value_type& diagonal_matrix<T>::operator[](size_t i){
        return ambient::load(this->data_)(i,0);
    }

    template<typename T>
    inline const value_type& diagonal_matrix<T>::operator()(size_t i, size_t j) const {
        assert(i == j);
        return ambient::load(this->data_)(i,0);
    }

    template<typename T>
    inline value_type& diagonal_matrix<T>:: operator()(size_t i, size_t j){
        assert(i == j);
        return ambient::load(this->data_)(i,0);
    }

    template<typename T>
    inline size_type diagonal_matrix<T>::size() const {
        return this->data_.num_rows();
    }

    template<typename T>
    inline diagonal_matrix<T>& diagonal_matrix<T>::locate(size_t i, size_t j){
        return *this;
    }

    template<typename T>
    inline const diagonal_matrix<T>& diagonal_matrix<T>::locate(size_t i, size_t j) const {
        return *this;
    }

    template<typename T>
    inline size_t diagonal_matrix<T>::addr(size_t i, size_t j) const {
        return i;
    }

    template<typename T> 
    inline void diagonal_matrix<T>::resize(size_t rows, size_t cols){
        assert(rows == cols);
        this->data_.resize(rows, 1);
    }

    template<typename T>
    inline const typename diagonal_matrix<T>::container& diagonal_matrix<T>::get_data() const {
        return this->data_;                          
    }

    template<typename T>
    inline typename diagonal_matrix<T>::container& diagonal_matrix<T>::get_data(){
        return this->data_;                          
    }

    // {{{ diagonal_matrix free functions
    template<typename T>
    inline size_type num_rows(const diagonal_matrix<T>& a){
        return a.num_rows();
    }

    template<typename T>
    inline size_type num_cols(const diagonal_matrix<T>& a){
        return a.num_cols();
    }

    template<typename T>
    inline diagonal_matrix<T> exp(diagonal_matrix<T> a, const T& alfa = 1.){
        exp_inplace(a, alfa);
        return a;
    }

    template<typename T>
    inline void exp_inplace(diagonal_matrix<T>& a, const T& alfa = 1.){
        kernels::template exp_diagonal<T>(a, alfa);
    }

    template<typename T>
    inline diagonal_matrix<T> sqrt(diagonal_matrix<T> a){
        sqrt_inplace(a);
        return a;
    }

    template<typename T>
    inline void sqrt_inplace(diagonal_matrix<T>& a){
        kernels::template sqrt_diagonal<T>(a);
    }

    template<typename T>
    inline std::ostream& operator<<(std::ostream& os, const diagonal_matrix<T>& a){
       os << a.data_ ; 
       return os;
    }

    template<typename T>
    inline void resize(diagonal_matrix<T>& a, size_t m, size_t n){
        a.resize(m, n);
    }
    // }}}

    #undef value_type
    #undef size_type

} }

#endif