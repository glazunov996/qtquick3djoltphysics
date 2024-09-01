#include "cursor.h"

#include <QCursor>
#include <QGuiApplication>

Cursor::Cursor(QObject *parent) : QObject(parent) {}

void Cursor::hide() const
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
}

void Cursor::show() const
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

QPoint Cursor::pos() const
{
    return QCursor::pos();
}

void Cursor::setPos(const QPoint &pos)
{
    QCursor::setPos(pos);
}
