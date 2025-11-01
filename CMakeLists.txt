cmake_minimum_required(VERSION 4.1)
project(manapiqtdesign)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0070 NEW)

OPTION(MANAPIQT_BUILD_TYPE "Build result: binary or library" "lib")
OPTION(MANAPIQT_INSTALL_ARCH "/x86_64-linux-gnu for example" "")

find_package(manapihttp REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets Gui Core5Compat Svg)

set(CMAKE_AUTOMOC ON)
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_compile_options($<$<CXX_COMPILER_ID:GNU>:-fmerge-all-constants>)

set(FILES ./manapiqt/include/ManapiQt.hpp
        include/ManapiPrimaryButton.hpp
        src/ManapiPrimaryButton.cpp
        include/ManapiStyles.hpp
        src/ManapiStyles.cpp
        include/ManapiSecondaryButton.hpp
        src/ManapiSecondaryButton.cpp
        include/ManapiIconButton.hpp
        src/ManapiIconButton.cpp
        include/ManapiFlatButton.hpp
        src/ManapiFlatButton.cpp
        include/ManapiLineEdit.hpp
        src/ManapiLineEdit.cpp
        src/ManapiCheckBox.cpp
        include/ManapiToolBar.hpp
        src/ManapiToolBar.cpp
        include/ManapiToolButton.hpp
        src/ManapiToolButton.cpp
        include/ManapiToolBox.hpp
        src/ManapiToolBox.cpp
        include/ManapiMenu.hpp
        src/ManapiMenu.cpp
        include/ManapiAction.hpp
        src/ManapiAction.cpp
        include/ManapiSpacerLine.hpp
        src/ManapiSpacerLine.cpp
        include/ManapiClickableWidget.hpp
        src/ManapiClickableWidget.cpp
        src/ManapiTreeView.cpp
        include/ManapiTreeView.hpp
        include/ManapiTableView.hpp
        src/ManapiTableView.cpp)

if (MANAPIQT_BUILD_TYPE STREQUAL "exe")
    add_executable(${PROJECT_NAME} main.cpp ${FILES})
else ()
    add_library(${PROJECT_NAME} ${FILES})
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE manapihttp Qt6::Core Qt6::Quick Qt6::Widgets Qt6::Gui Qt6::Core5Compat Qt6::Svg)
target_include_directories(${PROJECT_NAME} PRIVATE ./manapiqt/include ./include)


if (MANAPIQT_BUILD_TYPE STREQUAL "exe")
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/Zi>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-ggdb>
    )

    include(GNUInstallDirs)

    if (MANAPQT_INSTALL_ARCH)
        set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR}${MANAPQT_INSTALL_ARCH})
        set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR}${MANAPQT_INSTALL_ARCH})
    endif ()


    install(TARGETS ${PROJECT_NAME} EXPORT manapiqt-targets
            ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
            INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            #            FILE_SET CXX_MODULES
            #            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/src
            #            CXX_MODULES_BMI
            #            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/bmi-${CMAKE_CXX_COMPILER_ID}_$<CONFIG>
            #            FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECTNAME}
    )

    install(DIRECTORY manapiqt/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
    install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
    # install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    install(EXPORT manapiqt-targets
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
            #            CXX_MODULES_DIRECTORY .
    )

    set_target_properties(${PROJECT_NAME} PROPERTIES
            COMPILE_PDB_NAME ${PROJECT_NAME}
            COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR}
    )

    install(FILES cmake/manapiqtdesign-config.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
endif ()