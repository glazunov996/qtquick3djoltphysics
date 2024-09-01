#include "collisiongroup_p.h"

CollisionGroup::CollisionGroup(QObject *parent) : QObject(parent)
{
}

CollisionGroup::~CollisionGroup() = default;

GroupFilterTable *CollisionGroup::groupFilterTable() const
{
    return m_groupFilterTable;
}

void CollisionGroup::setGroupFilterTable(GroupFilterTable *groupFilterTable)
{
    if (m_groupFilterTable == groupFilterTable)
        return;

    if (m_groupFilterTable)
        m_groupFilterTable->disconnect(m_groupFilterTableSignalConnection);

    m_groupFilterTable = groupFilterTable;

    m_groupFilterTableSignalConnection = QObject::connect(m_groupFilterTable, &GroupFilterTable::changed, this,
                    [this]
    {
        const auto filter = m_groupFilterTable->getJoltGroupFilterTable();
        m_collisionGroup.SetGroupFilter(filter);
        emit changed();
    });

    QObject::connect(m_groupFilterTable, &QObject::destroyed, this,
                    [this]
    {
        m_groupFilterTable = nullptr;
        m_collisionGroup.SetGroupFilter(nullptr);
        emit changed();
    });

    const auto filter = m_groupFilterTable->getJoltGroupFilterTable();
    m_collisionGroup.SetGroupFilter(filter);

    emit groupFilterTableChanged(m_groupFilterTable);
    emit changed();
}

unsigned int CollisionGroup::groupId() const
{
    return m_groupId;
}

void CollisionGroup::setGroupId(unsigned int groupId)
{
    if (m_groupId == groupId)
        return;

    m_groupId = groupId;
    m_collisionGroup.SetGroupID(m_groupId);

    emit groupIdChanged(m_groupId);
    emit changed();
}

unsigned int CollisionGroup::subGroupId() const
{
    return m_subGroupId;
}

void CollisionGroup::setSubGroupId(unsigned int subGroupId)
{
    if (m_subGroupId == subGroupId)
        return;

    m_subGroupId = subGroupId;
    m_collisionGroup.SetSubGroupID(m_subGroupId);

    emit subGroupIdChanged(m_subGroupId);
    emit changed();
}

JPH::CollisionGroup CollisionGroup::getJoltCollisionGroup() const
{
    return m_collisionGroup;
}
