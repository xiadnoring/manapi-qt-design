#include "ManapiEventDispatcher.hpp"

#include <QCoreApplication>
#include "ManapiQt.hpp"

manapi::qt::EventDispatcherWrapper::EventDispatcherWrapper() {
    this->m_src = manapi::qt::event_dispatcher::create().unwrap();
    QCoreApplication::setEventDispatcher(this->m_src);
    manapi_log_debug("QCoreApplication::setEventDispatcher has finished");
}

manapi::qt::EventDispatcherWrapper::~EventDispatcherWrapper() {
}

void manapi::qt::EventDispatcherWrapper::unsubscribe() {
    this->m_src->unsubscribe();
}
