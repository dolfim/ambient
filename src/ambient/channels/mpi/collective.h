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

#ifndef AMBIENT_CHANNELS_MPI_COLLECTIVE
#define AMBIENT_CHANNELS_MPI_COLLECTIVE

namespace ambient { namespace channels { namespace mpi {

    template<typename T>
    class bcast {
        typedef memory::cpu::instr_bulk::allocator<int> allocator_type;
    public:
        void dispatch();
        bcast(T& o, rank_t root) : object(o), root(root), self(0) {}
    private:
        template<class C> friend class collective;
        T& object;
        std::vector<int,allocator_type> tags;
        rank_t root;
        int self;
        int size;
        rank_t* list;
        request impl; 
        guard_once once;
    };

    template<class T> class collective {};

    template<>
    class collective<typename channel::block_type> : public bcast<typename channel::block_type>, 
                                                     public memory::cpu::use_bulk_new<collective<typename channel::block_type> > {
        typedef memory::cpu::instr_bulk::allocator<int> allocator_type;
    public:
        collective(typename channel::block_type& r, rank_t root);
        void append(rank_t rank);
        bool involved();
        bool test();
    private:
        std::vector<bool,allocator_type> states;
        std::vector<rank_t,allocator_type> tree;
    };

    template<>
    class collective<typename channel::scalar_type> : public bcast<typename channel::scalar_type>, 
                                                      public memory::cpu::use_bulk_new<collective<typename channel::scalar_type> > {
    public:
        collective(typename channel::scalar_type& v, rank_t root);
        bool test();
    };

} } }

#endif
