#ifndef shapes_triangle_hpp
#define shapes_triangle_hpp

#include "../shape.hpp"

class Triangle : public Shape {
    Vec3 v0, v1, v2, vn0, vn1, vn2;

public:
    Triangle(
        const BxDF *bxdf, const Light *light,
        const Medium *inside, const Medium *outside,
        Vec3 v0, Vec3 v1, Vec3 v2
    );
    Triangle(
        const BxDF *bxdf, const Light *light,
        const Medium *inside, const Medium *outside,
        Vec3 v0, Vec3 v1, Vec3 v2,
        Vec3 vn0, Vec3 vn1, Vec3 vn2
    );

    bool intersect(const Ray &ray, double &t, Vec3 &pos, Vec2 &texPos) const override;
    Vec3 normal(const Vec2 &texPos) const override;
};

#ifdef ARC_IMPLEMENTATION

Triangle::Triangle(
    const BxDF *bxdf, const Light *light,
    const Medium *inside, const Medium *outside,
    Vec3 v0, Vec3 v1, Vec3 v2
) : Shape(bxdf, light, inside, outside), v0(v0), v1(v1), v2(v2) {
    vn0 = vn1 = vn2 = ((v1 - v0) ^ (v2 - v0)).norm();
    box = Box3(v0) + Box3(v1) + Box3(v2);
}

Triangle::Triangle(
    const BxDF *bxdf, const Light *light,
    const Medium *inside, const Medium *outside,
    Vec3 v0, Vec3 v1, Vec3 v2,
    Vec3 vn0, Vec3 vn1, Vec3 vn2
) : Shape(bxdf, light, inside, outside), v0(v0), v1(v1), v2(v2), vn0(vn0), vn1(vn1), vn2(vn2) {
    box = Box3(v0) + Box3(v1) + Box3(v2);
}


bool Triangle::intersect(const Ray &ray, double &t, Vec3 &pos, Vec2 &texPos) const {
    Vec3 e1 = v1 - v0, e2 = v2 - v0, T = ray.o - v0;
    Vec3 p = (ray.d ^ e2);
    double det = p * e1;
    if (det < 0) det = -det, T = -T;
    if (det < EPS) return false;
    double u = p * T;
    if (u < 0 || u > det) return false;
    Vec3 q = (T ^ e1);
    t = q * e2 / det;
    if (t < EPS) return false;
    double v = q * ray.d;
    if (v < 0 || u + v > det) return false;
    pos = ray.o + ray.d * t;
    texPos = {u / det, v / det};


    return true;
}

Vec3 Triangle::normal(const Vec2 &texPos) const {
    return (vn0 * (1 - texPos.x() - texPos.y()) + vn1 * texPos.x() + vn2 * texPos.y()).norm();
}

#endif
#endif /* shapes_triangle_hpp */
