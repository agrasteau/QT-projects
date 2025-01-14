#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStandardItemModel>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timer(new QTimer(this)),
    chronoTime(QTime(0, 0))
{
    ui->setupUi(this);

    setupTableView();
    //ui->lcdNumber->setVisible(false); // Masque le LCD
    ui->lcdNumber->display("00:00:00");

    connect(timer, &QTimer::timeout, this, &MainWindow::updateDisplay);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportToCSV);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupTableView()
{
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Temps");
    model->setHeaderData(1, Qt::Horizontal, "Ecart");
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::updateDisplay()
{
    chronoTime = chronoTime.addSecs(1);

    // Formater l'heure sous la forme "hh:mm:ss"
    QString timeText = chronoTime.toString("hh:mm:ss");

    // Vérifier si la chaîne est valide avant de l'afficher
    if (timeText.isEmpty()) {
        qDebug() << "Erreur : chaîne de temps vide!";
    } else {
        ui->lcdNumber->display(timeText);
        qDebug() << timeText ;

    }
}

void MainWindow::onStartButtonClicked()
{
    if (isRunning) {
        // Si le chronomètre est en cours, arrêtez-le
        timer->stop();  // Arrêter le chronomètre
        isRunning = false;  // Mettre à jour l'état du chronomètre
        QString currentTime = chronoTime.toString("hh:mm:ss");
        // Calculer l'écart de temps entre lastTime et chronoTime
        int elapsedSeconds = lastTime.secsTo(chronoTime);
        // Convertir l'écart en format "hh:mm:ss"
        QTime elapsedTime = QTime(0, 0).addSecs(elapsedSeconds);
        QString elapsedTimeString = elapsedTime.toString("hh:mm:ss");
        addRow(currentTime, elapsedTimeString);  // Ajouter le temps et l'écart à la table


    } else {
        // Si le chronomètre est arrêté, démarrez-le
        ui->tableView->model()->removeRows(0, ui->tableView->model()->rowCount());
        chronoTime = QTime(0, 0);  // Réinitialiser le temps
        lastTime = QTime(0, 0);    // Réinitialiser lastTime
        ui->lcdNumber->display("00:00:00");  // Réinitialiser l'affichage
        timer->start(1000);  // Démarrer la mise à jour toutes les secondes
        isRunning = true;  // Mettre à jour l'état du chronomètre
    }
}

void MainWindow::onStopButtonClicked()
{

    QString currentTime = chronoTime.toString("hh:mm:ss");


    QString elapsedTime;
        if (lastTime.isNull()) {
            elapsedTime = "00:00:00";  // Pas d'écart à afficher
        } else {
            // Calculer l'écart entre lastTime et chronoTime
            int elapsedSecs = lastTime.secsTo(chronoTime);  // L'écart en secondes
            QTime elapsed = QTime(0, 0).addSecs(elapsedSecs);  // Convertir en QTime
            elapsedTime = elapsed.toString("hh:mm:ss");
        }
    addRow(currentTime, elapsedTime);
        lastTime = chronoTime;
}

void MainWindow::addRow(const QString &time, const QString &gap)
{
    QList<QStandardItem *> rowItems;
    rowItems << new QStandardItem(time) << new QStandardItem(gap);
    rowItems.at(0)->setTextAlignment(Qt::AlignCenter);
    rowItems.at(1)->setTextAlignment(Qt::AlignCenter);
    model->appendRow(rowItems);
}


void MainWindow::exportToCSV()
{
    // Ouvrir une boîte de dialogue pour choisir où enregistrer le fichier CSV
    QString fileName = QFileDialog::getSaveFileName(this, tr("Enregistrer sous"), "", tr("Fichiers CSV (*.csv)"));

    if (fileName.isEmpty()) {
        return;  // Si l'utilisateur annule la boîte de dialogue, on ne fait rien
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier"));
        return;
    }

    QTextStream out(&file);

    // Récupérer le modèle de données du tableau
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());

    if (!model) {
        QMessageBox::critical(this, tr("Erreur"), tr("Le modèle de données n'est pas valide"));
        return;
    }

    // Parcourir les lignes et les colonnes du modèle pour construire le contenu CSV
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;

        for (int col = 0; col < model->columnCount(); ++col) {
            // Ajouter les données de la cellule à la liste
            rowData.append(model->item(row, col)->text());
        }

        // Joindre les éléments avec une virgule et les ajouter au fichier
        out << rowData.join(",") << "\n";
    }

    file.close();

    // Afficher un message de succès
    QMessageBox::information(this, tr("Succès"), tr("Le fichier CSV a été enregistré avec succès."));
}
