#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_settingsdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include "settings.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr, Settings *settings = nullptr);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    Settings *settings;

    void onSaveButtonClicked();
};

#endif // SETTINGSDIALOG_H
