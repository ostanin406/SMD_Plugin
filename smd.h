#pragma once

#include <vector>
#include <string>
#include "vec.h"

class CSmd
{
public:
	int Load(const char* file);
	int Save(const char* file);

private:
	char* ReadLine(char* line, int count, FILE* file);
	int MaterialID(const char* name);

	struct skeleton_t { std::vector <vec::vec3f> pos, rot; };

	struct triangles_t
	{
		int bone[3], matid;
		vec::vec3f pos[3], norm[3];
		vec::vec2f uv[3];
	};

	struct data_t
	{
		std::vector <int> nodes_parent;
		std::vector <skeleton_t> skeleton;
		std::vector <triangles_t> triangles;
		std::vector <std::string> material;
	};

public:
	data_t data;
};
