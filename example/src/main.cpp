#include "window.hpp"

#include "terrain.hpp"

/// Entry point of program

/// Creates and starts an instance of Window
int main(int argc, char *argv[]) {
	Window window;
	window.start();
	return 0;
}

/* Clouds */

//int seed = time(NULL);

// module::Perlin myModule;
// myModule.SetSeed(seed);
// utils::NoiseMap noise_map;
// utils::NoiseMapBuilderPlane noise_map_builder;
// noise_map_builder.SetSourceModule(myModule);
// noise_map_builder.SetDestNoiseMap(noise_map);
// noise_map_builder.SetDestSize(256, 256);
// noise_map_builder.SetBounds(2.0, 6.0, 1.0, 5.0);
// noise_map_builder.Build();

// utils::RendererImage renderer;
// utils::Image image;
// renderer.SetSourceNoiseMap (noise_map);
// renderer.SetDestImage (image);
// renderer.Render ();

// utils::WriterBMP writer;
// writer.SetSourceImage(image);
// writer.SetDestFilename ("tutorial.bmp");
// writer.WriteDestFile();