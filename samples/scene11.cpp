#define ARC_IMPLEMENTATION
#include "arc.hpp"
#include <bits/stdc++.h>
using namespace std;

//#define DEBUG_FLAG
int main() {
    ios::sync_with_stdio(false);


    long long T0 = time(nullptr);


    auto medium = new Transparent(Spectrum(1, 1, 1));
//    auto medium = new Scatter(0.3, Spectrum(1, 1, 1));

    auto camera = new PerspectiveCamera(
        Vec3(0, 0, 1.5),
        Vec3(0.8, 0, 0),
        Vec3(0, 0.6, 0),
        0.6
    );
    auto scene = new Scene(camera, Spectrum(0), medium);

    auto curve = new Spline(3,
        Vec2(0, 0), Vec2(0, 0),
        Vec2(.3, 0), Vec2(.5, .2),
        Vec2(.5, .4), Vec2(.3, .6),
        Vec2(.3, .8), Vec2(.3, 1)
    );

//    auto curve = new Spline(1, Vec2(.5, 0), Vec2(.5, 1));
    scene->addObject(
        new Revolved(
            new Lambert(rgb256(170, 250, 170)), nullptr,
            medium, medium,
            Ray(Vec3(0, -1, -1), Vec3(0, 1, 0)), curve
        )
    );

//    scene->addObject(
//        new Cylinder(
//            new Lambert(rgb256(170, 250, 170)), nullptr,
//            medium, medium,
//            Ray(Vec3(0, -1, -1), Vec3(0, 1, 0)), .5, 0, 1
//        )
//    );

    scene->addObject(
        new Flat(
            new Lambert(Spectrum(0)),
            new UniformLight(Spectrum(10)),
            medium, medium,
            Vec3(-0.3, 0.99, -0.7),
            Vec3(-0.3, 0.99, -1.3),
            Vec3(0.3, 0.99, -1.3),
            Vec3(0.3, 0.99, -0.7)
        ), "light"
    );

    scene->addObject(
        new Flat(
            new GGX(rgb256(250, 250, 250), 0.8), nullptr,
            medium, medium,
            Vec3(-1, -1, -2),
            Vec3(-1, -1, 2),
            Vec3(1, -1, 2),
            Vec3(1, -1, -2)
        ), "down"
    );

    scene->addObject(
        new Flat(
            new GGX(rgb256(250, 250, 250), 0.8), nullptr,
            medium, medium,
            Vec3(-1, 1, 2),
            Vec3(-1, 1, -2),
            Vec3(1, 1, -2),
            Vec3(1, 1, 2)
        ), "up"
    );

    scene->addObject(
        new Flat(
            new GGX(rgb256(170, 170, 250), 0.8), nullptr,
            medium, medium,
            Vec3(1, -1, -2),
            Vec3(1, -1, 2),
            Vec3(1, 1, 2),
            Vec3(1, 1, -2)
        ), "right"
    );

    scene->addObject(
        new Flat(
            new GGX(rgb256(250, 170, 170), 0.8), nullptr,
            medium, medium,
            Vec3(-1, -1, 2),
            Vec3(-1, -1, -2),
            Vec3(-1, 1, -2),
            Vec3(-1, 1, 2)
        ), "left"
    );

    scene->addObject(
        new Flat(
            new Lambert(rgb256(250, 250, 250)), nullptr,
            medium, medium,
            Vec3(-1, 1, -2),
            Vec3(-1, -1, -2),
            Vec3(1, -1, -2),
            Vec3(1, 1, -2)
        ), "back"
    );

    scene->addObject(
        new Flat(
            new Lambert(rgb256(250, 250, 250)), nullptr,
            medium, medium,
            Vec3(-1, -1, 2),
            Vec3(-1, 1, 2),
            Vec3(1, 1, 2),
            Vec3(1, -1, 2)
        ),
        "front"
    );

    auto tracer = new BidirectionalPathTracer(1600, 1200, scene);

    char output[100];
    sprintf(output, "results/scene11.png");

    int epoch = 1000;

    cerr << "[T + " << time(nullptr) - T0 << "] | target : " << epoch << endl;
    for (int i = 1; i <= epoch; ++i) {
        tracer->epoch();
        cerr << "[T + " << time(nullptr) - T0 << "] | epoch " << i << endl;
        tracer->savePNG(output, 1);
    }
}
