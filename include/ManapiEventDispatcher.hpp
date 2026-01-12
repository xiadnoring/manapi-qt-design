#pragma once

#include <memory>

namespace manapi::qt {
    class event_dispatcher;

    class EventDispatcherWrapper {
    public:
        EventDispatcherWrapper ();

        ~EventDispatcherWrapper ();

        void unsubscribe ();
    private:
        manapi::qt::event_dispatcher *m_src;
    };
}
