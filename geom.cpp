#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>


const double DIFF = 0.0001;
const double PI = M_PI;

double get_angle_from_atan(double x) {
    return (180 / PI) * atan(x);
}

struct Point {
    double x, y;
    Point(double x, double y) {
        this -> x = x;
        this -> y = y;
    }
    Point() {}
    Point & operator += (const Point & P2) {
        this -> x += P2.x;
        this -> y += P2.y;
        return *this;
    }
    Point & operator -= (const Point & P2) {
        this -> x -= P2.x;
        this -> y -= P2.y;
        return *this;
    }
    Point & operator *= (double alpha) {
        this -> x *= alpha;
        this -> y *= alpha;
        return *this;
    }
    Point & operator /= (double alpha) {
        this -> x /= alpha;
        this -> y /= alpha;
        return *this;
    }
    Point operator + (const Point & P2) const {
        Point temp = * this;
        temp += P2;
        return temp;
    }
    Point operator - (const Point & P2) const {
        Point temp = * this;
        temp -= P2;
        return temp;
    }
    Point operator * (double alpha) const {
        Point temp = * this;
        temp *= alpha;
        return temp;
    }
    Point operator / (double alpha) const {
        Point temp = * this;
        temp /= alpha;
        return temp;
    }
    Point operator - () const {
        Point temp = * this;
        temp *= -1;
        return temp;
    }
    bool operator < (const Point & b) const {
        if (std::abs(( * this).x - b.x) < DIFF)
            return (b.y - ( * this).y) > DIFF;
        return (b.x - ( * this).x) > DIFF;
    }

    bool operator > (const Point & b) const {
        return b < ( * this);
    }

    bool operator <= (const Point & b) const {
        return !(b < ( * this));
    }

    bool operator >= (const Point & b) const {
        return !(( * this) < b);
    }

    bool operator == (const Point & b) const {
        return (std::abs(x - b.x) < DIFF) && (std::abs(y - b.y) < DIFF);
    }

    bool operator != (const Point & b) const {
        return !(( * this) == b);
    }
    void rotate_point(double angle) {
        angle = (angle / 180) * PI;
        double sin = std::sin(angle);
        double cos = std::cos(angle);
        Point copy = * this;
        this -> x = copy.x * cos - copy.y * sin;
        this -> y = copy.x * sin + copy.y * cos;
    }
    double cos_of_angle(const Point & p) const {
        return (x * p.x + y * p.y) / (sqrt(x * x + y * y) * sqrt(p.x * p.x + p.y * p.y));
    }
    double abs() {
        return sqrt(x * x + y * y);
    }
};

class Line {
    public:
        double a, b, c;
    public:
        Line() {}
    Line(const Point & point1,
        const Point & point2) {
        if (point1.x == point2.x) {
            this -> b = 0;
            this -> c = -point1.x;
            this -> a = 1;
        } else {
            this -> b = -1;
            double slope = (point2.y - point1.y) / (point2.x - point1.x);
            this -> c = point1.y - slope * point1.x;
            this -> a = slope;
        }
    }
    Line(double slope, double shift) {
        this -> a = slope;
        this -> b = -1;
        this -> c = shift;
    }
    Line(const Point & point, double slope) {
        this -> a = slope;
        this -> b = -1;
        this -> c = point.y - this -> a * point.x;
    }
    bool operator == (const Line & line2) const {
        return std::abs(a - line2.a) < DIFF && std::abs(b - line2.b) < DIFF && std::abs(c - line2.c) < DIFF;
    }

    bool operator != (const Line & line2) const {
        return !( * this == line2);
    }
    void reflect_point_over_line(Point & point) const {
        double r = -2 * (a * point.x + b * point.y + c) / (a * a + b * b);
        point.x += a * r;
        point.y += b * r;
    }
};

class Shape {
    protected:
        virtual void scale(double coeficent) = 0;
    virtual void adjust(const Point & delta) = 0;
    virtual void rotate(double angle) = 0;
    public:
        virtual void rotate(const Point & center, double angle) {
            adjust(-center);
            rotate(angle);
            adjust(center);
        }
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool isCongruentTo(const Shape & other) const = 0;
    virtual bool containsPoint(const Point & point) const = 0;
    virtual bool operator == (const Shape & other) const = 0;
    virtual void scale(const Point & center, double coeficent) {
        adjust(-center);
        scale(coeficent);
        adjust(center);
    }
    virtual bool isSimilarTo(const Shape & other) const = 0;

