#ifndef QTQUICK3DJOLTPHYSICSGLOBAL_P_H
#define QTQUICK3DJOLTPHYSICSGLOBAL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qtquick3djoltphysicsglobal.h"
#include <QQmlEngine>

namespace Quick3dJoltPhysics
{
    Q_NAMESPACE_EXPORT(Q_QUICK3DJOLTPHYSICS_EXPORT)
    QML_NAMED_ELEMENT(JoltPhysics)

    enum class BackFaceMode {
        IgnoreBackFaces,
        CollideWithBackFaces,
    };
    Q_ENUM_NS(BackFaceMode)

    enum class ActiveEdgeMode {
        CollideOnlyWithActive,
        CollideWithAll,
    };
    Q_ENUM_NS(ActiveEdgeMode)

    enum CollectFacesMode {
        CollectFaces,
        NoFaces
    };
    Q_ENUM_NS(CollectFacesMode)
}

#endif // QTQUICK3DJOLTPHYSICSGLOBAL_P_H
