#include "CuttingRenderer.h"
#include <QBrush>
#include <QColor>
#include <QGraphicsTextItem>
#include <QPen>

void CuttingRenderer::render(QGraphicsScene* scene, const Sheet& sheet, const CuttingResult& result)
{
    scene->clear();
    constexpr int margin = 20;
    scene->setSceneRect(-margin, -margin, sheet.width + margin * 2, sheet.height + margin * 2);

    QPen borderPen(Qt::black, 2);
    borderPen.setCosmetic(true);
    scene->addRect(0, 0, sheet.width, sheet.height, borderPen, Qt::NoBrush);

    QPen gridPen(Qt::lightGray);
    gridPen.setWidth(0);
    gridPen.setCosmetic(true);
    for (int x = 0; x <= sheet.width; x += 10)
    {
        scene->addLine(x + 0.5, 0, x + 0.5, sheet.height, gridPen);
    }
    for (int y = 0; y <= sheet.height; y += 10)
    {
        scene->addLine(0, y + 0.5, sheet.width, y + 0.5, gridPen);
    }

    auto sheetText = scene->addText(QString("%1 x %2").arg(sheet.width).arg(sheet.height));
    sheetText->setPos(5, 5);
    for (const auto& placement : result.placements)
    {
        QColor colors[]{QColor(220,240,255), QColor(220,255,220), QColor(255,240,220), QColor(255,220,240), QColor(240,240,200)};
        QColor color = colors[placement.partId % std::size(colors)];
        scene->addRect(placement.rect.x, placement.rect.y, placement.rect.width, placement.rect.height, QPen(QColor(Qt::black)), QBrush(QColor(color)));
        QString textItem;
        if (placement.rotated) {
            textItem = QString("%1(R)\n(%2,%3)").arg(placement.partId).arg(placement.rect.width).arg(placement.rect.height);
        } else {
            textItem = QString("%1\n(%2,%3)").arg(placement.partId).arg(placement.rect.width).arg(placement.rect.height);
        }
        auto text = scene->addText(textItem);
        text->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        QRectF rect(placement.rect.x, placement.rect.y, placement.rect.width, placement.rect.height);
        QRectF textRect = text->boundingRect();
        text->setPos(rect.center().x() - textRect.width()/2, rect.center().y() - textRect.height()/2);
    }
}