#include "settings.h"
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

Settings::Settings() {
    loadSettings(); // Uygulama başladığında ayarları yükle
}

// Ayarları settings.json dosyasından yükleyen fonksiyon
void Settings::loadSettings() {
    QFile loadFile("settings.json");

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray saveData = loadFile.readAll();

    // JSON verisini bir QJsonDocument nesnesine dönüştürme
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    if (loadDoc.isNull()) {
        loadFile.close();
        return;
    }

    // JSON belgesinden bir QJsonObject oluşturma
    QJsonObject loadObj = loadDoc.object();

    // recordSpeed değerini al ve doğrulama
    if (loadObj.contains("recordSpeed") && loadObj["recordSpeed"].isDouble()) {
        int value = loadObj["recordSpeed"].toInt();

        if (value >= 0) {
            recordSpeed = value; 
        } else {
            recordSpeed = 99; 
        }
    } else {
        recordSpeed = 99; 
    }

    // visualizationSpeed değerini alma ve doğrulama
    if (loadObj.contains("visualizationSpeed") && loadObj["visualizationSpeed"].isDouble()) {
        int value = loadObj["visualizationSpeed"].toInt();

        if (value >= 0) {
            visualizationSpeed = value;
        } else {
            visualizationSpeed = 99;
        }
    } else {
        visualizationSpeed = 99;
    }

    loadFile.close();
}

// Ayarları settings.json dosyasına kaydeden fonksiyon
void Settings::saveSettings() {
    QFile saveFile("settings.json");

    if (!saveFile.open(QIODevice::WriteOnly)) {
        return;
    }

    // Kaydedilecek JSON nesnesini oluşturma
    QJsonObject saveObj;
    saveObj["recordSpeed"] = recordSpeed;
    saveObj["visualizationSpeed"] = visualizationSpeed;

    // JSON belgesini oluşturma ve dosyaya yazma
    QJsonDocument saveDoc(saveObj);
    saveFile.write(saveDoc.toJson());

    saveFile.close();
}

// Getter ve Setter fonksiyonları
void Settings::setRecordSpeed(int speed) {
    recordSpeed = speed;
}

void Settings::setVisualizationSpeed(int speed) {
    visualizationSpeed = speed;
}

int Settings::getRecordSpeed() const {
    return recordSpeed;
}

int Settings::getVisualizationSpeed() const {
    return visualizationSpeed;
}