    virtual void reflex(const Point & center) = 0;
    virtual void reflex(const Line & axis) = 0;
    bool operator != (const Shape & other) const {
        return !( * this == other);
    }
    virtual~Shape() =
        default;

};

class Polygon: public Shape {
    protected: std::vector < Point > peaks;
    public: template < class T,
    class...T2 >
    Polygon(T var1, T2...var2) {
        addargs(var1, var2...);
    }

    template < class T,
    class...T2 >
    void addargs(T var1, T2...var2) {
        peaks.push_back(var1);
        addargs(var2...);
    }
    template < class T >
    void addargs(T n) {
        peaks.push_back(n);
    }

    Polygon(const std::initializer_list < Point > & args): peaks(args) {}

    void reflex(const Point & center) override {
        for (auto & apex: peaks) {
            apex = center * 2 - apex;
        }
    }
    size_t verticesCount() const {
        return peaks.size();
    }

    void rotate(const Point & center, double angle) override {
        Shape::rotate(center, angle);
    }

    const std::vector < Point > & getVertices() const {
        return peaks;
    }

    void reflex(const Line & axis) override {
        for (auto & apex: peaks) {
            axis.reflect_point_over_line(apex);
        }
    }

    double area() const override {
        double area = 0;
        for (size_t i = 0; i < verticesCount(); ++i) {
            size_t next = i;
            rotate_around_figure(next, false);
            area += (peaks[i].x * peaks[next].y - peaks[next].x * peaks[i].y) / 2;
        }
        return std::abs(area);
    }
    double perimeter() const override {
        double perimeter = 0;
        for (size_t i = 0; i < verticesCount(); ++i) {
            size_t next = i;
            rotate_around_figure(next, false);
            perimeter += sqrt(pow(peaks[i].x - peaks[next].x, 2) + pow(peaks[i].y - peaks[next].y, 2));
        }
        return perimeter;
    }
    explicit Polygon(std::vector < Point > prototypes): peaks(std::move(prototypes)) {}

    void scale(const Point & center, double coeficent) override {
        Shape::scale(center, coeficent);
    }

    void rotate(double angle) override {
        for (auto & apex: peaks) {
            apex.rotate_point(angle);
        }
    }

    void rotate_around_figure(size_t& i, bool back) const
    {
        if(back){
            if (i == 0) i = peaks.size();
            --i;
        }else{
            ++i;
            if (i == peaks.size()) i = 0;
        }
    }

    void scale(double coeficent) override {
        for (auto & apex: peaks) {
            apex *= coeficent;
        }
    }

    bool isCongruentTo(const Shape & pol2) const override {
        std::cerr << "cong1" << std::endl;
        const auto * poly_2 = dynamic_cast <
            const Polygon * > ( & pol2);
        if (poly_2 == nullptr) return false;
        return isSimilarTo( * poly_2) && std::abs(perimeter() - poly_2 -> perimeter()) < DIFF;
    }
    void adjust(const Point & delta) override {
        for (auto & apex: peaks) {
            apex += delta;
        }
    }

