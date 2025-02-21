#include "visualizationwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easylogging++.h"
#include <ctime>
#include <QTextStream>
#include <QScreen>
#include "settings.h"
#include "settingsdialog.h"

// EasyLogging++'ı başlatma
INITIALIZE_EASYLOGGINGPP

// Log dosyasının adını zaman damgasına göre oluşturma
std::string getLogFileName() {
    char buffer[32];
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_mouse_coordinates.lo", &tm) == 0) {
        // Hata durumunda varsayılan bir isim kullan
        return "mouse_coordinates.lo";
    }
    return std::string(buffer);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , recording(false)
    , visualizationWindow(nullptr)
    , settings(new Settings())
{
    ui->setupUi(this);
    ui->stopButton->setEnabled(false); // Başlangıçta "stop" butonunu devre dışı bırakma

    // Başlangıç ve bitiş tarihlerini belirleme
    QDateTime todayStart = QDateTime::currentDateTime();
    todayStart.setTime(QTime::fromString("00:00:00", "HH:mm:ss"));
    ui->startDateEdit->setDateTime(todayStart);

    QDateTime todayEnd = QDateTime::currentDateTime();
    todayEnd.setTime(QTime::fromString("23:59:59", "HH:mm:ss"));
    ui->endDateEdit->setDateTime(todayEnd);

    // Grafik sahnesi oluşturma
    scene = new QGraphicsScene(this);

    // Butonlara tıklama olaylarını bağlama
    connect(ui->visualizeButton, &QPushButton::clicked, this, &MainWindow::onShowVisualizationClicked);
    connect(ui->recordButton, &QPushButton::clicked, this, &MainWindow::onRecordButtonClicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::onStopButtonClicked);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

    // EasyLogging++ yapılandırmasını ayarlama
    el::Configurations conf;
    conf.setToDefault();
    conf.set(el::Level::Info, el::ConfigurationType::Filename, getLogFileName());
    conf.set(el::Level::Info, el::ConfigurationType::Format, "%datetime (x,y)=%msg");
    conf.set(el::Level::Info, el::ConfigurationType::ToFile, "true");
    el::Loggers::reconfigureLogger("default", conf);

    // Ekran boyutunu alma
    QSize screenSize = QApplication::primaryScreen()->size();
    screenWidth = screenSize.width();
    screenHeight = screenSize.height();

    // Log zamanlayıcısını başlatma
    logTimer = new QTimer(this);
    connect(logTimer, &QTimer::timeout, [this]() {
        if (recording) {
            QPoint pos = QCursor::pos(); // Fare pozisyonunu alma
            LOG(INFO) << pos.x() << "," << pos.y();
        }
    });
}

MainWindow::~MainWindow()
{
    if (recording) {
        recording = false;
        logTimer->stop();
    }
    delete ui;
    delete visualizationWindow;
    delete settings;
}

// Görselleştirme penceresini açma
void MainWindow::onShowVisualizationClicked()
{
    QDateTime startDate = ui->startDateEdit->dateTime();
    QDateTime endDate = ui->endDateEdit->dateTime();

    if (startDate >= endDate) {
        return;
    }

    if (!visualizationWindow) {
        visualizationWindow = new VisualizationWindow(screenWidth, screenHeight, this, settings);
    }
    visualizationWindow->loadLogData(startDate, endDate);
    visualizationWindow->show();
}

// Kayıt ve durdur butonlarının durumunu değiştirme
void MainWindow::switchRecordStatement()
{
    bool recordBtnEnabled = ui->recordButton->isEnabled();
    bool stopBtnEnabled = ui->stopButton->isEnabled();
    ui->recordButton->setEnabled(!recordBtnEnabled);
    ui->stopButton->setEnabled(!stopBtnEnabled);
}

// Fare hareketi olayını yakala ve loglama
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (recording) {
        QPoint pos = event->pos();
        LOG(INFO) << pos.x() << "," << pos.y();
    }
}

// Kayıt butonuna tıklanması durumunda
void MainWindow::onRecordButtonClicked()
{
    switchRecordStatement();
    recording = true;
    logTimer->start(settings->getRecordSpeed()); // Zamanlayıcıyı başlatma
}

// Durdur butonuna tıklanması durumunda
void MainWindow::onStopButtonClicked()
{
    switchRecordStatement();
    recording = false;
    logTimer->stop();
}

void MainWindow::onSettingsButtonClicked()
{
    SettingsDialog settingsDialog(this, settings);
    settingsDialog.exec();
}
