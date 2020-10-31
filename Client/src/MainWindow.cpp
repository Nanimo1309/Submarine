#include "MainWindow.hpp"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>

#include <QMovie>
#include <QPixmap>
#include <QIcon>

#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include <QVideoFrame>

MainWindow::MainWindow(QObject* parent):
    QObject(parent)
{
    auto qmlEngine = new QQmlEngine(this);
    auto qmlComponent = new QQmlComponent(qmlEngine, ":MainWindow", qmlEngine);
    m_window = qobject_cast<QQuickWindow*>(qmlComponent->create());

    QObject::connect(m_window, &QQuickWindow::destroyed, this, &MainWindow::closed);

    auto icon = new QMovie(m_window);
    icon->setFileName(":AppIcon");
    QObject::connect(icon, &QMovie::frameChanged, [icon, this](int){m_window->setIcon(QIcon(icon->currentPixmap()));});
    icon->start();

    m_cameraDisplay = m_window->findChild<QObject*>("cameraDisplay")->property("videoSurface").value<QAbstractVideoSurface*>();
    m_cameraDisplay->start(QVideoSurfaceFormat(QSize(), QVideoFrame::Format_ARGB32));

    auto controler = m_window->findChild<QObject*>("controler");
    QObject::connect(controler, SIGNAL(rightMotor(double)), this, SIGNAL(rightMotor(double)));
    QObject::connect(controler, SIGNAL(leftMotor(double)), this, SIGNAL(leftMotor(double)));
    QObject::connect(controler, SIGNAL(immersion(double)), this, SIGNAL(immersion(double)));
}

void MainWindow::show()
{
    m_window->show();
}

void MainWindow::cameraFrame(const QVideoFrame& frame)
{
    m_cameraDisplay->present(frame);
}