    bool isConvex() const {
        bool neg = 0, pos = 0;
        size_t iterator;
        for (size_t i = 0; i < verticesCount(); ++i) {
            iterator = i;
            Point p1 = peaks[iterator];
            rotate_around_figure(iterator, false);
            Point p2 = peaks[iterator];
            rotate_around_figure(iterator, false);
            Point p3 = peaks[iterator];
            double area = (p1 - p2).x * (p3 - p2).y - (p1 - p2).y * (p3 - p2).x;
            (area < 0 ? neg : pos) = true;
            if (neg && pos)
                return false;
        }

        return true;
    }
    bool operator == (const Shape & other) const override {
        const auto * otherCast = dynamic_cast <
            const Polygon * > ( & other);
        if (otherCast == nullptr) return false;
        if (verticesCount() != otherCast -> verticesCount())
            return false;
        size_t start, dif;
        start = dif = find(otherCast -> peaks.begin(), otherCast -> peaks.end(), peaks[0]) - otherCast -> peaks.begin();
        if (start == otherCast -> verticesCount()) return false;
        otherCast -> rotate_around_figure(start, false);
        if (otherCast -> peaks[start] == peaks[1]) {
            for (size_t i = 0; i < verticesCount(); ++i) {
                if (otherCast -> peaks[dif] != peaks[i]) return false;
                otherCast -> rotate_around_figure(dif, false);
            }
        } else {
            for (size_t i = 0; i < verticesCount(); ++i) {
                if (otherCast -> peaks[dif] != peaks[i]) return false;
                otherCast -> rotate_around_figure(dif, true);
            }
        }
        return true;
    }
    bool isSimilarTo(const Shape & other) const override {
        const auto * figur2 = dynamic_cast <
            const Polygon * > ( & other);
        if (figur2 == nullptr) return false;

        if (verticesCount() != figur2 -> verticesCount()) {
            return false;
        }

        double k = (perimeter() / figur2 -> perimeter());

        bool result_straight = false;
        bool result_cross = false;

        for (size_t indx = 0; indx < verticesCount(); indx++) {

            result_straight = true;
            for (size_t i = 0; i < verticesCount(); i++) {

                size_t j = i;
                Point current = peaks[j];
                rotate_around_figure(j, false);
                Point next = peaks[j];
                size_t m = (i + indx) % verticesCount();
                Point figure2_current = figur2 -> peaks[m];
                figur2 -> rotate_around_figure(m, false);
                Point figure2_next = figur2 -> peaks[m];

                if (std::abs((current - next).abs() / (figure2_current - figure2_next).abs() - k) > DIFF) {
                    result_straight = false;
                    break;
                }
            }
            if(result_straight) return true;
        }
        for (size_t indx = 0; indx < verticesCount(); indx++) {
            result_cross = true;
            for (size_t i = 0, j = 0; i < verticesCount(); i++) {

                Point current = peaks[j];
                rotate_around_figure(j, true);
                Point next = peaks[j];
                size_t m = (i + indx) % verticesCount();
                Point figure2_current = figur2 -> peaks[m];
                figur2 -> rotate_around_figure(m, false);
                Point figure2_next = figur2 -> peaks[m];
                if (std::abs((current - next).abs() / (figure2_current - figure2_next).abs() - k) > DIFF) {
                    result_cross = false;
                    break;
                }
            }

            if ( result_cross) return true;
        }
        return false;
    }

    bool containsPoint(const Point & point) const override {

        size_t i, j;
        bool c = 0;
        for (i = 0, j = verticesCount() - 1; i < verticesCount(); j = i++) {
            if ((((peaks[i].y <= point.y) && (point.y < peaks[j].y)) ||
                    ((peaks[j].y <= point.y) && (point.y < peaks[i].y))) &&
                (point.x < (peaks[j].x - peaks[i].x) * (point.y - peaks[i].y) / (peaks[j].y - peaks[i].y) + peaks[i].x))
                c = !c;
        }

        return c;
    }

};

class Ellipse: public Shape {
    public: Point focus1;
    Point focus2;
    double a_axis;
    public: Ellipse(const Point & focus1,
        const Point & focus2, double axis) {

        this -> focus1 = focus1;
        this -> focus2 = focus2;
        this -> a_axis = axis / 2;
    }
    std::pair < Point,
    Point > focuses() const {
        return std::make_pair(focus1, focus2);
    }

    double find_c() const {
        return sqrt((focus1.x - focus2.x) * (focus1.x - focus2.x) + (focus1.y - focus2.y) * (focus1.y - focus2.y)) / 2;
    }
    double find_b() const {
        return sqrt(a_axis * a_axis - find_c() * find_c());
    }
    double eccentricity() const {
        return find_c() / a_axis;
    }

    Point center() const {
        return (focus1 + focus2) / 2;
    }

    double area() const override {

        double area = PI * a_axis * find_b();
        return area;
    }
    double perimeter() const override {

        double h = pow(a_axis - find_b(), 2) / pow(a_axis + find_b(), 2);
        return PI * (a_axis + find_b()) * (1 + 3 * h / (10 + sqrt(4 - 3 * h)));
    }

