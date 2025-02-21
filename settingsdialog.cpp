#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent, Settings *settings) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    settings(settings)
{
    ui->setupUi(this);

    // Save butonuna tıklama olayını bağlama
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsDialog::onSaveButtonClicked);

    // Başlangıçta mevcut ayarları yükle
    if (settings) {
        ui->recordSpeedSpinBox->setValue(settings->getRecordSpeed());
        ui->visualizationSpeedSpinBox->setValue(settings->getVisualizationSpeed());
    }
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onSaveButtonClicked()
{

    // Kayıt hızını ve görselleştirme hızını al
    int recordSpeed = ui->recordSpeedSpinBox->value();
    int visualizationSpeed = ui->visualizationSpeedSpinBox->value();

    // Ayarları güncelle
    settings->setRecordSpeed(recordSpeed);
    settings->setVisualizationSpeed(visualizationSpeed);

    // Ayarları dosyaya kaydet
    settings->saveSettings();

    // Dialogu kapatma
    accept();
}
