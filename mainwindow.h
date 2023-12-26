#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QTableView>
#include <QVector>

class Patient {
public:
    QString id;
    QString lastName;
    QString firstName;
    QString middleName;
    int birthYear;
    int height;
    int weight;
};

class PatientTableModel : public QAbstractTableModel {
public:
    PatientTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void addPatient(const Patient &patient);
    void removePatient(int row);
    QVector<Patient> getPatients() const;

private:
    QVector<Patient> patients;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addPatient();
    void removePatient();
    void saveDataToFile();

private:
    void setupUi();

    QTableView *tableView;
    PatientTableModel *patientModel;
};

#endif
