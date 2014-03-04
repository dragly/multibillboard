import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import CompPhys.MultiBillboard 1.0
import CompPhys.MultiBillboardDemo 1.0

Viewport  {
    id: rootViewport

    property real fps: 1
    property real lastDraw: 0

    width: 1280
    height: 720

    fillColor: "black"
    blending: true
    fovzoom: false

    Component.onCompleted: {
        forceActiveFocus()
    }

    light: Light {
        ambientColor: Qt.rgba(1,1,1,1)
        position.x: myCamera.eye.x
        position.y: myCamera.eye.y
        position.z: myCamera.eye.z
        linearAttenuation: 0.08
    }

    camera: Camera {
        id: myCamera
        eye: Qt.vector3d(50,50,50)
        nearPlane: 5
        farPlane: 5000
    }

    ExampleDataSource {
        id: exampleDataSource
        numPointsPerDimension: 50
    }

    MultiBillboard {
        id: multiSphere
        dataSource: exampleDataSource
        hasPeriodicCopies: true
////        cullFaces: Item3D.CullBackFaces
////        sortPoints: Item3D.BackToFront

        texture: "sphere2.png"
    }

    FrameRateCounter {
        id: frameRate
    }

    Text {
        color: "white"
        text: "FPS: " + frameRate.fps.toFixed(2)
    }

    Timer {
        running: true
        repeat: true
        interval: 1
        onTriggered: {
            myCamera.tiltPanRollCenter(0,0.01,0)
        }
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_P) {
            multiSphere.hasPeriodicCopies = !multiSphere.hasPeriodicCopies
        }
    }
}
