#ifndef CURSOR_H
#define CURSOR_H

#include <QObject>
#include <QPoint>

class Cursor : public QObject
{
    Q_OBJECT
public:
    explicit Cursor(QObject *parent = nullptr);

    Q_INVOKABLE void hide() const;
    Q_INVOKABLE void show() const;

    Q_INVOKABLE QPoint pos() const;
    Q_INVOKABLE void setPos(const QPoint &pos);
};

#endif // CURSOR_H
