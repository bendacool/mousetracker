#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

class Settings
{
public:
    Settings();
    ~Settings() = default;

    void loadSettings();
    void saveSettings();

    int getRecordSpeed() const;
    int getVisualizationSpeed() const;

    void setRecordSpeed(int speed);
    void setVisualizationSpeed(int speed);

private:
    int recordSpeed;
    int visualizationSpeed;
    QString settingsFilePath;
};

#endif // SETTINGS_H
