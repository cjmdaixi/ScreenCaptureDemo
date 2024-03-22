#ifndef WINDOWCAPTURER_H
#define WINDOWCAPTURER_H

#include <QObject>
#include <QQmlEngine>
#include <QWindow>
#include <QThread>
#include <QPixmap>
#include <QTimer>

class ImageSender : public QObject
{
    Q_OBJECT
public:
    ImageSender(QObject *parent = nullptr);
    ~ImageSender();

public slots:
    void initialize();
    void onScreenCaptured(QPixmap screenshot);
private:
    void* m_context = nullptr;
    void* m_socket = nullptr;
};

class WindowCapturer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    Q_PROPERTY(QWindow* window READ window WRITE setWindow NOTIFY windowChanged)
    Q_PROPERTY(int capturedWidth READ capturedWidth WRITE setCapturedWidth NOTIFY capturedWidthChanged FINAL)
    Q_PROPERTY(int capturedHeight READ capturedHeight WRITE setCapturedHeight NOTIFY capturedHeightChanged FINAL)

    explicit WindowCapturer(QObject *parent = nullptr);
    ~WindowCapturer();

    Q_INVOKABLE void startCapturing(QString savePath);
    Q_INVOKABLE void stopCapturing();
    QWindow *window() const;
    void setWindow(QWindow *newWindow);

    int capturedWidth() const;
    void setCapturedWidth(int newCapturedWidth);

    int capturedHeight() const;
    void setCapturedHeight(int newCapturedHeight);

private slots:
    void onNeedCapturing();
signals:
    void windowChanged();
    void screenCaptured(QPixmap screenshot);
    void capturedWidthChanged();

    void capturedHeightChanged();

private:
    ImageSender* m_imageSender = nullptr;
    QThread m_workerThread;
    QTimer m_captureTimer;
    QWindow *m_window = nullptr;
    QScreen* m_screen = nullptr;
    int m_capturedWidth;
    int m_capturedHeight;
};

#endif // WINDOWCAPTURER_H
