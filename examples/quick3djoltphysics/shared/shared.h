#include <QQmlEngine>

#include "../shared/examplecharactercontactlistener.h"
#include "../shared/examplecontactlistener.h"
#include "../shared/examplegeometry.h"
#include "../shared/examplelayers.h"
#include "../shared/examplesensorcontactlistener.h"

static inline void registerSharedTypes()
{
    qmlRegisterType<ExampleCharacterContactListener>("Example", 1, 0, "ExampleCharacterContactListener");
    qmlRegisterType<ExampleContactListener>("Example", 1, 0, "ExampleContactListener");
    qmlRegisterType<ExampleTriangleGeometry>("Example", 1, 0, "ExampleTriangleGeometry");
    qmlRegisterType<ExampleMeshGridGeometry>("Example", 1, 0, "ExampleMeshGridGeometry");
    qmlRegisterType<ExampleStairsGridGeometry>("Example", 1, 0, "ExampleStairsGridGeometry");
    qmlRegisterType<ExampleObjectLayerPairFilter>("Example", 1, 0, "ExampleObjectLayerPairFilter");
    qmlRegisterType<ExampleBroadPhaseLayer>("Example", 1, 0, "ExampleBroadPhaseLayer");
    qmlRegisterType<ExampleObjectVsBroadPhaseLayerFilter>("Example", 1, 0, "ExampleObjectVsBroadPhaseLayerFilter");
    qmlRegisterType<ExampleSensorContactListener>("Example", 1, 0, "ExampleSensorContactListener");
}
