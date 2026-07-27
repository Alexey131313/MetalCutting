#pragma once
#include <QMainWindow>

namespace Ui
{
class InputWindow;
}

class InputWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InputWindow(QWidget* parent = nullptr);
    ~InputWindow() override;

private slots:
    void onAddPartClicked();
    void onRemovePartClicked();
    void updateIds();
    void onCalculateClicked();

private:
    Ui::InputWindow* ui;
};