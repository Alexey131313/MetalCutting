#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include "../app/CuttingService.h"
#include "graphics/CuttingRenderer.h"
#include <QShowEvent>
#include <QMessageBox>
#include "inputwindow.h"


MainWindow::MainWindow(const CuttingRequest& request, AlgorithmType algorithm, InputWindow* inputWindow, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), inputWindow_(inputWindow)
{
    ui->setupUi(this);
    resize(1220, 840);
    scene_ = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene_);
    ui->graphicsView->viewport()->installEventFilter(this);

    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    CuttingService service;

    try{
        auto result = service.execute(request, algorithm);
        CuttingRenderer::render(scene_, request.sheet, result);
        ui->graphicsView->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
        double utilization = result.usedArea / (request.sheet.width * request.sheet.height) * 100.0;
        setWindowTitle(QString("Produced: %1 | Unproduced: %2 | Utilization: %3%").arg(result.producedCount).arg(result.unproducedCount).arg(utilization,0,'f',2));

    } catch (const std::exception& e) {
        scene_->clear();
        QMessageBox::critical(this, "Ошибка раскроя", QString("Не удалось выполнить раскрой:\n%1").arg(e.what()));
    }
    catch (...) {
        QMessageBox::critical(this, "Неизвестная ошибка", "Произошла непредвиденная ошибка при выполнении раскроя");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    ui->graphicsView->fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
    baseScale_ = ui->graphicsView->transform().m11();

    statusBar()->showMessage("Масштаб: 100%");
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (inputWindow_)
        inputWindow_->show();
    event->accept();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsView->viewport() &&
        event->type() == QEvent::Wheel)
    {
        auto *wheel = static_cast<QWheelEvent *>(event);
        constexpr double zoomFactor = 1.15;
        constexpr double minScale = 0.2;
        constexpr double maxScale = 10.0;
        double currentScale = ui->graphicsView->transform().m11();
        double relativeScale = currentScale / baseScale_;

        if (wheel->angleDelta().y() > 0)
        {
            if (relativeScale < maxScale)
                ui->graphicsView->scale(zoomFactor, zoomFactor);
        }
        else
        {
            if (relativeScale > minScale)
                ui->graphicsView->scale(1.0 / zoomFactor, 1.0 / zoomFactor);
        }

        currentScale = ui->graphicsView->transform().m11();
        relativeScale = currentScale / baseScale_;
        statusBar()->showMessage(QString("Масштаб: %1%").arg(relativeScale * 100.0, 0, 'f', 0));
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}