from ..type_hints import MatrixInt32, VectorFloat64

class Index:

    def __init__(self, lon: VectorFloat64, lat: VectorFloat64,
                 triangles: MatrixInt32) -> None:
        ...

    def lat(self) -> VectorFloat64:
        ...

    def lon(self) -> VectorFloat64:
        ...

    def triangles(self) -> MatrixInt32:
        ...
