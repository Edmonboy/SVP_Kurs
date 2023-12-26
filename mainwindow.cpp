#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <qmessagebox.h>

PatientTableModel::PatientTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int PatientTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return patients.size();
}

int PatientTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 6;
}

QVariant PatientTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= patients.size() || index.column() >= 6)
        return QVariant();

    const Patient &patient = patients[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: return patient.lastName;
        case 1: return patient.firstName;
        case 2: return patient.middleName;
        case 3: return patient.birthYear;
        case 4: return patient.height;
        case 5: return patient.weight;
        default: break;
        }
    }

    return QVariant();
}

QVariant PatientTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return tr("Фамилия");
        case 1: return tr("Имя");
        case 2: return tr("Отчество");
        case 3: return tr("Год рождения");
        case 4: return tr("Рост");
        case 5: return tr("Вес");
        default: break;
        }
    }

    return QVariant();
}

Qt::ItemFlags PatientTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool PatientTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        Patient &patient = patients[index.row()];
        switch (index.column()) {
        case 0: patient.lastName = value.toString(); break;
        case 1: patient.firstName = value.toString(); break;
        case 2: patient.middleName = value.toString(); break;
        case 3: patient.birthYear = value.toInt(); break;
        case 4: patient.height = value.toInt(); break;
        case 5: patient.weight = value.toInt(); break;
        default: break;
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}

void PatientTableModel::addPatient(const Patient &patient) {
    beginInsertRows(QModelIndex(), patients.size(), patients.size());
    patients.push_back(patient);
    endInsertRows();
}

void PatientTableModel::removePatient(int row) {
    if (row >= 0 && row < patients.size()) {
        beginRemoveRows(QModelIndex(), row, row);
        patients.removeAt(row);
        endRemoveRows();
    }
}

QVector<Patient> PatientTableModel::getPatients() const {
    return patients;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
    resize(630, 600);
}

void MainWindow::addPatient() {
    Patient patient;
    patient.lastName = "";
    patient.firstName = "";
    patient.middleName = "";
    patient.birthYear = 0;
    patient.height = 0;
    patient.weight = 0;

    patientModel->addPatient(patient);
}

void MainWindow::removePatient() {
    QModelIndexList selectedRows = tableView->selectionModel()->selectedRows();
    for (int i = selectedRows.size() - 1; i >= 0; --i) {
        int row = selectedRows[i].row();
        patientModel->removePatient(row);
    }
}

void MainWindow::saveDataToFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Data"), "", tr("Text Files (*.txt);;All Files (*)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            QVector<Patient> patients = patientModel->getPatients();
            for (const Patient &patient : patients) {
                out << patient.lastName << ","
                    << patient.firstName << ","
                    << patient.middleName << ","
                    << patient.birthYear << ","
                    << patient.height << ","
                    << patient.weight << "\n";
            }
            file.close();
            QMessageBox::information(this, tr("Information"), tr("Успешно сохранено!"));
        }
    }
}

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    tableView = new QTableView(this);
    patientModel = new PatientTableModel(this);
    tableView->setModel(patientModel);

    mainLayout->addWidget(tableView);

    QPushButton *addButton = new QPushButton(tr("Добавить пациента"), this);
    QPushButton *removeButton = new QPushButton(tr("Удалить пациента"), this);
    QPushButton *saveButton = new QPushButton(tr("Сохранить"), this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removePatient);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveDataToFile);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(saveButton);

    mainLayout->addLayout(buttonLayout);

    setCentralWidget(centralWidget);
}


