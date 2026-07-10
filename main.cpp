#include <QApplication>
#include "ui/inputwindow.h"
#include "tests/AlgorithmTestRunner.h"

#define RUN_TESTS 0

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    #if RUN_TESTS
        AlgorithmTestRunner::runAll();
        return 0;
    #else
        InputWindow window;
        window.show();
        return app.exec();
    #endif
}