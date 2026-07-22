cmake_minimum_required(VERSION 3.19)

project(MetalCutting LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt6 6.5 REQUIRED COMPONENTS
    Core
    Widgets
    LinguistTools
)

qt_standard_project_setup()

set(UI_SOURCES
    src/ui/MainWindow.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.ui
    src/ui/inputwindow.h
    src/ui/inputwindow.cpp
    src/ui/inputwindow.ui

    src/ui/graphics/CuttingRenderer.h
    src/ui/graphics/CuttingRenderer.cpp

)

set(DOMAIN_SOURCES
    src/domain/entities/Rectangle.h
    src/domain/entities/Sheet.h
    src/domain/entities/Part.h
    src/domain/entities/Placement.h
    src/domain/entities/FreeRectangle.h
    src/domain/entities/ExpandedPart.h

    src/domain/requests/CuttingRequest.h

    src/domain/results/CuttingResult.h

    src/domain/algorithms/ICuttingAlgorithm.h
    src/domain/algorithms/GreedyAlgorithm.h
    src/domain/algorithms/GreedyAlgorithm.cpp
    src/domain/algorithms/PartExpander.h
    src/domain/algorithms/PartSorter.h
    src/domain/algorithms/PartExpander.cpp
    src/domain/algorithms/PartSorter.cpp
    src/domain/algorithms/GuillotineAlgorithm.h
    src/domain/algorithms/GuillotineAlgorithm.cpp
    src/domain/algorithms/MaxRectsAlgorithm.h
    src/domain/algorithms/MaxRectsAlgorithm.cpp
)

set(APPLICATION_SOURCES
    src/app/AlgorithmFactory.h
    src/app/AlgorithmFactory.cpp
)

set(INFRASTRUCTURE_SOURCES
    src/app/CuttingService.h
    src/app/CuttingService.cpp
    src/app/CuttingValidator.h
    src/app/CuttingValidator.cpp
)

set(TESTS_SOURCES




    src/tests/TestScenario.h
)

qt_add_executable(MetalCutting
    WIN32
    MACOSX_BUNDLE

    main.cpp

    ${UI_SOURCES}
    ${DOMAIN_SOURCES}
    ${APPLICATION_SOURCES}
    ${INFRASTRUCTURE_SOURCES}
    ${TESTS_SOURCES}
)

qt_add_translations(
    TARGETS MetalCutting
    TS_FILES MetalCutting_ru_RU.ts
)

target_link_libraries(MetalCutting
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

install(TARGETS MetalCutting
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET MetalCutting
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

target_include_directories(MetalCutting PRIVATE
    src
)

install(SCRIPT ${deploy_script})