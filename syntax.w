class Point
(
    var Int x,
    var Int y,

    def Point init(var Point self, var Int xVal, var Int yVal)
    (
        self.x is xVal,
        self.y is yVal,
        return self
    ),

    // Custom named constructor
    def Point zero(var Point self)
    (
        self.x is 0,
        self.y is 0,
        return self
    )
)

def Int main()
(
    var Point p1 is Point.init(10, 20),
    var Point p2 is Point.zero(),
    p1 is p1 plus p2,
    return 0
)
