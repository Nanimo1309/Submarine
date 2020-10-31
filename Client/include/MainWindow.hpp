#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QObject>

class QQuickWindow;
class QAbstractVideoSurface;
class QVideoFrame;

class MainWindow:
    public QObject
{
    Q_OBJECT

public:
    MainWindow(QObject* parent);

    void show();

signals:
    void closed();

    void rightMotor(double power);
    void leftMotor(double power);
    void immersion(double depth);

public slots:
    void cameraFrame(const QVideoFrame& frame);

private:
    QQuickWindow* m_window;
    QAbstractVideoSurface* m_cameraDisplay;
};

#endif //MAINWINDOW_HPP