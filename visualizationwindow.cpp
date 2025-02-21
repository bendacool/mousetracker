#include "visualizationwindow.h"
#include <QVBoxLayout>
#include <QTimer>
#include <QPushButton>
#include <QTimer>
#include <QMap>
#include <QGraphicsTextItem>

VisualizationWindow::VisualizationWindow(int screenWidth, int screenHeight, QWidget *parent, Settings* settings)
    : QDialog(parent)
    , scene(new QGraphicsScene(this))
    , view(new QGraphicsView(scene, this))
    , screenWidth(screenWidth)
    , screenHeight(screenHeight)
    , timer(new QTimer(this))
    , currentIndex(0)
    , settings(settings)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);

    // Simülasyonu başlatacak butonu oluşturma
    startButton = new QPushButton("Start Simulation", this);
    layout->addWidget(startButton);

    // Heatmap butonunu ekleme
    heatmapButton = new QPushButton("Show Heatmap", this);
    layout->addWidget(heatmapButton);

    setLayout(layout);
    setWindowTitle("Mouse Movement Visualization");
    resize(800, 600);

    // Ekran boyutunu pencere boyutuna orantıla
    widthRatio = static_cast<double>(width()) / screenWidth;
    heightRatio = static_cast<double>(height()) / screenHeight;

    // Start butonuna tıklama işlemi ile simülasyonu başlatma
    connect(startButton, &QPushButton::clicked, this, &VisualizationWindow::startSimulation);

    // Heatmap butonuna tıklama işlemi
    connect(heatmapButton, &QPushButton::clicked, this, &VisualizationWindow::showHeatmap);

    // Zamanlayıcı her tetiklendiğinde simülasyonu güncelleme
    connect(timer, &QTimer::timeout, this, &VisualizationWindow::updateSimulation);
}

// Başlangıç ve bitiş noktalarını çizme
void VisualizationWindow::drawStartAndEnd()
{
    scene->addEllipse(allPoints.first().x() * widthRatio - 4, allPoints.first().y() * heightRatio - 4,
                      8, 8, QPen(Qt::green), QBrush(Qt::green));
    scene->addEllipse(allPoints.last().x() * widthRatio - 4, allPoints.last().y() * heightRatio - 4,
                      8, 8, QPen(Qt::red), QBrush(Qt::red));
}

// Noktalar listesini çizme
void VisualizationWindow::drawPoints(const QList<QPoint> &points)
{
    allPoints = points;
    scene->clear();
    currentIndex = 0;

    for (int i = 0; i < points.size() - 1; i++) {
        scene->addLine(points[i].x() * widthRatio, points[i].y() * heightRatio,
                       points[i+1].x() * widthRatio, points[i+1].y() * heightRatio, QPen(Qt::blue));
    }

    if (!allPoints.isEmpty()) {
        drawStartAndEnd();
    }
}

// Simülasyonu başlatma
void VisualizationWindow::startSimulation()
{
    if (allPoints.isEmpty()) return;

    scene->clear();
    drawStartAndEnd();

    startButton->setEnabled(false);
    currentIndex = 0;
    timer->start(settings->getVisualizationSpeed());
}

// Simülasyonu güncelleme
void VisualizationWindow::updateSimulation()
{
    if (currentIndex >= allPoints.size() - 1) {
        startButton->setEnabled(true);
        timer->stop();
        return;
    }

    QPen pen(Qt::blue);
    pen.setWidth(2);
    scene->addLine(allPoints[currentIndex].x() * widthRatio, allPoints[currentIndex].y() * heightRatio,
                   allPoints[currentIndex + 1].x() * widthRatio, allPoints[currentIndex + 1].y() * heightRatio, pen);
    currentIndex++;
}

// Log verisini yükle ve simülasyona aktarma
void VisualizationWindow::loadLogData(const QDateTime &startDate, const QDateTime &endDate) {
    QList<QPoint> points;

    for (QDateTime date = startDate; date <= endDate; date = date.addDays(1)) {
        QString logFileName = date.toString("yyyy-MM-dd") + "_mouse_coordinates.lo";
        QFile file(logFileName);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains("(x,y)=")) {
                    QStringList split = line.split("(x,y)=");
                    QString timestamp = split.first().left(19).trimmed();
                    QDateTime logDate = QDateTime::fromString(timestamp, "yyyy-MM-dd HH:mm:ss");

                    if (!logDate.isValid() || logDate < startDate || logDate > endDate)
                        continue;

                    QString coords = split.last();
                    QStringList xy = coords.split(",");
                    if (xy.size() == 2) {
                        int x = xy[0].toInt();
                        int y = xy[1].toInt();
                        points.append(QPoint(x, y));
                    }
                }
            }
            file.close();
        }
    }

    drawPoints(points);
}


// Heatmap gösterimi
void VisualizationWindow::showHeatmap() {
    if (allPoints.isEmpty()) return;

    QMap<QPair<int, int>, int> heatmap;

    for (const QPoint &point : allPoints) {
        QPair<int, int> adjustedPoint(point.x() * widthRatio, point.y() * heightRatio);
        heatmap[adjustedPoint]++;
    }

    scene->clear();

    for (auto it = heatmap.begin(); it != heatmap.end(); ++it) {
        int intensity = qMin(it.value() * 10, 255);
        QColor color(255, 0, 0, intensity);
        scene->addEllipse(it.key().first - 4, it.key().second - 4, 8, 8, QPen(color), QBrush(color));
    }
}
