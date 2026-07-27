#pragma once
struct PartSummary
{
    int id{};
    double width{};
    double height{};
    int requested{};
    int produced{};
    bool allowRotation{true};
};

