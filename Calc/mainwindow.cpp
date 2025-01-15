#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeConnections()
{
    // Connect slider signal to slot
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::onSliderMoved);

    // Connect line edits to track changes
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(ui->lineEdit_2, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(ui->lineEdit_3, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
}

void MainWindow::onSliderMoved(int value)
{
    ui->label_ip->setText(QString("Slider Value: %1").arg(value));
}

void MainWindow::onTextChanged(const QString &text)
{
    // Example logic for handling text changes
    qDebug() << "Text changed to:" << text;
}
