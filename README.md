# Qt6/7 Norm Design
Qt6 and Qt7 are supported. Library based on [ManapiHttp](https://github.com/xiadnoring/manapi-http) and
[Manapi Qt Extension](https://github.com/xiadnoring/manapi-qt-extension). 

Required [ManapiHttp](https://github.com/xiadnoring/manapi-http) version is ```1.0.3```

<img src="./assets/img.png" style="max-width: 600px">

## Example

```c++
#include <manapihttp/ManapiInitTools.hpp>
#include <manapihttp/ManapiEventLoop.hpp>
#include <manapihttp/ManapiTime.hpp>
#include <manapihttp/std/ManapiScopePtr.hpp>
#include <manapihttp/std/ManapiBeforeDelete.hpp>

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include <manapiqtdesign/ManapiQt.hpp>
#include <manapiqtdesign/ManapiStyles.hpp>
#include <manapiqtdesign/ManapiToolBar.hpp>

int main(int argc, char *argv[]) {
    manapi::init_tools::log_trace_init(manapi::debug::LOG_TRACE_HIGH);

    manapi::qt::init_styles("./../styles");
    
    auto ctx = manapi::async::context::create(0).unwrap();

    ctx->run ([&] (const std::function<void()>& cb) -> void {
        auto event_dispatcher = manapi::qt::event_dispatcher::create().unwrap();
        QCoreApplication::setEventDispatcher(event_dispatcher);

        QApplication app (argc, argv);
        QWidget window;

        manapi::qt::subscribe_stylesheet(&window, "app.css");
        manapi::before_delete bd ([window = &window] ()
            -> void { manapi::qt::unsubscribe_stylesheet(window); });

        manapi::scope_ptr vbox (new QVBoxLayout);
        manapi::scope_ptr label (new QLabel);

        vbox->addWidget(label.release());

        window.setLayout(vbox.release());
        
        /* WARNING: This code is not totally production-ready */
        manapi::async::run ([&window] () -> manapi::future<> {
            manapi::unwrap(co_await manapi::qt::load_styles());
            window.show();
        });

        manapi::async::current()->eventloop()->custom_event_loop([&app, event_dispatcher] ()
            -> void {
            /* event loop run here */
            QApplication::exec();
            event_dispatcher->unsubscribe();
        });

        manapi::async::current()->timerpool()->append_interval_sync(200,
            [label = label.get()] (const manapi::timer &) -> void {
            label->setText(QString::fromStdString(std::format("{:}", 
                manapi::time::current_time())));
        });

        /* custom event loop run here */
        cb ();
    } /* event loop run here too */ );

    return 0;
}
```

## Build & Install

```bash
# Init
cmake -B build

# Build
cmake --build build

# Install
cmake --install build
```