#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onSliderMoved(int value);
    void onTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    void initializeConnections();  // Déclaration de la fonction
};

#endif // MAINWINDOW_H

