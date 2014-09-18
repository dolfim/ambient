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

#ifndef AMBIENT_CONTAINER_NUMERIC_DIAGONAL_MATRIX_H
#define AMBIENT_CONTAINER_NUMERIC_DIAGONAL_MATRIX_H

namespace ambient { namespace numeric {
   
    template<typename T, class Allocator>
    class matrix;

    template<typename T>
    class diagonal_matrix
    {
    public:
        typedef matrix<T> container;
        typedef typename container::difference_type difference_type;
        typedef typename container::value_type value_type;
        typedef typename container::scalar_type scalar_type;
        typedef typename container::real_type real_type;
        typedef typename container::size_type size_type;
        
        diagonal_matrix(size_t rows, size_t cols, value_type init = value_type());
        size_type num_rows() const;
        size_type num_cols() const;
        const value_type& operator[](size_t i) const;
        value_type& operator[](size_t i); 
        const value_type& operator()(size_t i, size_t j) const;
        value_type& operator()(size_t i, size_t j);
        diagonal_matrix& locate(size_t i, size_t j);
        const diagonal_matrix& locate(size_t i, size_t j) const;
        size_t addr(size_t i, size_t j) const;
        void resize(size_t m, size_t n);
        template< class T1 > 
        friend std::ostream & operator<<(std::ostream& os, const diagonal_matrix<T1>& a);
        const container& get_data() const; 
        container& get_data();    
        size_type size() const;
   private:
        container data_;
    };

} }

#endif
