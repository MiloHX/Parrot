Scene: Utitled
ActiveCameraEntity: 2
Entities:
  - Entity: 1234567890
    TagComponent:
      Tag: Secondary Camera Entity
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        ProjectionFOV: 45
        PespectiveNear: 0.00999999978
        PespectiveFar: 1000
        OrthographicSize: 15
        OrthographicNear: -1
        OrthographicFar: 1
      FixedAspectRatio: false
  - Entity: 1234567890
    TagComponent:
      Tag: Primary Camera Entity
    TransformComponent:
      Translation: [0, 0, 6.9000001]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        ProjectionFOV: 45
        PespectiveNear: 0.00999999978
        PespectiveFar: 1000
        OrthographicSize: 15
        OrthographicNear: -1
        OrthographicFar: 1
      FixedAspectRatio: false
  - Entity: 1234567890
    TagComponent:
      Tag: Colored Square 2
    TransformComponent:
      Translation: [-1.39999998, -1.39999998, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [0, 1, 1, 1]
  - Entity: 1234567890
    TagComponent:
      Tag: Colored Square
    TransformComponent:
      Translation: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 1, 0, 1]