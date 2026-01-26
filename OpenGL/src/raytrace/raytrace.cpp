#include <iostream>
#include "vec3.h"

int main()
{
    //Image

    int image_width = 256;
    int image_height = 256;
    vec3 point1(1.0,1.0,1.0);
    vec3 point2(3.0,1.0,1.0);
    vec3 point3(2.0,3.0,1.0);


    //Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int (255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::clog << "\rDone.           \n";

}