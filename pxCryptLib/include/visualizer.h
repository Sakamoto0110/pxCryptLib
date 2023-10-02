#pragma once


#include <stdio.h>
#include <cstdint>


#include <vector>


namespace Visualizer {
	uint32_t RGBA(uint8_t red, uint8_t green, uint8_t blue) {
		return ((red & 0x0ff) << 24) | ((green & 0x0ff) << 16) | ((blue & 0x0ff) << 8) | ((0 & 0x0ff) << 0);
	}
	uint32_t RGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
		return ((red & 0x0ff) << 24) | ((green & 0x0ff) << 16) | ((blue & 0x0ff) << 8) | ((alpha & 0x0ff) << 0);
	}
	



	// Function to interpolate between two colors
// r1, g1, b1: Color 1 (e.g., red)
// r2, g2, b2: Color 2 (e.g., blue)
// t: Interpolation parameter (0.0 to 1.0)
// Returns the interpolated color
	int interpolateColor(int r1, int g1, int b1, int r2, int g2, int b2, double t) {
		int r = static_cast<int>((1.0 - t) * r1 + t * r2);
		int g = static_cast<int>((1.0 - t) * g1 + t * g2);
		int b = static_cast<int>((1.0 - t) * b1 + t * b2);
		return (r << 16) | (g << 8) | b;
	}

	struct MAP_DATA {
		uint8_t colors[6];
		double brightness[2];
		uint32_t extra;
	};
	static constexpr MAP_DATA DEFAULT_GRAD_2D = { {225,0,0,0,0,225},{.8,.3} };

	// Function provided by chat gpt 3.5
	uint32_t* ColorMap(int width, int height, MAP_DATA dat = DEFAULT_GRAD_2D) {
		// Define the minimum and maximum brightness values
		const double MIN_BRIGHTNESS = dat.brightness[0];
		const double MAX_BRIGHTNESS = dat.brightness[1];
		const uint8_t r1 = dat.colors[0];
		const uint8_t r2 = dat.colors[1];
		const uint8_t g1 = dat.colors[2];
		const uint8_t g2 = dat.colors[3];
		const uint8_t b1 = dat.colors[4];
		const uint8_t b2 = dat.colors[5];		
	
		uint32_t* colors = new uint32_t[width * height];

		auto op = [&](int x, int y) {
			// Calculate the brightness (y-axis)
			double brightness = MIN_BRIGHTNESS + (MAX_BRIGHTNESS - MIN_BRIGHTNESS) * (static_cast<double>(y) / height);

			// Interpolate between red (255, 0, 0) and blue (0, 0, 255) for the x-axis
			uint32_t color = interpolateColor(r1, g1, b1, r2, g2, b2, static_cast<double>(x) / width);

			// Adjust brightness
			uint32_t adjustedR = static_cast<uint32_t>(brightness * (color >> 16 & 255));
			uint32_t adjustedG = static_cast<uint32_t>(brightness * (color >> 8 & 255));
			uint32_t adjustedB = static_cast<uint32_t>(brightness * (color & 255));

			*(colors++) = RGBA(adjustedR, adjustedG, adjustedB);
		};
		for (int y = 0; y < height; ++y)
			for (int x = 0; x < width; ++x)
				if(dat.extra==1) op(y, x);
				else op(x,y);
		

		return colors - (width * height);

	}




	uint32_t make_3byte_rgb(uint8_t red, uint8_t green, uint8_t blue) {
		return ((red & 0x0ff) << 16) | ((green & 0x0ff) << 8) | ((blue & 0x0ff) << 0);
	}

	
	uint32_t make_4byte_rgba(uint8_t red, uint8_t green, uint8_t blue) {
		return ((red & 0x0ff) << 24) | ((green & 0x0ff) << 16) | ((blue & 0x0ff) << 8) | ((0 & 0x0ff) << 0);
	}

	uint32_t make_4byte_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
		return ((red & 0x0ff) << 24) | ((green & 0x0ff) << 16) | ((blue & 0x0ff) << 8) | ((alpha & 0x0ff) << 0);
	}

	uint32_t convert_3byte_to_4byte_rgb(uint32_t three_byte_color) {
		return three_byte_color << 8;
	}




}