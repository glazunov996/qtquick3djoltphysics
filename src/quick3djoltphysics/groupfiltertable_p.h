#ifndef GROUPFILTERTABLE_P_H
#define GROUPFILTERTABLE_P_H

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>
#include <Jolt/Physics/Collision/GroupFilterTable.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT GroupFilterTable : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(unsigned int numSubGroups READ numSubGroups WRITE setNumSubGroups NOTIFY numSubGroupsChanged)
    QML_NAMED_ELEMENT(GroupFilterTable)
public:
    explicit GroupFilterTable(QObject *parent = nullptr);
    ~GroupFilterTable() override;

    void classBegin() override {}
    void componentComplete() override;

    unsigned int numSubGroups() const;
    void setNumSubGroups(unsigned int numSubGroups);

    Q_INVOKABLE void disableCollision(unsigned int subGroup1, unsigned int subGroup2);
    Q_INVOKABLE void enableCollision(unsigned int subGroup1, unsigned int subGroup2);

signals:
    void numSubGroupsChanged(unsigned int numSubGroups);
    void changed();

private:
    JPH::Ref<JPH::GroupFilterTable> getJoltGroupFilterTable() const;
    friend class CollisionGroup;

    unsigned int m_numSubGroups = 0;
    JPH::Ref<JPH::GroupFilterTable> m_groupFilterTable = nullptr;
    bool m_componentComplete = false;
};

#endif // GROUPFILTERTABLE_P_H
