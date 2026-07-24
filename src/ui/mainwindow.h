#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QEvent>
#include <QWheelEvent>
#include "../domain/requests/CuttingRequest.h"
#include "../app/AlgorithmFactory.h"

class InputWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const CuttingRequest& request, AlgorithmType algorithm, InputWindow* inputWindow, QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    InputWindow* inputWindow_;
    double baseScale_ = 1.0;
};
#endif // MAINWINDOW_H
