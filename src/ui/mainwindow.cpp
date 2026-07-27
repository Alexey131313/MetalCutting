#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include "../app/CuttingService.h"
#include "graphics/CuttingRenderer.h"
#include <QShowEvent>
#include <QMessageBox>
#include "inputwindow.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QSizePolicy>
#include "graphics/SummaryBuilder.h"
#include "graphics/UiUtils.h"

MainWindow::MainWindow(const CuttingRequest& request, AlgorithmType algorithm, InputWindow* inputWindow, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), inputWindow_(inputWindow)
{
    ui->setupUi(this);
    auto* rootLayout = new QHBoxLayout(ui->centralwidget);
    rootLayout->setContentsMargins(10, 10, 10, 10);
    rootLayout->setSpacing(10);
    rootLayout->addWidget(ui->graphicsView, 1);
    ui->graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        const double sheetArea = request.sheet.width * request.sheet.height;
        double utilization = sheetArea > 0.0 ? (result.usedArea / sheetArea) * 100.0 : 0.0;
        setWindowTitle(QString("Produced: %1 | Unproduced: %2 | Utilization: %3%").arg(result.producedCount).arg(result.unproducedCount).arg(utilization,0,'f',2));
        auto summaries = SummaryBuilder::build(request.parts, result.placements);
        double usedLength = SummaryBuilder::calculateUsedLength(result.placements);
        detailsPanel_ = new QFrame(ui->centralwidget);
        detailsPanel_->setObjectName("detailsPanel");
        detailsPanel_->setMinimumWidth(340);
        detailsPanel_->setMaximumWidth(380);
        detailsPanel_->setStyleSheet("QFrame#detailsPanel { background: rgba(245, 245, 245, 235); border: 1px solid rgba(80, 80, 80, 120); border-radius: 8px; }"
            "QLabel { color: #202020; }");

        auto* panelLayout = new QVBoxLayout(detailsPanel_);
        panelLayout->setContentsMargins(12, 12, 12, 12);
        panelLayout->setSpacing(8);

        auto* titleLabel = new QLabel("Детализация раскроя", detailsPanel_);
        titleLabel->setStyleSheet("font-size: 15px; font-weight: 600;");
        panelLayout->addWidget(titleLabel);

        auto* linearLabel = new QLabel(QString("Линейный расход: %1 м из %2 м\nОстаток по длине: %3 м")
                                           .arg(UiUtils::formatLengthMeters(usedLength))
                                           .arg(UiUtils::formatLengthMeters(request.sheet.height))
                                           .arg(UiUtils::formatLengthMeters(std::max(0.0, request.sheet.height - usedLength))),detailsPanel_);
        linearLabel->setWordWrap(true);
        panelLayout->addWidget(linearLabel);

        auto* utilizationLabel = new QLabel(QString("Площадь использована: %1%\nЛинейное использование: %2%")
                .arg(utilization, 0, 'f', 2)
                .arg(request.sheet.height > 0.0 ? (usedLength / request.sheet.height) * 100.0 : 0.0, 0, 'f', 2), detailsPanel_);
        utilizationLabel->setWordWrap(true);
        panelLayout->addWidget(utilizationLabel);

        auto* scrollArea = new QScrollArea(detailsPanel_);
        scrollArea->setWidgetResizable(true);
        scrollArea->setFrameShape(QFrame::NoFrame);

        auto* scrollContent = new QWidget(scrollArea);
        auto* scrollLayout = new QVBoxLayout(scrollContent);
        scrollLayout->setContentsMargins(0, 0, 0, 0);
        scrollLayout->setSpacing(8);

        for (const auto& summary : summaries)
        {
            const bool isProduced = summary.produced > 0;
            auto* itemLabel = new QLabel(QString("Изделие #%1\nРазмер: %2 x %3\nЗаказано: %4\nВыполнено: %5\nОсталось: %6\nПоворот: %7")
                    .arg(summary.id)
                    .arg(summary.width, 0, 'f', 2)
                    .arg(summary.height, 0, 'f', 2)
                    .arg(summary.requested)
                    .arg(summary.produced)
                    .arg(std::max(0, summary.requested - summary.produced))
                    .arg(summary.allowRotation ? "да" : "нет"), scrollContent);
            itemLabel->setWordWrap(true);
            itemLabel->setStyleSheet(isProduced ? "QLabel { padding: 8px; background: rgba(255, 255, 255, 210); border: 1px solid rgba(0, 0, 0, 40); border-radius: 6px; }"
                    : "QLabel { padding: 8px; background: rgba(255, 226, 226, 225); border: 1px solid rgba(170, 90, 90, 90); border-radius: 6px; }");
            scrollLayout->addWidget(itemLabel);
        }

        scrollLayout->addStretch(1);
        scrollArea->setWidget(scrollContent);
        panelLayout->addWidget(scrollArea, 1);

        rootLayout->addWidget(detailsPanel_);
        rootLayout->setStretch(0, 1);
        rootLayout->setStretch(1, 0);

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
