#include "arc.hpp"
#include <bits/stdc++.h>
using namespace std;

int main()
{
    ios::sync_with_stdio(0);

    shared_ptr<BxDF> bxdf = make_shared<BSDF>();

    shared_ptr<Material> Mair = make_shared<Material>(
        0, Spectrum(0), 1, 0, Spectrum(1), Spectrum(1), Spectrum(1), "air"
    );

    shared_ptr<Material> Mlight0 = make_shared<Material>(
        1, Spectrum(0), 1, 0, Spectrum(0), Spectrum(0), Spectrum(0), "light0"
    );

    shared_ptr<Material> Mlight1 = make_shared<Material>(
        1, Spectrum(1), 1, 0, Spectrum(0), Spectrum(0), Spectrum(0), "light1"
    );

    shared_ptr<Material> Mlight2 = make_shared<Material>(
        1, Spectrum(4), 1, 0, Spectrum(0), Spectrum(0), Spectrum(0), "light2"
    );

    shared_ptr<Material> Mlight6 = make_shared<Material>(
        1, Spectrum(36), 1, 0, Spectrum(0), Spectrum(0), Spectrum(0), "light6"
    );

    shared_ptr<Material> Mdiffuse = make_shared<Material>(
        0, Spectrum(0), 1.5, 1, rgb888(255), rgb888(255), Spectrum(1), "diffuse"
    );

    shared_ptr<Object> skybox = make_shared<Object>(
        bxdf,
        make_shared<Sphere>(Vec3(0, 0, 0), INF / 10),
        make_shared<Solid>(Mair, Mlight0),
        "skybox"
    );

    shared_ptr<Sence> sence = make_shared<Sence>(
        Mair, skybox
    );

    shared_ptr<Mapping> texture = make_shared<Mapping>("texture.png");

    Trans3 Tback(
        Vec3(-30, -30, -25), Vec3(-30, 30, -25), Vec3(30, 30, -25),
        Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(1, 1, 0)
    );

    auto Surface = make_shared<Solid>(
        Mdiffuse, Mair, texture, Tback, 0.5
    );

    shared_ptr<Object> back = make_shared<Object>(
        bxdf,
        make_shared<Flat>(
            Vec3(-30, -30, -25),
            Vec3(-30, 30, -25),
            Vec3(30, 30, -25),
            Vec3(30, -30, -25)
        ),
        Surface,
        "back"
    );
    sence->add_object(back);

    char output[100];
    sprintf(output, "result.ppm");

    shared_ptr<Object> light = make_shared<Object>(
        bxdf,
        make_shared<Disc>(Vec3(-35.36, 0, 35.36), Vec3(0.707, 0, -0.707), 20),
        make_shared<Solid>(Mlight6, Mair),
        "light"
    );
    sence->add_object(light);

    shared_ptr<Camera> camera = make_shared<PerspectiveCamera>(
        Vec3(0, 0, 50),
        Vec3(1, 0, 0),
        Vec3(0, 1, 0),
        1
    );

    shared_ptr<oBuffer> image = make_shared<oBuffer>(400, 400, camera);
    shared_ptr<Render> render = make_shared<Render>(image, sence);

    int epoch = 10000, cluster = 1;
    cerr << "target : " << epoch << endl;
    for (int i = 1; i <= epoch; ++i)
    {
        render->epoch(cluster);
        cerr << "epoch " << i << endl;
        if (i % 5 == 0)
            image->save(output, 1);
    }
    image->save(output, 1);
}
