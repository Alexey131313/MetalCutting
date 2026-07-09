#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "../domain/requests/CuttingRequest.h"
#include "../app/AlgorithmFactory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const CuttingRequest& request, AlgorithmType algorithm, QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
};
#endif // MAINWINDOW_H
