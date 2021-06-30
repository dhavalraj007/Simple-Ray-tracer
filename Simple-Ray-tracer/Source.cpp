#include<iostream>
#include"stb_image.h"
#include"stb_image_write.h"
#include"glm/vec3.hpp"
#include"helper.h"

// Constants
constexpr auto SCR_WIDTH = 800;
constexpr auto SCR_HEIGHT = 600;
constexpr auto SCR_NC = 3;


void render()
{
	unsigned char* image = new unsigned char[SCR_WIDTH * SCR_HEIGHT * SCR_NC];

	unsigned int index = 0;
	for (int y = SCR_HEIGHT-1; y >=0; y--)
	{
		std::cerr << "\rScan lines Remaining : " << y << "   " << std::flush;
		for (int x = 0; x < SCR_WIDTH; x++)
		{
			float r = (float)x / SCR_WIDTH;
			float g = (float)y / SCR_HEIGHT;
			float b = 0.2f;
			writeColor(image, index, color(r, g, b));
		}
	}
	std::cerr << "\nDone.\n" << std::flush;
	stbi_write_png("./shit.png", SCR_WIDTH, SCR_HEIGHT, SCR_NC, image,
		SCR_NC *SCR_WIDTH );

}



int main()
{
	render();
	return 0;
}