#pragma once
#include <QGraphicsScene>
#include "../../domain/results/CuttingResult.h"
#include "../../domain/entities/Sheet.h"

class CuttingRenderer
{
public:
    static void render(QGraphicsScene* scene, const Sheet& sheet, const CuttingResult& result);
};