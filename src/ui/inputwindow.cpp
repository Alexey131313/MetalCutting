#include "InputWindow.h"

#include "ui_inputwindow.h"

#include "mainwindow.h"

#include "../domain/requests/CuttingRequest.h"
#include "../domain/entities/Part.h"

#include "../app/AlgorithmFactory.h"

#include <QTableWidgetItem>

InputWindow::InputWindow(
    QWidget* parent)
    :
    QMainWindow(parent),
    ui(new Ui::InputWindow)
{
    ui->setupUi(this);

    ui->partsTable
        ->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch);

    connect(
        ui->addPartButton,
        &QPushButton::clicked,
        this,
        &InputWindow::onAddPartClicked);

    connect(
        ui->removePartButton,
        &QPushButton::clicked,
        this,
        &InputWindow::onRemovePartClicked);

    connect(
        ui->calculateButton,
        &QPushButton::clicked,
        this,
        &InputWindow::onCalculateClicked);
}

InputWindow::~InputWindow()
{
    delete ui;
}

void InputWindow::onAddPartClicked()
{
    int row =
        ui->partsTable->rowCount();

    ui->partsTable->insertRow(row);

    auto* idItem =
        new QTableWidgetItem(
            QString::number(row + 1));

    idItem->setFlags(
        idItem->flags()
        & ~Qt::ItemIsEditable);

    ui->partsTable->setItem(
        row,
        0,
        idItem);

    ui->partsTable->setItem(
        row,
        1,
        new QTableWidgetItem("100"));

    ui->partsTable->setItem(
        row,
        2,
        new QTableWidgetItem("100"));

    ui->partsTable->setItem(
        row,
        3,
        new QTableWidgetItem("1"));

    auto* rotationItem =
        new QTableWidgetItem();

    rotationItem->setCheckState(
        Qt::Checked);

    ui->partsTable->setItem(
        row,
        4,
        rotationItem);
}

void InputWindow::onRemovePartClicked()
{
    int row =
        ui->partsTable->currentRow();

    if (row < 0)
        return;

    ui->partsTable->removeRow(row);

    updateIds();
}

void InputWindow::updateIds()
{
    for (int row = 0;
         row < ui->partsTable->rowCount();
         ++row)
    {
        auto* item =
            ui->partsTable->item(
                row,
                0);

        if (item)
        {
            item->setText(
                QString::number(
                    row + 1));
        }
    }
}

void InputWindow::onCalculateClicked()
{
    CuttingRequest request;

    request.sheet.width =
        ui->sheetWidthSpinBox->value();

    request.sheet.height =
        ui->sheetHeightSpinBox->value();

    for(int row = 0;
         row < ui->partsTable->rowCount();
         ++row)
    {
        Part part;

        part.id =
            ui->partsTable
                ->item(row,0)
                ->text()
                .toInt();

        part.width =
            ui->partsTable
                ->item(row,1)
                ->text()
                .toDouble();

        part.height =
            ui->partsTable
                ->item(row,2)
                ->text()
                .toDouble();

        part.quantity =
            ui->partsTable
                ->item(row,3)
                ->text()
                .toInt();

        part.allowRotation =
            ui->partsTable
                ->item(row,4)
                ->text()
                .toLower() == "yes";

        request.parts.push_back(
            part);
    }

    AlgorithmType algorithm;

    switch(
        ui->algorithmComboBox
            ->currentIndex())
    {
    case 0:
        algorithm =
            AlgorithmType::Greedy;
        break;

    case 1:
        algorithm =
            AlgorithmType::Guillotine;
        break;

    default:
        algorithm =
            AlgorithmType::MaxRects;
        break;
    }

    auto* window =
        new MainWindow(
            request,
            algorithm);

    window->show();

    close();
}