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

#ifndef AMBIENT_MEMORY_REGION
#define AMBIENT_MEMORY_REGION

namespace ambient { namespace memory {

    constexpr size_t paged(size_t size, size_t page){  return page * (size_t)((size+page-1)/page); } // (page example: 4096)
    constexpr size_t aligned_64(size_t size){ return 64 * (size_t)((size+63)/64); }
    constexpr size_t aligned_8(size_t size){ return 8 * (size_t)((size+7)/8); }
    template<size_t S> constexpr size_t aligned_64(){ return 64 * (size_t)((S+63)/64); }

    template<size_t S, class Factory>
    class serial_region {
    public:
        serial_region(){
            this->buffer = NULL;
            this->iterator = (char*)this->buffer+S;
        }
        void realloc(){
            this->buffer = Factory::provide();
            this->iterator = (char*)this->buffer;
        }
        void* malloc(size_t sz){
            if(((size_t)iterator + sz - (size_t)this->buffer) >= S) realloc();
            void* m = (void*)iterator;
            iterator += aligned_64(sz);
            return m;
        }
        void reset(){
            this->iterator = (char*)this->buffer+S;
        }
    protected:
        void* buffer;
        char* iterator;
    };

    template<size_t S, class Factory>
    class private_region : public serial_region<S,Factory> {
    public:
        typedef serial_region<S,Factory> base;
        void* malloc(size_t sz){
            if(((size_t)this->iterator + sz - (size_t)this->buffer) >= S){
                this->buffer = pool.provide();
                this->iterator = (char*)this->buffer;
            }
            void* m = (void*)this->iterator;
            this->iterator += aligned_64(sz);
            return m;
        }
        void reset(){
            base::reset();
            pool.reset();
        }
    private:
        Factory pool;
    };

    template<size_t S, class Factory>
    class region : public serial_region<S,Factory> {
    public:
        typedef ambient::mutex mutex;
        typedef ambient::guard<mutex> guard;
        typedef serial_region<S,Factory> base;

        region() : count(0) {}
    private:
        void realloc(){
            if(this->count){
                Factory::collect(this->buffer, this->count);
                this->count = 0;
            }
            base::realloc();
        }
    public:
        void* malloc(size_t sz){
            guard g(this->mtx);
            if(((size_t)this->iterator + sz - (size_t)this->buffer) >= S) realloc();
            this->count++;
            void* m = (void*)this->iterator;
            this->iterator += aligned_64(sz);
            return m;
        }
        void reset(){
            base::reset();
            this->count = 0; 
        }
    private:
        long int count;
        mutex mtx;
    };

} }

#endif