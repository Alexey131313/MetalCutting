#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include "../app/CuttingService.h"
#include "graphics/CuttingRenderer.h"
#include <QShowEvent>
#include <QMessageBox>

MainWindow::MainWindow(const CuttingRequest& request, AlgorithmType algorithm, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1220, 840);
    scene_ =
        new QGraphicsScene(this);

    ui->graphicsView->setScene(
        scene_);

    CuttingService service;

    try{
        auto result =
            service.execute(
                request,
                algorithm);
        CuttingRenderer::render(
            scene_,
            request.sheet,
            result);

        ui->graphicsView->fitInView(
            scene_->sceneRect(),
            Qt::KeepAspectRatio);

        double utilization =
            result.usedArea /
            (
                request.sheet.width *
                request.sheet.height
                ) * 100.0;

        setWindowTitle(
            QString(
                "Produced: %1 | Unproduced: %2 | Utilization: %3%")
                .arg(result.producedCount)
                .arg(result.unproducedCount)
                .arg(utilization,0,'f',2));

    } catch (const std::exception& e) {
        scene_->clear();
        QMessageBox::critical(
            this,
            "Ошибка раскроя",
            QString("Не удалось выполнить раскрой:\n%1").arg(e.what()));
    }
    catch (...) {
        QMessageBox::critical(
            this,
            "Неизвестная ошибка",
            "Произошла непредвиденная ошибка при выполнении раскроя");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    ui->graphicsView->fitInView(
        scene_->sceneRect(),
        Qt::KeepAspectRatio);
}
