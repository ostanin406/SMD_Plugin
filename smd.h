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
	struct nodes_t
	{
		std::string name;
		int parent = 0;
	};

	struct skeleton_t
	{
		int time = 0;
		std::vector <vec::vec3f> pos, rot;
	};

	struct triangles_t
	{
		std::string name;
		int bone[3];
		vec::vec3f pos[3], norm[3];
		vec::vec2f uv[3];
	};

	struct data_t
	{
		std::vector <nodes_t> nodes;
		std::vector <skeleton_t> skeleton;
		std::vector <triangles_t> triangles;
	};

	nodes_t nodes;
	skeleton_t skeleton;
	triangles_t triangles;

public:
	data_t data;
};