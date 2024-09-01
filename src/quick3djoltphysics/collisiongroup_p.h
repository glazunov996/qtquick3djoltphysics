#ifndef COLLISIONGROUP_P_H
#define COLLISIONGROUP_P_H

#include "groupfiltertable_p.h"

#include <QtQuick3DJoltPhysics/qtquick3djoltphysicsglobal.h>
#include <QtQml/QQmlEngine>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/CollisionGroup.h>

class Q_QUICK3DJOLTPHYSICS_EXPORT CollisionGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GroupFilterTable *groupFilterTable READ groupFilterTable WRITE setGroupFilterTable NOTIFY groupFilterTableChanged)
    Q_PROPERTY(unsigned int groupId READ groupId WRITE setGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(unsigned int subGroupId READ subGroupId WRITE setSubGroupId NOTIFY subGroupIdChanged)
    QML_NAMED_ELEMENT(CollisionGroup)
public:
    explicit CollisionGroup(QObject *parent = nullptr);
    ~CollisionGroup() override;

    GroupFilterTable *groupFilterTable() const;
    void setGroupFilterTable(GroupFilterTable *groupFilterTable);
    unsigned int groupId() const;
    void setGroupId(unsigned int groupId);
    unsigned int subGroupId() const;
    void setSubGroupId(unsigned int subGroupId);

signals:
    void groupFilterTableChanged(GroupFilterTable *groupFilterTable);
    void groupIdChanged(unsigned int groupId);
    void subGroupIdChanged(unsigned int subGroupId);
    void changed();

private:
    JPH::CollisionGroup getJoltCollisionGroup() const;
    friend class Body;

    GroupFilterTable *m_groupFilterTable = nullptr;
    QMetaObject::Connection m_groupFilterTableSignalConnection;
    unsigned int m_groupId = -1;
    unsigned int m_subGroupId = -1;
    JPH::CollisionGroup m_collisionGroup;
};

#endif // COLLISIONGROUP_P_H
