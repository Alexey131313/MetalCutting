#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include "../app/CuttingService.h"
#include "graphics/CuttingRenderer.h"
#include <QShowEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1000, 800);
    scene_ =
        new QGraphicsScene(this);

    ui->graphicsView->setScene(
        scene_);

    CuttingRequest request;

    request.sheet =
        {
            286,
            300
        };

    request.parts =
        {
            {1,286,150,1,true},
            {2,150,136,1,true}
        };

    CuttingService service;

    try{
        auto result =
            service.execute(
                request);
        CuttingRenderer::render(
            scene_,
            request.sheet,
            result);

        ui->graphicsView->fitInView(
            scene_->sceneRect(),
            Qt::KeepAspectRatio);
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
