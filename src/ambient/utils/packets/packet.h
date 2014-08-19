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

#ifndef AMBIENT_CHANNELS_MPI_PACKETS_INSTANCE
#define AMBIENT_CHANNELS_MPI_PACKETS_INSTANCE

namespace ambient { namespace channels { namespace mpi {

    class packet
    {
    public:
        void* data;
        MPI_Datatype mpi_t;
        int lifetime;
        const packet_t& type;
        const packet_t& get_t();
        MPI_Datatype get_mpi_t();
        int   get_t_code();
        const void* get(int field);
        size_t get_bound(size_t field);
        void* get_memory();
        bool  disposable();

        template<typename T>
        T get(int field){
            return *(T*)this->get(field);
        }

        void set(int field, const void* value);
        void set(int field, int value);
        packet(const packet_t& type, const void* memory);
        packet(const packet_t& type, void* memory, va_list& fields); // used in auxiliary.hpp
        packet(const packet& p);
        ~packet();
    };

    packet* pack(const packet_t& type, void* memory, ...);
    packet* unpack(const packet_t& type, void* memory);

} } }

#endif
