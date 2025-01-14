#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QTimer>
#include <QTime>

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
    void onStartButtonClicked();
    void onStopButtonClicked();
    void updateDisplay();
    void exportToCSV();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QTimer *timer;
    QTime chronoTime;
    QTime lastTime;
    bool isRunning = false;

    void setupTableView();
    void addRow(const QString &time, const QString &gap);
};

#endif // MAINWINDOW_H


