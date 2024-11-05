#ifndef MOUSEHANDLER_H
#define MOUSEHANDLER_H

#include <QGuiApplication>
#include <QQuickWindow>
#include <QMouseEvent>
#include <QPointF>
#include <QObject>

class MouseHandler : public QObject
{
    Q_OBJECT
public:
    MouseHandler(QObject *parent = nullptr);

    Q_INVOKABLE void grabMouse(QQuickWindow *window);
    Q_INVOKABLE void releaseMouse(QQuickWindow *window);
    Q_INVOKABLE QPointF getMouseDelta();

private:
    QPointF m_lastMousePos;
    QPointF m_grabMousePos;
};

#endif // MOUSEHANDLER_H
