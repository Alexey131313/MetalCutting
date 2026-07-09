#include "AlgorithmTestRunner.h"

#include "TestScenarios.h"

#include "../app/CuttingService.h"

#include <QDebug>

bool AlgorithmTestRunner::runAll()
{
    auto tests =
        createTestScenarios();

    int passed = 0;

    for (const auto& test : tests)
    {
        qDebug()
        << "\nRunning:"
        << test.name;

        CuttingService service;

        try
        {
            auto result =
                service.execute(test.request, test.algorithm);

            bool ok =
                result.producedCount ==
                    test.expectedProduced &&
                result.unproducedCount ==
                    test.expectedUnproduced;

            if (ok)
            {
                ++passed;

                qDebug()
                    << "[PASS]";
            }
            else
            {
                qDebug()
                << "[FAIL]";

                qDebug()
                    << "Expected produced:"
                    << test.expectedProduced;

                qDebug()
                    << "Actual produced:"
                    << result.producedCount;
            }
        }
        catch (...)
        {
            qDebug()
            << "[EXCEPTION]";
        }
    }

    qDebug()
        << "\nPassed"
        << passed
        << "of"
        << tests.size();

    return passed ==
           tests.size();
}