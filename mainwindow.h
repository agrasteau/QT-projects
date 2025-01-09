#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void clearFields();
    void updateValidateButtonState();
    void enableValidationIfNeeded();
    void validateAlcoholLevel();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