    std::pair < Line,
    Line > directrices() const {
        double exc = eccentricity();
        double x = a_axis / exc;
        double angle = get_angle_from_atan((focus2.y - focus1.y) / (focus2.x - focus1.x));

        Point point01(x, 0), point20(-x, 0);
        Point point11 = point01 + Point(0, 1);
        Point point21 = -point11;

        point01.rotate_point(angle);
        point20.rotate_point(angle);
        point11.rotate_point(angle);
        point21.rotate_point(angle);

        return std::make_pair(Line(point01 + center(), point11 + center()), Line(point20 + center(), point21 + center()));
    }

    void scale(double coeficent) override {

        focus1 *= coeficent;
        focus2 *= coeficent;
        a_axis *= coeficent;
    }

    void rotate(double angle) override {
        focus1.rotate_point(angle);
        focus2.rotate_point(angle);
    }

    void scale(const Point & center, double coeficent) override {

        Shape::scale(center, coeficent);
    }

    bool operator == (const Shape & elipse2) const override {

        const auto * elipse_2 = dynamic_cast <
            const Ellipse * > ( & elipse2);
        if (elipse_2 == nullptr) return false;
        return std::min(focus1, focus2) == std::min(elipse_2 -> focus1, elipse_2 -> focus2) &&
            std::max(focus1, focus2) == std::max(elipse_2 -> focus1, elipse_2 -> focus2) && std::abs(a_axis - elipse_2 -> a_axis) < DIFF;
    }

    void rotate(const Point & center, double angle) override {

        Shape::rotate(center, angle);
    }

    void reflex(const Point & center) override {

        focus1 = center * 2 - focus1;
        focus2 = center * 2 - focus2;
    }

    bool isCongruentTo(const Shape & elipse2) const override {

        const auto * elipse_2 = dynamic_cast <
            const Ellipse * > ( & elipse2);
        if (elipse_2 == nullptr) return false;
        return std::abs(a_axis - elipse_2 -> a_axis) < DIFF && std::abs(find_b() - elipse_2 -> find_b()) < DIFF;
    }
    void adjust(const Point & delta) override {
        focus1 += delta;
        focus2 += delta;
    }

    void reflex(const Line & axis) override {

        axis.reflect_point_over_line(focus1);
        axis.reflect_point_over_line(focus2);

    }

    bool isSimilarTo(const Shape & elipse2) const override {

        const auto * elipse_2 = dynamic_cast <
            const Ellipse * > ( & elipse2);
        if (elipse_2 == nullptr) return false;
        return std::abs(eccentricity() - elipse_2 -> eccentricity()) < DIFF;
    }

    bool containsPoint(const Point & point) const override {

        double dist_1 = sqrt(pow(focus1.x - point.x, 2) + pow(focus1.y - point.y, 2));
        double dist_2 = sqrt(pow(focus2.x - point.x, 2) + pow(focus2.y - point.y, 2));
        return dist_1 + dist_2 < 2 * a_axis || std::abs(dist_1 + dist_2 - 2 * a_axis) < DIFF;
    }

};

class Circle: public Ellipse {
    public: Circle(const Point & center, double rad): Ellipse(center, center, rad * 2) {}

    double radius() const {
        return a_axis;
    }

    double perimeter() const {
        return 2 * PI * a_axis;
    }
    double area() const {
        return PI * a_axis * a_axis;
    }

};

class Rectangle: public Polygon {
    public: Rectangle(const Point & p1,
        const Point & p2, double r): Polygon(Point(0, 0), Point(0, 0), Point(0, 0), Point(0, 0)) {
        peaks[0] = p1;
        peaks[2] = p2;
        double angle = get_angle_from_atan(r);

        double d = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
        double a = sqrt((d * d) / (r * r + 1));
        double b = r * a;

        peaks[3] = (peaks[2] - peaks[0]) * (b / d);
        peaks[3].rotate_point(angle - 90);
        peaks[1] = (peaks[2] - peaks[0]) * (a / d);
        peaks[1].rotate_point(angle);

        peaks[3] += peaks[0];
        peaks[1] += peaks[0];

    }
    std::pair < Line,
    Line > diagonals() const {
        Line d1(peaks[1], peaks[3]);
        Line d2(peaks[0], peaks[2]);
        return std::make_pair(d1, d2);
    }

