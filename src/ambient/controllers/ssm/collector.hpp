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

namespace ambient { namespace memory {

    using ambient::models::ssm::history;
    using ambient::models::ssm::revision;
    using ambient::models::ssm::transformable;
    using ambient::models::ssm::sizeof_transformable;

    inline void collector::reserve(size_t n){
        this->rev.reserve(n);
        this->str.reserve(n);
    }

    inline void collector::push_back(transformable* o){
        this->raw.push_back(o);
    }

    inline void collector::push_back(revision* o){
        if(!o->valid()) o->spec.weaken();
        o->spec.crefs--;
        if(!o->referenced()){ // squeeze
            if(o->valid() && !o->locked() && o->spec.region == ambient::region_t::standard){
                ambient::pool::free(o->data, o->spec); // artifacts or last one
                o->spec.region = ambient::region_t::delegated;
            }
            this->rev.push_back(o);
        }
    }

    inline void collector::push_back(history* o){
        this->push_back(o->current);
        this->str.push_back(o);
    }

    inline void collector::delete_ptr::operator()( revision* r ) const {
        if(r->valid() && r->spec.region == ambient::region_t::standard){
            ambient::pool::free(r->data, r->spec); // artifacts
            r->spec.region = ambient::region_t::delegated;
        }
        delete r; 
    }

    inline void collector::delete_ptr::operator()( history* e ) const {
        delete e;
    }

    inline void collector::delete_ptr::operator()( transformable* e ) const {
        ambient::pool::free<fixed,sizeof_transformable()>(e);
    } 

    inline void collector::clear(){
        std::for_each( rev.begin(), rev.end(), delete_ptr());
        std::for_each( str.begin(), str.end(), delete_ptr());
        std::for_each( raw.begin(), raw.end(), delete_ptr());
        rev.clear();
        str.clear();
        raw.clear();
    }

} }
