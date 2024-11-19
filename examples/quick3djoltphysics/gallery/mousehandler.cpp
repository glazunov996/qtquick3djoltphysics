#include "mousehandler.h"

#include <QCursor>
#include <QScreen>
#include <QDebug>

MouseHandler::MouseHandler(QObject *parent) : QObject(parent)
{
}

void MouseHandler::grabMouse(QQuickWindow *window)
{
    //window->setMouseGrabEnabled(true);
    //QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    m_lastMousePos = QCursor::pos();
    m_grabMousePos = m_lastMousePos;
}

void MouseHandler::releaseMouse(QQuickWindow *window)
{
    //window->setMouseGrabEnabled(false);
    //QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
    QCursor::setPos(m_grabMousePos.toPoint());
}

QPointF MouseHandler::getMouseDelta()
{
    const auto pos = QCursor::pos();
    const auto delta = m_lastMousePos - pos;

    m_lastMousePos = pos;

    const auto *screen = QGuiApplication::primaryScreen();
    const auto geometry = screen->availableGeometry();
    if (m_lastMousePos.x() <= geometry.left() || m_lastMousePos.x() >= geometry.right() - 1
            || m_lastMousePos.y() <= geometry.top() || m_lastMousePos.y() >= geometry.bottom() - 1) {
        m_lastMousePos = screen->availableGeometry().center();
        QCursor::setPos(m_lastMousePos.toPoint());
    }

    return delta;
}
