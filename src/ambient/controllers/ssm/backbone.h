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

#ifndef AMBIENT_CONTROLLERS_SSM_BACKBONE
#define AMBIENT_CONTROLLERS_SSM_BACKBONE

namespace ambient {

    class backbone : public context {
        backbone(const backbone&) = delete;
        backbone& operator=(const backbone&) = delete;
        backbone(); 
    private:
        typedef typename context::controller_type controller_type;
        utils::funneled_io io_guard;
        actor_auto* base_actor;
        ambient::mutex mtx;
        int num_procs;
        int tag_ub;
        int sid;
    public:
       ~backbone();
        controller_type* provide_controller();
        void revoke_controller(controller_type* c);
        void sync();
        void info();
        int  generate_sid();
        bool tunable();
        void schedule();
        void intend_read(models::ssm::revision* o);
        void intend_write(models::ssm::revision* o);
        bool has_nested_actor();
        void pop_actor();
        void pop_scope();
        void push_actor(actor* s);
        void push_scope(scope* s);
        controller_type& get_controller();
        ambient::mutex& get_mutex();
        actor_auto& get_base_actor();
        actor& get_actor();
        scope& get_scope();
        int get_sid();
        int get_num_procs();
    public:
        template<class T>
        struct weak_instance {
            static backbone w;
        };
    };

    template<class T> backbone backbone::weak_instance<T>::w;
    inline backbone& select(){ return backbone::weak_instance<void>::w; }
}

#endif
