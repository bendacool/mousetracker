#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

// Settings dosyasını oluşturan fonksiyon
void createSettingsFile() {
    QFile settingsFile("settings.json");

    // Eğer settings dosyası mevcut değilse oluştur
    if (!settingsFile.exists()) {
        if (settingsFile.open(QIODevice::WriteOnly)) {
            QJsonObject settings;
            settings["recordSpeed"] = 99;           // Varsayılan kayıt hızı
            settings["visualizationSpeed"] = 99;    // Varsayılan görselleştirme hızı

            QJsonDocument doc(settings);
            settingsFile.write(doc.toJson());       
            settingsFile.close();                   
        }
    }
}

int main(int argc, char *argv[])
{
    createSettingsFile();          
    QApplication a(argc, argv);    
    MainWindow w;                  
    w.show();                      
    return a.exec();               
}
