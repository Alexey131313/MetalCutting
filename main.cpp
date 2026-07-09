#include <QApplication>
#include "ui/mainwindow.h"
#include "tests/AlgorithmTestRunner.h"

#define RUN_TESTS 0

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    #if RUN_TESTS

        AlgorithmTestRunner::runAll();

        return 0;

    #else

        MainWindow window;

        window.show();

        return app.exec();

    #endif
}