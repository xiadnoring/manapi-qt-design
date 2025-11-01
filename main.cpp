#include <manapihttp/ManapiInitTools.hpp>
#include <manapihttp/ManapiEventLoop.hpp>
#include <manapihttp/ManapiTime.hpp>
#include <manapihttp/std/ManapiScopePtr.hpp>
#include <manapihttp/fs/ManapiFilesystem.hpp>
#include <manapihttp/std/ManapiBeforeDelete.hpp>

#include <fstream>
#include <iostream>
#include <ManapiCheckBox.hpp>

#include <QApplication>
#include <QTextEdit>
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPushButton>
#include <QMainWindow>
#include <QWindow>
#include <QVBoxLayout>
#include <QWidgetAction>
#include <QSplitter>
#include <qstandarditemmodel.h>
#include <QTabWidget>
#include <QHeaderView>
#include <QTableWidget>

#include "ManapiAction.hpp"
#include "ManapiFlatButton.hpp"
#include "ManapiIconButton.hpp"
#include "ManapiPrimaryButton.hpp"
#include "ManapiQt.hpp"
#include "ManapiSecondaryButton.hpp"
#include "ManapiStyles.hpp"
#include "ManapiLineEdit.hpp"
#include "ManapiMenu.hpp"
#include "ManapiTableView.hpp"
#include "ManapiToolBar.hpp"
#include "ManapiToolBox.hpp"
#include "ManapiToolButton.hpp"
#include "ManapiTreeView.hpp"

