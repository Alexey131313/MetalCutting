#pragma once

#include "Rectangle.h"

struct Placement
{
    int partId{};

    Rectangle rect;

    bool rotated{false};
};