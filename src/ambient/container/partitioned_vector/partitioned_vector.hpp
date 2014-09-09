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

#ifndef AMBIENT_CONTAINER_PARTITIONED_VECTOR_PARTITIONED_VECTOR_HPP
#define AMBIENT_CONTAINER_PARTITIONED_VECTOR_PARTITIONED_VECTOR_HPP

#include "ambient/container/partitioned_vector/partitioned_vector.h"
#include "ambient/numeric/future.hpp"
#include "ambient/container/partitioned_vector/detail.hpp"

namespace ambient {
     
    template <class Vector, int IB>
    partitioned_vector<Vector, IB>::~partitioned_vector(){
        int n = this->data.size();
        for(int i = 0; i < n; i++) delete this->data[i];
    }
     
    template <class Vector, int IB>
    partitioned_vector<Vector,IB>::partitioned_vector()
    : length(0), nt(0)
    {
    }

    template <class Vector, int IB>
    partitioned_vector<Vector,IB>::partitioned_vector(Vector* a)
    : length(a->size()), nt(__a_ceil(a->size()/IB))
    {
        this->data.push_back(a);
    }

    template <class Vector, int IB>
    partitioned_vector<Vector,IB>::partitioned_vector(size_type length, value_type init_value)
    : length(length), nt(__a_ceil(length/IB))
    {
        int tail = __a_mod(length, IB);
        this->data.reserve(nt);

        for(int i = 1; i < nt; i++)
            this->data.push_back(new Vector(IB, init_value));
        this->data.push_back(new Vector(tail, init_value));
    }

    template <class Vector, int IB>
    partitioned_vector<Vector,IB>::partitioned_vector(const partitioned_vector& a)
    : length(a.length), nt(a.nt)
    {
        int nb = a.data.size();
        for(int k = 0; k < nb; k++) this->data.push_back(new Vector(a[k]));
    }
    
    template <class Vector, int IB>
    partitioned_vector<Vector,IB>& partitioned_vector<Vector,IB>::operator = (const partitioned_vector& rhs){
        partitioned_vector c(rhs);
        this->swap(c);
        return *this;
    }
        
    template<class Vector, int IB>
    template<class OtherVector, int OtherIB>
    partitioned_vector<Vector,IB>& partitioned_vector<Vector,IB>::operator = (const partitioned_vector<OtherVector,OtherIB>& rhs){
        // redo for OtherIB
        int b = rhs.data.size();
        for(int k = 0; k < b; k++)
            (*this).partition(k) = rhs.partition(k);
        return *this;
    }

    template<class Vector, int IB>
    typename partitioned_vector<Vector,IB>::size_type partitioned_vector<Vector,IB>::size() const {
        return this->length;
    }
        
    template<class Vector, int IB>
    bool partitioned_vector<Vector,IB>::empty() const {
        return (this->length == 0);
    }

    template<class Vector, int IB>
    void partitioned_vector<Vector,IB>::swap(partitioned_vector& r){
        std::swap(r.data,   this->data);
        std::swap(r.length, this->length);
        std::swap(r.nt,     this->nt);
    }

    template<class Vector, int IB>
    void partitioned_vector<Vector,IB>::resize(size_type n){
        if(length == n) return;
        partitioned_vector r(n);
        int nb_min = std::min(r.nt, nt);
        int tail = std::min(r.partition(nb_min-1).size(), this->partition(nb_min-1).size());
        
        for(int k = 0; k < nb_min; k++){
            r.partition(k) = a.partition(k);
        }
        r.partition(nb_min-1).resize(tail);
        swap(a, r);
    }
        
    template<class Vector, int IB>
    Vector& partitioned_vector<Vector,IB>::partition(size_type k){
        return *this->data[k];
    }

    template<class Vector, int IB>
    const Vector& partitioned_vector<Vector,IB>::partition(size_type k) const {
        return *this->data[k];
    }

    template<class Vector, int IB>
    Vector& partitioned_vector<Vector,IB>::locate(size_type i){
        return this->partition(i/IB);
    }

    template<class Vector, int IB>
    const Vector& partitioned_vector<Vector,IB>::locate(size_type i) const {
        return this->partition(i/IB);
    }

    template<class Vector, int IB>
    typename partitioned_vector<Vector,IB>::value_type& partitioned_vector<Vector,IB>::operator[] (size_type i){
        size_type part = this->data[0]->size();
        return ambient::load(*this->data[(int)(i/part)])[i % part];
    }

    template<class Vector, int IB>
    const typename partitioned_vector<Vector,IB>::value_type& partitioned_vector<Vector,IB>::operator[] (size_type i) const {
        size_type part = this->data[0]->size();
        return ambient::load(*this->data[(int)(i/part)])[i % part];
    }

    template<class Vector, int IB>
    size_t partitioned_vector<Vector,IB>::addr(size_type i) const {
        return i % IB;
    }

    template <class Vector, int IB>
    typename partitioned_vector<Vector,IB>::iterator partitioned_vector<Vector,IB>::begin(){
        return typename partitioned_vector<Vector,IB>::iterator(*this, 0);
    }
     
    template <class Vector, int IB>
    typename partitioned_vector<Vector,IB>::iterator partitioned_vector<Vector,IB>::end(){
        return typename partitioned_vector<Vector,IB>::iterator(*this, size());
    }

    template <class Vector, int IB>
    typename partitioned_vector<Vector,IB>::const_iterator partitioned_vector<Vector,IB>::cbegin() const {
        return typename partitioned_vector<Vector,IB>::const_iterator(*this, 0);
    }
     
    template <class Vector, int IB>
    typename partitioned_vector<Vector,IB>::const_iterator partitioned_vector<Vector,IB>::cend() const {
        return typename partitioned_vector<Vector,IB>::const_iterator(*this, size());
    }

}

#endif
