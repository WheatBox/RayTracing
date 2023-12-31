#include "Model.h"
#include "Ray.h"
#include "Math.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../Third/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SH 80 // Size Half
#define X 0
#define Y -60
#define Z 120
#define LBN { X-SH, Y+SH, Z-SH } // Left Bottom Near
#define LBF { X-SH, Y+SH, Z+SH }
#define RBF { X+SH, Y+SH, Z+SH }
#define RBN { X+SH, Y+SH, Z-SH }
#define LTN { X-SH, Y-SH, Z-SH }
#define LTF { X-SH, Y-SH, Z+SH }
#define RTF { X+SH, Y-SH, Z+SH }
#define RTN { X+SH, Y-SH, Z-SH }

#define WALLSPEC 0.f
#define MODELSPEC 0.f

Model models[] = {

	// ---- Bottom ----
	{ .type = eMFT_Triangle, .data.triangle = { LBN, RBF, LBF, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	{ .type = eMFT_Triangle, .data.triangle = { LBN, RBN, RBF, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	// ---- Top ----
	{ .type = eMFT_Triangle, .data.triangle = { LTN, LTF, RTF, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	{ .type = eMFT_Triangle, .data.triangle = { LTN, RTF, RTN, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	// ---- Left ----
	{ .type = eMFT_Triangle, .data.triangle = { LBN, LTF, LTN, { .color = { 255, 0, 0 }, .specular = WALLSPEC } } },
	{ .type = eMFT_Triangle, .data.triangle = { LBN, LBF, LTF, { .color = { 255, 0, 0 }, .specular = WALLSPEC } } },
	// ---- Right ----
	{ .type = eMFT_Triangle, .data.triangle = { RBN, RTN, RTF, { .color = { 0, 255, 0 }, .specular = WALLSPEC } } },
	{ .type = eMFT_Triangle, .data.triangle = { RBN, RTF, RBF, { .color = { 0, 255, 0 }, .specular = WALLSPEC } } },
	// ---- Far ----
	{ .type = eMFT_Triangle, .data.triangle = { LBF, RTF, LTF, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	{ .type = eMFT_Triangle, .data.triangle = { LBF, RBF, RTF, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	// ---- Near ----
	//{ .type = eMFT_Triangle, .data.triangle = { LBN, LTN, RTN, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	//{ .type = eMFT_Triangle, .data.triangle = { LBN, RTN, RBN, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },

	{ .type = eMFT_Triangle, .data.triangle = { { X-SH*3, Y+SH*4, Z-SH }, { X, Y-SH*4, Z-SH }, { X+SH*3, Y+SH*4, Z-SH }, { .color = { 255, 255, 255 }, .specular = WALLSPEC } } },
	// ---- - ----

	{
		.type = eMFT_Sphere,
		.data.sphere = {
			//.center = { 30, -15, 100 },
			.center = { 0, Y-SH+10, Z },
			//.center = { 0, -60, Z },
			/*{ -20, -120, 140 },
			{ 10, -20, 100 }, 
			{ -50, -50, -50 },*/
			.radius = 10,
			.material = {
				.color = { 255, 255, 255 },
				.specular = 0.f,
				.emission = {
					.color = { 255, 255, 255 },
					.power = 96.f
				}
			}
		}
	},

//	{ .type = eMFT_Sphere, .data.sphere = { { 0, 0, 120 }, 20, { .color = { 255, 255, 255 }, .specular = MODELSPEC } } },

//	{ .type = eMFT_Triangle, .data.triangle = { { 40, 20, 140 }, { 40, -40, 160 }, { -20, -20, 140 }, { .color = { 255, 255, 255 } } } },

	{ .type = eMFT_Sphere, .data.sphere = { { -10, -15, 180 }, 20, { .color = { 255, 255, 40 }, .specular = MODELSPEC } } },
	{ .type = eMFT_Sphere, .data.sphere = { { 20, -40, 150 }, 20, { .color = { 240, 20, 180 }, .specular = 1.f } } },
	{ .type = eMFT_Sphere, .data.sphere = { { 0, 0, 120 }, 20, { .color = { 127, 127, 255 }, .specular = .5f } } },
	{ .type = eMFT_Sphere, .data.sphere = { { -20, 0, 80 }, 20, { .color = { 255, 255, 255 }, .specular = MODELSPEC } } },
};
const size_t modelCount = sizeof(models) / sizeof(* models);

int main(int argc, char ** argv) {

	int sample = 256;
	int reflectionTimes = 4;
	const char * szOutputFilename = "./out.png";

	if(argc <= 1) {
		printf("Usage:\n    -s <Sample number>\n    -r <Reflection times>\n    -o <Output filename>\n");
		printf("\nNow using default settings\n\n");
	} else
	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-h")) {
			printf("Usage:\n    -s <Sample number>\n    -r <Reflection times>\n    -o <Output filename>\n");
			return 0;
		} else
		if(!strcmp(argv[i], "-s")) {
			sample = atoi(argv[++i]);
		} else
		if(!strcmp(argv[i], "-r")) {
			reflectionTimes = atoi(argv[++i]);
		} else
		if(!strcmp(argv[i], "-o")) {
			szOutputFilename = argv[++i];
		} else {
			printf("Unknown command: %s\n", argv[i]);
		}
	}

	const int width = 800, height = 800, channel = 3;
	const size_t widthInByte = (size_t)width * channel;
	const size_t dataSize = (size_t)widthInByte * height;
	unsigned char * data = (uint8 *)malloc(sizeof(uint8) * dataSize);

	if(!data) {
		perror("Failed to malloc!");
		return -1;
	}

	Vec3 rayPos = { 0, Y, -140 };

	Vec3 front = { 0, 0, 1 };
	// Remove the first plane in front of the camera
	const Model * pFirstPlane = Ray_IntersectGetModel(NULL, NULL, rayPos, front, models, modelCount, NULL);

	printf("Start to render\n");
	printf("    Sample number: %d\n", sample);
	printf("    Reflection times: %d\n", reflectionTimes);
	printf("    Output filename: %s\n", szOutputFilename);
	clock_t startTime = clock();

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			Vec3 rayDir = {
				(float)x / width - 0.5f,
				(float)y / height - 0.5f,
				1.f
			};

			int r = 0, g = 0, b = 0;
			for(int i = 0; i < sample; i++) {
				float emissionPowerTemp = 0.f;
				Color colorTemp = Ray_Trace(& emissionPowerTemp, reflectionTimes, rayPos, rayDir, models, modelCount, /*NULL*/ pFirstPlane);
				r += (int)(colorTemp.r * emissionPowerTemp);
				g += (int)(colorTemp.g * emissionPowerTemp);
				b += (int)(colorTemp.b * emissionPowerTemp);
			}
			Color color = {
				(uint8)Clamp(r / sample, 0, 255),
				(uint8)Clamp(g / sample, 0, 255),
				(uint8)Clamp(b / sample, 0, 255)
			};

			size_t pixelOff = (size_t)y * widthInByte + (size_t)x * channel;
			data[pixelOff + 0] = color.r;
			data[pixelOff + 1] = color.g;
			data[pixelOff + 2] = color.b;
		}

		if(y % (height / 10) == 0) {
			printf("Rendering %.0f%%\n", (float)y / height * 100);
		}
	}
	printf("Render 100%% Over!\n");

	printf("Time used: %f second(s)\n", (float)(clock() - startTime) / CLOCKS_PER_SEC);

	printf("Now saving... (Filename: %s)\n", szOutputFilename);
	stbi_write_png(szOutputFilename, width, height, channel, data, 0);
	printf("Save over!\n");

#ifdef _WIN32
	char szCommand[1024] = { "start " };
	strcat(szCommand, szOutputFilename);
	printf(szCommand);
	printf("\n");
	system(szCommand);
#endif

	free(data);
	return 0;
}