int main(int argc, char *argv[]) {
    manapi::init_tools::log_trace_init(manapi::debug::LOG_TRACE_HIGH);

    manapi::qt::init_styles("./../styles");

    auto ctx = manapi::async::context::create(0).unwrap();

    ctx->run (0, [&] (const std::function<void()>& cb) -> void {
        auto event_dispatcher = manapi::qt::event_dispatcher::create().unwrap();
        QCoreApplication::setEventDispatcher(event_dispatcher);

        QApplication app (argc, argv);
        QApplication::setStyle("Basic");
        QMainWindow window;

        manapi::qt::subscribe_stylesheet(&window, "app.css");
        manapi::before_delete bd ([window = &window] ()
            -> void { manapi::qt::unsubscribe_stylesheet(window); });

        manapi::scope_ptr testmenu (new manapi::qt::Menu ());
        testmenu->addAction("Customize Toolbar");
        testmenu->addAction("Add Action");
        testmenu->addSeparator();
        auto sayhello = testmenu->addAction("Say Hello");

        manapi::scope_ptr text_block (new QTextEdit ());

        manapi::scope_ptr btn_primary1 (new manapi::qt::PrimaryButton ("OK"));
        manapi::scope_ptr btn_primary2 (new manapi::qt::PrimaryButton ("OK"));

        manapi::scope_ptr btn_sec1 (new manapi::qt::SecondaryButton ("Cancel"));
        manapi::scope_ptr btn_sec2 (new manapi::qt::SecondaryButton ("Apply"));

        manapi::scope_ptr icon1_pixmap (new QPixmap ("./../icon1.svg"));
        manapi::scope_ptr icon2_pixmap (new QPixmap ("./../icon1.svg"));
        manapi::scope_ptr icon_start (new QPixmap ("./../play-solid-full.svg"));
        manapi::scope_ptr icon_stop (new QPixmap ("./../stop-solid-full.svg"));

        manapi::scope_ptr btn_icon1 (new manapi::qt::IconButton ());

        btn_icon1->setIconDefault(*icon_start);
        btn_icon1->setIconFocus(*icon_start);
        btn_icon1->setIconPressed(*icon_stop);

        btn_icon1->setObjectName("btnicon1");
        manapi::scope_ptr btn_icon2 (new manapi::qt::IconButton ());
        btn_icon2->setIcon(*icon2_pixmap);
        manapi::scope_ptr btn_icon3 (new manapi::qt::IconButton ());
        btn_icon3->setIcon(*icon_stop);

        manapi::scope_ptr btn_flat1 (new manapi::qt::FlatButton ("UTF-8"));
        manapi::scope_ptr btn_flat2 (new manapi::qt::FlatButton ("ASCII"));
        manapi::scope_ptr btn_flat3 (new manapi::qt::FlatButton ( "Windows-1281"));

        manapi::scope_ptr line_edit (new manapi::qt::LineEdit ( "Line Edit"));
        manapi::scope_ptr line_edit_disabled (new manapi::qt::LineEdit ( "Disabled"));

        manapi::scope_ptr checkbox (new manapi::qt::CheckBox ( "Enabled"));
        manapi::scope_ptr checkbox_disabled (new manapi::qt::CheckBox ( "#1"));
        manapi::scope_ptr checkbox_disabled2 (new manapi::qt::CheckBox ( "#2"));
        manapi::scope_ptr checkbox_disabled3 (new manapi::qt::CheckBox ( "#3"));

        manapi::scope_ptr toolbox (new manapi::qt::ToolBox ());
        toolbox->addItem(new QLabel ("TEXT #1TEXT #1TEXT #1TEXT #1TEXT #1TEXT #1TEXT #1TEXT #1"), "TITLE #11");
        toolbox->addItem(new QLabel ("TEXT #2TEXT #2TEXT #2TEXT #2TEXT #2TEXT #2TEXT #2TEXT #2"), "TITLE #21");
        toolbox->addItem(new QLabel ("TEXT #3TEXT #3TEXT #3TEXT #3TEXT #3TEXT #3TEXT #3TEXT #3"), "TITLE #31");

        manapi::scope_ptr toolbtn_exit (new manapi::qt::ToolButton ("&Exit"));
        manapi::scope_ptr toolbtn_file (new manapi::qt::ToolButton ("&File"));
        manapi::scope_ptr toolbtn_file_menu (new manapi::qt::Menu ());
        toolbtn_file_menu->addAction(*icon1_pixmap, "Save")->setShortcut(Qt::CTRL | Qt::Key_S);

        auto toolbtn_file_menu_s = new manapi::qt::Action (*icon1_pixmap, "Open");
        toolbtn_file_menu_s->setDisabled(true);
        toolbtn_file_menu->addAction(toolbtn_file_menu_s);
        toolbtn_file_menu->addSeparator();

        manapi::scope_ptr toolbtn_file3_menu (new manapi::qt::Menu ("Settings"));
        toolbtn_file3_menu->addAction(*icon1_pixmap, "New");
        toolbtn_file3_menu->addAction("Open");
        toolbtn_file3_menu->addSeparator();
        toolbtn_file3_menu->addAction("Settings");
        toolbtn_file_menu->addAction("Settings");
        toolbtn_file_menu->addMenu(toolbtn_file3_menu.release());
        toolbtn_file->setMenu(toolbtn_file_menu.release());
        manapi::scope_ptr toolbtn_file2 (new manapi::qt::ToolButton ("&View"));
        manapi::scope_ptr toolbtn_file2_menu (new manapi::qt::Menu ());
        toolbtn_file2_menu->addAction(*icon1_pixmap, "New");
        toolbtn_file2_menu->addAction("Open");
        toolbtn_file2_menu->addSeparator();
        toolbtn_file2_menu->addAction("Settings");
        toolbtn_file2->setMenu(toolbtn_file2_menu.release());
        manapi::scope_ptr toolbar (new manapi::qt::ToolBar ());

        btn_flat1->setActive(true);
        btn_primary2->setDisabled(true);
        btn_sec2->setDisabled(true);
        btn_flat3->setDisabled(true);
        btn_icon3->setDisabled(true);
        line_edit_disabled->setDisabled(true);
        checkbox_disabled->setDisabled(true);
        checkbox_disabled2->setDisabled(true);
        checkbox_disabled2->setCheckState(Qt::CheckState::Checked);
        checkbox_disabled3->setCheckState(Qt::CheckState::PartiallyChecked);

        manapi::scope_ptr treeview (new manapi::qt::TreeView ());

        bool modelfin = false;
        manapi::scope_ptr model (new QStandardItemModel() );
        manapi::async::run ([&modelfin, model = model.get()] () mutable  -> manapi::future<> {
            auto dir = manapi::unwrap(co_await manapi::filesystem::async_opendir("/"));
            std::unique_ptr<manapi::ev::dirent_t, manapi::ev::impl_array_deleter<manapi::ev::dirent_t>> dirents (new manapi::ev::dirent_t[16]);
            dir->dirents = dirents.get();
            dir->nentries = 16;
            bool flg = true;
            try {
                while (true) {
                    auto res = manapi::unwrap(co_await manapi::filesystem::async_readdir(dir, [model, &flg] (manapi::ev::dir_t *s, std::size_t result) -> void {
                        for (int i = 0; i < result; i++) {
                            bool const isDir = s->dirents[i].type == uv_dirent_type_t::UV_DIRENT_DIR;
                            manapi::scope_ptr file (new QStandardItem ( manapi::qt::pixmap(isDir ? "folder" : "file",
                                QSize(16,16), Qt::darkGray), s->dirents[i].name));
                            model->appendRow(file.release());
                            if (isDir) {
                                file->setData(QString::fromStdString(manapi::filesystem::path::join("/", s->dirents[i].name)));
                            }
                        }
                    }));
                    if (res != dir->nentries) {
                        break;
                    }
                }
            }
            catch (std::exception const &e) {
                manapi_log_error(e.what());
            }
            co_await manapi::filesystem::async_closedir(dir);
            modelfin = true;
        });

        treeview->setModel(model.release());
        manapi::scope_ptr selection_model ( new QItemSelectionModel(treeview->model()));
        treeview->setSelectionModel(selection_model.release());
        {
            static auto cb = +[] (QStandardItem *model, std::string path) mutable  -> manapi::future<> {
                auto dir = manapi::unwrap(co_await manapi::filesystem::async_opendir(path));
                std::unique_ptr<manapi::ev::dirent_t, manapi::ev::impl_array_deleter<manapi::ev::dirent_t>> dirents (new manapi::ev::dirent_t[16]);
                dir->dirents = dirents.get();
                dir->nentries = 16;
                try {
                    while (true) {
                        auto res = manapi::unwrap(co_await manapi::filesystem::async_readdir(dir, [model, path] (manapi::ev::dir_t *s, std::size_t result) -> void {
                            for (int i = 0; i < result; i++) {
                                bool const isDir = s->dirents[i].type == uv_dirent_type_t::UV_DIRENT_DIR;
                                manapi::scope_ptr file (new QStandardItem ( manapi::qt::pixmap(isDir ? "folder" : "file",
                                    QSize(16,16), Qt::darkGray), s->dirents[i].name));
                                model->appendRow(file.release());
                                if (isDir) {
                                    file->setData(QString::fromStdString(manapi::filesystem::path::join(path, s->dirents[i].name)));
                                }
                            }
                        }));
                        if (res != dir->nentries) {
                            break;
                        }
                    }
                }
                catch (std::exception const &e) {
                    manapi_log_error(e.what());
                }
                co_await manapi::filesystem::async_closedir(dir);
            };

            QWidget::connect(treeview->selectionModel(), &QItemSelectionModel::currentChanged, [&modelfin, model = model.get()] (  const QModelIndex & current,
    const QModelIndex & previous) -> void {
                if (!modelfin) {
                    return;
                }
                auto item = model->itemFromIndex(current);
                if (item) {
                    auto data = item->data();
                    if (!data.isNull()) {
                        item->setData(QVariant ());
                        auto s = data.toString().toStdString();
                        manapi::async::run (cb (item, std::move(s)));
                    }
                }
            });
        }

        manapi::scope_ptr vbox (new QVBoxLayout);
        manapi::scope_ptr label (new QLabel);

        manapi::scope_ptr hbox1 (new QHBoxLayout);
        manapi::scope_ptr hbox2 (new QHBoxLayout);
        manapi::scope_ptr hbox3 (new QHBoxLayout);
        manapi::scope_ptr hbox4 (new QHBoxLayout);
        manapi::scope_ptr hbox5 (new QHBoxLayout);
        manapi::scope_ptr hbox6 (new QHBoxLayout);

        hbox1->setSpacing(5);
        hbox2->setSpacing(5);
        hbox3->setSpacing(5);
        hbox5->setSpacing(5);
        hbox6->setSpacing(5);

        vbox->addWidget(label.release());

        hbox1->addWidget(btn_primary1.release());
        hbox1->addWidget(btn_sec1.release());

        hbox2->addWidget(btn_primary2.release());
        hbox2->addWidget(btn_sec2.release());

        hbox3->addWidget(btn_icon1.release());
        hbox3->addWidget(btn_icon2.release());
        hbox3->addWidget(btn_icon3.release());

        hbox3->setAlignment(hbox1.get(), Qt::AlignLeft);
        hbox3->setAlignment(hbox2.get(), Qt::AlignLeft);
        hbox3->setAlignment(hbox3.get(), Qt::AlignLeft);
        hbox3->addSpacerItem(new QSpacerItem (0,0));
        hbox3->setStretch(0, 0);
        hbox3->setStretch(1, 0);
        hbox3->setStretch(2, 0);
        hbox3->setStretch(3, 1);

        hbox4->addWidget(btn_flat1.release());
        hbox4->addWidget(btn_flat2.release());
        hbox4->addWidget(btn_flat3.release());

        hbox5->addWidget(line_edit.release());
        hbox5->addWidget(line_edit_disabled.release());

        hbox6->addWidget(checkbox.release());
        hbox6->addWidget(checkbox_disabled.release());
        hbox6->addWidget(checkbox_disabled2.release());
        hbox6->addWidget(checkbox_disabled3.release());

        vbox->addItem(hbox1.release());
        vbox->addItem(hbox2.release());
        vbox->addItem(hbox3.release());
        vbox->addItem(hbox4.release());
        vbox->addItem(hbox5.release());
        vbox->addItem(hbox6.release());
        vbox->addWidget(toolbox.release());
        vbox->addWidget(text_block.release());
        vbox->addSpacerItem(new QSpacerItem (0, 0));
        vbox->setAlignment(hbox1.get(), Qt::AlignTop);
        vbox->setAlignment(hbox2.get(), Qt::AlignTop);
        vbox->setAlignment(hbox3.get(), Qt::AlignTop);
        vbox->setAlignment(hbox4.get(), Qt::AlignTop);
        vbox->setAlignment(hbox5.get(), Qt::AlignTop);
        vbox->setAlignment(hbox6.get(), Qt::AlignTop);
        vbox->setAlignment(toolbox.get(), Qt::AlignTop);
        vbox->setAlignment(text_block.get(), Qt::AlignTop);
        vbox->setStretch(0, 0);
        vbox->setStretch(1, 0);
        vbox->setStretch(2, 0);
        vbox->setStretch(3, 0);
        vbox->setStretch(4, 0);
        vbox->setStretch(5, 0);
        vbox->setStretch(6, 0);
        vbox->setStretch(7, 0);
        vbox->setStretch(8, 1);

        toolbar->addWidget(toolbtn_exit.release());
        toolbar->addWidget(toolbtn_file.release());
        toolbar->addWidget(toolbtn_file2.release());

        manapi::scope_ptr svbox (new QVBoxLayout ());
        manapi::scope_ptr tabs (new QTabWidget ());
        manapi::scope_ptr table (new manapi::qt::TableView());

        tabs->addTab(table.release(), "Table");
        table->setColumnCount(4); // Указываем число колонок
        table->setShowGrid(true); // Включаем сетку
        // Разрешаем выделение только одного элемента
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        // Разрешаем выделение построчно
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setHorizontalHeaderLabels({"First Name", "Last Name", "Age", "Work"});
        table->horizontalHeader()->setStretchLastSection(true);
        for (int i =0;i < 6; i++) {
            table->insertRow(i);
            table->setItem (i, 0, new QTableWidgetItem ("Timur"));
            table->setItem (i, 1, new QTableWidgetItem ("Zajnullin"));
            table->setItem (i, 2, new QTableWidgetItem ("18"));
            table->setItem (i, 3, new QTableWidgetItem ("Programmer"));
        }
        table->resizeColumnsToContents();
        svbox->addWidget(tabs.release());
        tabs->addTab(new QTextEdit ("OKEY DOKEY"), "Text");

        manapi::scope_ptr form (new QGridLayout ());

        QTextEdit *formte1;
        QLineEdit *formpe1;
        QLabel *formla1;

        formla1 = new QLabel ("First Name");
        formpe1 = new QLineEdit ();
        formpe1->setFixedWidth(220);
        formla1->setBuddy(formpe1);

        form->addWidget(formla1, 0, 0, Qt::AlignLeft | Qt::AlignTop);
        form->addWidget(formpe1, 0, 1, Qt::AlignRight);

        formla1 = new QLabel ("Last Name");
        formpe1 = new QLineEdit ();
        formpe1->setFixedWidth(220);
        formla1->setBuddy(formpe1);

        form->addWidget(formla1, 1, 0, Qt::AlignLeft | Qt::AlignTop);
        form->addWidget(formpe1, 1, 1, Qt::AlignRight);

        formla1 = new QLabel ("Age");
        formpe1 = new QLineEdit ();
        formpe1->setFixedWidth(220);
        formla1->setBuddy(formpe1);

        form->addWidget(formla1, 2, 0, Qt::AlignLeft | Qt::AlignTop);
        form->addWidget(formpe1, 2, 1, Qt::AlignRight);

        formla1 = new QLabel ("Work");
        formte1 = new QTextEdit ();
        formte1->setFixedWidth(250);
        formla1->setBuddy(formte1);

        form->addWidget(formla1, 3, 0, Qt::AlignLeft | Qt::AlignTop);
        form->addWidget(formte1, 3, 1, Qt::AlignRight);


        manapi::scope_ptr formvbox (new QVBoxLayout ());
        manapi::scope_ptr formhbox (new QHBoxLayout ());

        manapi::scope_ptr formw (new QWidget ());
        formvbox->addLayout(form.release());
        formvbox->addLayout(formhbox.release());
        formvbox->setSpacing(5);
        formvbox->setAlignment(formhbox.get(), Qt::AlignBottom);
        formvbox->setAlignment(form.get(), Qt::AlignTop);
        formvbox->setStretch(0, 1);
        formvbox->setStretch(1, 0);
        formhbox->setSpacing(5);
        formhbox->addSpacerItem(new QSpacerItem (0, 0));
        auto formhp1 = new manapi::qt::PrimaryButton ("OK");
        formhbox->addWidget(formhp1);
        auto formhp2 = new manapi::qt::SecondaryButton ("Reset");
        formhbox->addWidget(formhp2);
        formhbox->setAlignment(formhp1, Qt::AlignRight);
        formhbox->setAlignment(formhp2, Qt::AlignRight);
        formhbox->setStretch(0, 1);
        formhbox->setStretch(1,0);
        formhbox->setStretch(2, 0);
        formw->setLayout(formvbox.release());
        tabs->addTab(formw.release(), "Form");

        manapi::scope_ptr winwin (new QWidget());
        manapi::scope_ptr winwinlayout (new QHBoxLayout ());

        manapi::scope_ptr splitter (new QSplitter ());
        manapi::scope_ptr winwinwidget (new QWidget ());
        manapi::scope_ptr reallayout (new QHBoxLayout ());
        reallayout->addLayout(vbox.release());
        reallayout->addLayout(svbox.release());
        reallayout->setSpacing(10);
        winwinwidget->setLayout(reallayout.release());
        splitter->addWidget(treeview.release());
        splitter->addWidget(winwinwidget.release());
        winwinlayout->setContentsMargins(0, 0, 0, 0);
        winwinlayout->addWidget(splitter.release());

        winwin->setLayout(winwinlayout.release());

        window.setWindowTitle("float");

        window.setCentralWidget(winwin.release());
        window.addToolBar(toolbar.release());
        window.show();

        QWidget::connect(btn_primary1.get(), &QPushButton::released, [] () -> void {

        });

        QWidget::connect(sayhello, &QAction::triggered, [] () -> void {
            manapi_log_debug("Hello, World!");
        });

        QWidget::connect(btn_icon2.get(), &QPushButton::clicked, [testmenu = testmenu.get()] () -> void {
            testmenu->showMenu();
        });

        QWidget::connect(toolbtn_exit.get(), &QPushButton::clicked, [] () -> void {
             manapi::async::run(manapi::async::current()->stop());
        });

        manapi::async::current()->timerpool()->append_interval_async(5000, [] (const auto &) -> manapi::future<> {
            manapi::unwrap(co_await manapi::qt::load_styles());
        });

        manapi::async::current()->eventloop()->custom_event_loop([&app, event_dispatcher] ()
            -> void {
            QApplication::exec();
            manapi::qt::unsubscribe_stylesheets();
            event_dispatcher->unsubscribe();
        });

        manapi::async::run ([] () -> manapi::future<> {
            manapi::unwrap(co_await manapi::qt::load_styles());
        });

        manapi::async::current()->timerpool()->append_interval_sync(1,
            [label = label.get()] (const manapi::timer &) -> void {
            label->setText(QString::fromStdString(std::format("{:}",
                manapi::time::current_time())));
        });

        cb ();
    });

    return 0;
}
