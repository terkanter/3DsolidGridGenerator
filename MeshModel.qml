import Qt3D.Core 2.0
import Qt3D.Render 2.0

Entity {
    id: root

    property real x: 0.0
    property real y: 0.0
    property real z: 0.0
    property real scale: 1.0
    property real theta: 90.0
    property real phi: 0.0
    property Material material

    components: [ transform, mesh, root.material ]

    Transform {
        id: transform
        translation: Qt.vector3d(root.x, root.y, root.z)
        rotation: fromEulerAngles(theta, phi, 0)
        scale: root.scale
    }

    Mesh {
        id: mesh
        source: meshPath
    }
}
