#include "windowcapturer.h"
#include <zmq.h>
#include <QScreen>
#include <QTimer>
#include <QtDebug>
#include <QLoggingCategory>
#include <QBuffer>

Q_LOGGING_CATEGORY(log_wc, "windowcapture");

WindowCapturer::WindowCapturer(QObject *parent)
    : QObject{parent}
{
    m_imageSender = new ImageSender();
    m_imageSender->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::started, m_imageSender, &ImageSender::initialize);
    connect(&m_workerThread, &QThread::finished, m_imageSender, &QObject::deleteLater);
    connect(this, &WindowCapturer::screenCaptured, m_imageSender, &ImageSender::onScreenCaptured);
    m_workerThread.start();

    m_captureTimer.setInterval(1000/30);
    connect(&m_captureTimer, &QTimer::timeout, this, &WindowCapturer::onNeedCapturing);
}

WindowCapturer::~WindowCapturer()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void WindowCapturer::startCapturing(QString savePath)
{
    qCInfo(log_wc)<<"start capture window:"<<m_capturedWidth<<"*"<<m_capturedHeight;
    m_captureTimer.start();
}

void WindowCapturer::stopCapturing()
{
    m_captureTimer.stop();
}

void WindowCapturer::onNeedCapturing()
{
    auto screenshot = m_screen->grabWindow();
    auto scaledScreenshot = screenshot.scaled(QSize(m_capturedWidth, m_capturedHeight), Qt::KeepAspectRatio);
    qCDebug(log_wc)<<"Window grabbed!";
    emit screenCaptured(scaledScreenshot);
}

ImageSender::ImageSender(QObject *parent)
    : QObject(parent)
{

}

ImageSender::~ImageSender()
{
    if(m_socket){
        zmq_close(m_socket);
        zmq_ctx_destroy(m_context);
    }
}

void ImageSender::initialize()
{
    m_context = zmq_ctx_new();
    m_socket = zmq_socket(m_context, ZMQ_PUB);
    int rc = zmq_bind (m_socket, "tcp://*:5556");
    Q_ASSERT(rc == 0);
    qCInfo(log_wc)<<"Image sender initialized!";
}

void ImageSender::onScreenCaptured(QPixmap screenshot)
{
    static int index = 0;
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    screenshot.save(&buffer, "BMP"); // writes pixmap into bytes in PNG format
    qCDebug(log_wc)<<"new screen shot arrived with bytes:"<<bytes.size();
    zmq_send(m_socket, bytes.constData(), bytes.size(), 0);
}

QWindow *WindowCapturer::window() const
{
    return m_window;
}

void WindowCapturer::setWindow(QWindow *newWindow)
{
    if (m_window == newWindow)
        return;
    m_window = newWindow;
    m_screen = m_window->screen();
    Q_ASSERT(m_screen);
    emit windowChanged();
}

int WindowCapturer::capturedWidth() const
{
    return m_capturedWidth;
}

void WindowCapturer::setCapturedWidth(int newCapturedWidth)
{
    if (m_capturedWidth == newCapturedWidth)
        return;
    m_capturedWidth = newCapturedWidth;
    emit capturedWidthChanged();
}

int WindowCapturer::capturedHeight() const
{
    return m_capturedHeight;
}

void WindowCapturer::setCapturedHeight(int newCapturedHeight)
{
    if (m_capturedHeight == newCapturedHeight)
        return;
    m_capturedHeight = newCapturedHeight;
    emit capturedHeightChanged();
}
