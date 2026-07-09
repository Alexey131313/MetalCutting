#pragma once

#include "../domain/requests/CuttingRequest.h"

#include <QString>

struct TestScenario
{
    QString name;

    CuttingRequest request;

    int expectedProduced{};
    int expectedUnproduced{};
};