class Point (var Int x, var Int y) (
    func Point init(var Int xVal, var Int yVal) (
        var Point new,
        new.x is xVal,
        new.y is yVal,
        return new,
    ),

    func Point zero() (
        var Point new,
        new.x is 0,
        new.y is 0,
        return new,
    ),

    func Point plus(var Point p1, var Point p2) (
        var Point new,
        new.x is p1.x plus p2.x,
        new.y is p1.y plus p2.y,
        return new,
    ),

    func Int distance(var Point p1, var Point p2) (
        return ((p1.x minus p2.x).pow[2] plus (p1.y minus p2.y).pow[2]).sqrt[],
    )
)

func Int main() (
    var Point p1 is Point.init[2, -3],
    var Point p2 is Point.init[1, -5],
    var Point zero is Point.zero[],

    var Point closerToZero,
    if (zero.distance[p1] le zero.distance[p2]) (
        closerToZero is p1
    ) else (
        closerToZero is p2
    ),

    while (1) (
        break
    ),

    return 0
)
