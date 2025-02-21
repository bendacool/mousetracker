#ifndef VISUALIZATIONWINDOW_H
#define VISUALIZATIONWINDOW_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPen>
#include <QBrush>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "settings.h"

class VisualizationWindow : public QDialog {
    Q_OBJECT

public:
    explicit VisualizationWindow(int screenWidth, int screenHeight, QWidget *parent = nullptr, Settings *settings = nullptr);
    void drawPoints(const QList<QPoint> &points);
    void loadLogData(const QDateTime &startDate, const QDateTime &endDate);

private:
    int screenWidth;
    int screenHeight;
    double widthRatio;
    double heightRatio;
    int currentIndex;
    bool selecting;
    QRect* selectionRect;
    QTimer *timer;
    QList<QPoint> allPoints;
    Settings *settings;

    QPushButton *startButton;
    QPushButton *heatmapButton;
    QGraphicsScene *scene;
    QGraphicsView *view;

    void drawStartAndEnd();
    void startSimulation();
    void updateSimulation();
    void showHeatmap();
    void initSelection();
    void processSelectedArea(const QRectF &selectedArea);
};

#endif // VISUALIZATIONWINDOW_H
