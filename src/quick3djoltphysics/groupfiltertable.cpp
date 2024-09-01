#include "groupfiltertable_p.h"

GroupFilterTable::GroupFilterTable(QObject *parent) : QObject(parent)
{
    m_groupFilterTable = new JPH::GroupFilterTable();
}

GroupFilterTable::~GroupFilterTable() = default;

void GroupFilterTable::componentComplete()
{
    m_componentComplete = true;
}

unsigned int GroupFilterTable::numSubGroups() const
{
    return m_numSubGroups;
}

void GroupFilterTable::setNumSubGroups(unsigned int numSubGroups)
{
    if (m_numSubGroups == numSubGroups)
        return;

    m_numSubGroups = numSubGroups;
    m_groupFilterTable = new JPH::GroupFilterTable(m_numSubGroups);

    emit numSubGroupsChanged(m_numSubGroups);
    emit changed();
}

void GroupFilterTable::disableCollision(unsigned int subGroup1, unsigned int subGroup2)
{
    if (!m_componentComplete) {
        qWarning() << "Warning: calling 'disableCollision' before GroupFilterTable is complete will have "
                      "no effect";
        return;
    }

    m_groupFilterTable->DisableCollision(subGroup1, subGroup2);
}

void GroupFilterTable::enableCollision(unsigned int subGroup1, unsigned int subGroup2)
{
    if (!m_componentComplete) {
        qWarning() << "Warning: calling 'enableCollision' before GroupFilterTable is complete will have "
                      "no effect";
        return;
    }

    m_groupFilterTable->EnableCollision(subGroup1, subGroup2);
}

JPH::Ref<JPH::GroupFilterTable> GroupFilterTable::getJoltGroupFilterTable() const
{
    return m_groupFilterTable;
}
