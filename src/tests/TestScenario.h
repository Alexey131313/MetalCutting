#pragma once

#include "../domain/requests/CuttingRequest.h"
#include "../app/AlgorithmFactory.h"

#include <QString>

struct TestScenario
{
    QString name;

    CuttingRequest request;

    AlgorithmType algorithm;

    int expectedProduced{};
    int expectedUnproduced{};
};