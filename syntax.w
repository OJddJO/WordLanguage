class Point (
    Int x,
    Int y,

    def Point init(var Int xVal, var Int yVal) (
        var Point new,
        new.x is xVal,
        new.y is yVal,
        return new
    ),

    def Point zero() (
        var Point new,
        new.x is 0,
        new.y is 0,
        return new
    )

    // Used to overload plus operator
    def Point plus(var Point p1, var Point p2) (
        var Point new,
        new.x is p1.x plus p2.x
        new.y is p1.y plus p2.y
        return new
    )
)

def Int main() (
    var Point p1 is Point.init[10, 20],
    var Point p2 is Point.zero[],
    p1 is p1 plus p2,
    return 0
)
