#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connecter les signaux et slots
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearFields);
    connect(ui->validateButton, &QPushButton::clicked, this, &MainWindow::validateAlcoholLevel);
    connect(ui->weightInput, &QLineEdit::textChanged, this, &MainWindow::updateValidateButtonState);
    connect(ui->beerQuantityInput, &QLineEdit::textChanged, this, &MainWindow::enableValidationIfNeeded);
    connect(ui->VineQuantityInput, &QLineEdit::textChanged, this, &MainWindow::enableValidationIfNeeded);
    connect(ui->AperoQuantityInput, &QLineEdit::textChanged, this, &MainWindow::enableValidationIfNeeded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearFields()
{
    ui->weightInput->clear();
    ui->beerQuantityInput->clear();
    ui->VineQuantityInput->clear();
    ui->AperoQuantityInput->clear();
    ui->resultLabel->clear();
}

void MainWindow::updateValidateButtonState()
{
    bool isWeightValid = !ui->weightInput->text().isEmpty();
    ui->validateButton->setEnabled(isWeightValid);
}

void MainWindow::enableValidationIfNeeded()
{
    // Permet la validation seulement si le poids n'est pas vide et toutes les quantités ont été remplies
    bool isWeightValid = !ui->weightInput->text().isEmpty();
    bool isBeerValid = !ui->beerQuantityInput->text().isEmpty();
    bool isVineValid = !ui->VineQuantityInput->text().isEmpty();
    bool isAperoValid = !ui->AperoQuantityInput->text().isEmpty();

    ui->validateButton->setEnabled(isWeightValid && (isBeerValid || isVineValid || isAperoValid));
}

void MainWindow::validateAlcoholLevel()
{
    double weight = ui->weightInput->text().toDouble();
    double beerQuantity = ui->beerQuantityInput->text().toDouble();
    double vineQuantity = ui->VineQuantityInput->text().toDouble();
    double aperoQuantity = ui->AperoQuantityInput->text().toDouble();

    double alcoholConsumed = 0.0;

    // Calcul de la quantité d'alcool selon les boissons
    if (beerQuantity > 0) {
        alcoholConsumed += beerQuantity * 5 / 100;  // 5% pour la bière
    }
    if (vineQuantity > 0) {
        alcoholConsumed += vineQuantity * 12 / 100;  // 12% pour le vin
    }
    if (aperoQuantity > 0) {
        alcoholConsumed += aperoQuantity * 35 / 100;  // 35% pour l'apéritif
    }

    // Déterminer le sexe et multiplier par le facteur correspondant
    double alcoholLevel = 0.0;
    if (ui->maleRadio->isChecked()) {
        alcoholLevel = alcoholConsumed * 11 / weight;  // Hommes : 11g d'alcool pour chaque kg
    } else {
        alcoholLevel = alcoholConsumed * 12 / weight;  // Femmes : 12g d'alcool pour chaque kg
    }

    // Affichage du résultat avec une couleur en fonction du taux d'alcoolémie
    ui->resultLabel->setText(QString("Taux d'alcoolémie: %1").arg(alcoholLevel));

    if (alcoholLevel > 0.5) {
        ui->resultLabel->setStyleSheet("background-color: red; color: white;");
    } else {
        ui->resultLabel->setStyleSheet("background-color: green; color: white;");
    }
}
