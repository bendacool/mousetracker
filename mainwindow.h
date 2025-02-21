#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "visualizationwindow.h"
#include "settings.h"
#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QCursor>
#include <QGraphicsScene>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onRecordButtonClicked();
    void onStopButtonClicked();
    void onSettingsButtonClicked();
    void onShowVisualizationClicked();

private:
    Ui::MainWindow *ui;
    bool recording;
    QTimer *logTimer;
    QGraphicsScene *scene;
    VisualizationWindow *visualizationWindow;
    int screenWidth;
    int screenHeight;

    Settings *settings;

    void switchRecordStatement();
    void loadLogData(const QDateTime &startDate, const QDateTime &endDate);
    void plotMousePath(const QList<QPoint> &points);
};

#endif // MAINWINDOW_H