    Point center() const {
        return (peaks[0] + peaks[2]) / 2;
    }
};

class Square: public Rectangle {
    public:

        Circle circumscribedCircle() const {
            return Circle(center(), sqrt((peaks[0].x - peaks[2].x) * (peaks[0].x - peaks[2].x) + (peaks[0].y - peaks[2].y) * (peaks[0].y - peaks[2].y)) / 2);
        }

    Circle inscribedCircle() const {
        return Circle(center(), sqrt((peaks[0].x - peaks[1].x) * (peaks[0].x - peaks[1].x) + (peaks[0].y - peaks[1].y) * (peaks[0].y - peaks[1].y)));
    }
    Square(const Point & point1,
        const Point & point2): Rectangle(point1, point2, 1) {}
};

class Triangle: public Polygon {
    public:

        Point circum_circle_center() const {
            Point A, B, C;
            std::vector < Point > f = {
                peaks[0],
                peaks[1],
                peaks[2]
            };
            sort(f.begin(), f.end());
            A = f[0];
            B = f[1];
            C = f[2];

            double AC = sqrt((A.x - C.x) * (A.x - C.x) + (A.y - C.y) * (A.y - C.y));
            double BC = sqrt((B.x - C.x) * (B.x - C.x) + (B.y - C.y) * (B.y - C.y));
            double AB = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
            double area = Polygon::area();
            double R = AC * BC * AB / (4 * area);

            double angle = acos((AB) / (2 * R)) * 180 / PI;
            Point F = C = B - A;
            F = F / F.abs();

            F.rotate_point(angle);

            F *= (R);

            F += A;

            return F;

        }

    Point inscribe_circle_center() const {
        Point A = peaks[0], B = peaks[1], C = peaks[2];
        double AC = sqrt((A.x - C.x) * (A.x - C.x) + (A.y - C.y) * (A.y - C.y));
        double BC = sqrt((B.x - C.x) * (B.x - C.x) + (B.y - C.y) * (B.y - C.y));
        double AB = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));

        return Point(A.x * BC + B.x * AC + C.x * AB, A.y * BC + B.y * AC + C.y * AB) / (AB + BC + AC);
    }

    public: Triangle(const Point & p1,
        const Point & p2,
            const Point & p3): Polygon(p1, p2, p3) {}

    Circle circumscribedCircle() const {
        Point o = circum_circle_center();
        double r = sqrt((o.x - peaks[0].x) * (o.x - peaks[0].x) + (o.y - peaks[0].y) * (o.y - peaks[0].y));
        return Circle(o, r);
    }

    Circle inscribedCircle() const {
        return Circle(inscribe_circle_center(), 2 * (area() / perimeter()));
    }

    Circle ninePointsCircle() const {
        Point center = (orthocenter() + circum_circle_center()) / 2;
        Point k = (peaks[0] + peaks[1]) / 2;
        double r = sqrt((center.x - k.x) * (center.x - k.x) + (center.y - k.y) * (center.y - k.y));
        return Circle(center, r);
    }

    Line EulerLine() const {
        Point orthcenter = orthocenter(), circum_center = circum_circle_center(), centoid_center = centroid();
        if (orthcenter == centoid_center && centoid_center == circum_center)
            return Line(orthcenter, 1);
        else if (orthcenter != circum_center)
            return Line(orthcenter, circum_center);
        else
            return Line(circum_center, centoid_center);
    }

    Point orthocenter() const {
        Point ortho;
        Point A = peaks[0], B = peaks[1], C = peaks[2];

        double alpha = tan(acos((B - A).cos_of_angle(C - A)));
        double beta = tan(acos((C - B).cos_of_angle(A - B)));
        double gamma = tan(acos((A - C).cos_of_angle(B - C)));
        double summ = alpha + beta + gamma;

        return Point((A.x * alpha + B.x * beta + C.x * gamma), (A.y * alpha + B.y * beta + C.y * gamma)) / summ;
    }

    Point centroid() const {
        Point o;
        o.x = (peaks[0].x + peaks[1].x + peaks[2].x);
        o.y = (peaks[0].y + peaks[1].y + peaks[2].y);
        return o / 3;
    }
};

#endif