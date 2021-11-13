#include "smd.h"

int CSmd::Load(const char* file)
{
#define floatok (float)atof(strtok(NULL, DELIM))
#define vec2tok vec::vec2f{ floatok, floatok }
#define vec3tok vec::vec3f{ floatok, floatok, floatok }

	if (strcmp(strrchr(file, '.'), ".smd") != 0) return 0;

	FILE* f = fopen(file, "rt");
	if (!f) return 0;

	const char* DELIM = " \t\r\n";
	char line[256];
	char* tok;
	int end = 0;

	while (!feof(f))
	{
		if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;

		if (strcmp(tok, "nodes") == 0)
		{
			while (!feof(f))
			{
				if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;
				if ((end = strcmp(tok, "end")) == 0) break;

				tok = strtok(NULL, "\""); // Bone name
				data.nodes_parent.push_back(atoi(strtok(NULL, DELIM)));
			}
			if (end != 0) return 0;
			continue;
		}

		if (strcmp(tok, "skeleton") == 0)
		{
			skeleton_t skeleton;
			while (!feof(f))
			{
				if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;
				if ((end = strcmp(tok, "end")) == 0) break;

				if (strcmp(tok, "time") == 0)
				{
					tok = strtok(NULL, DELIM); // Time count

					for (size_t i = 0; i < data.nodes_parent.size(); i++)
					{
						if (feof(f)) return 0;
						if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;

						skeleton.pos.push_back(vec3tok);
						skeleton.rot.push_back(vec3tok);
					}
					data.skeleton.push_back(skeleton);
				}
			}
			if (end != 0) return 0;
			continue;
		}

		if (strcmp(tok, "triangles") == 0)
		{
			triangles_t triangles;
			while (!feof(f))
			{
				if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;
				if ((end = strcmp(tok, "end")) == 0) break;

				triangles.matid = MaterialID(tok);

				for (int i = 0; i < 3; i++)
				{
					if (feof(f)) return 0;
					if ((tok = ReadLine(line, sizeof(line), f)) == 0) continue;

					triangles.bone[i] = atoi(tok);
					triangles.pos[i] = vec3tok;
					triangles.norm[i] = vec3tok;
					triangles.uv[i] = vec2tok;
				}
				data.triangles.push_back(triangles);
			}
			if (end != 0) return 0;
			continue;
		}
	}

	fclose(f);
	return 1;

#undef floatok
#undef vec2tok
#undef vec3tok
}

int CSmd::Save(const char* file)
{
	if (strcmp(strrchr(file, '.'), ".smd") != 0) return 0;

	FILE* f = fopen(file, "wt");
	if (!f) return 0;

	fputs("version 1\n", f);

	fputs("nodes\n", f);
	for (size_t i = 0; i < data.nodes_parent.size(); i++)
		fprintf(f, "%i \"bone%i\" %i\n", i, i, data.nodes_parent[i]);
	fputs("end\n", f);

	fputs("skeleton\n", f);
	for (size_t i = 0; i < data.skeleton.size(); i++)
	{
		fprintf(f, "time %i\n", i);
		for (size_t j = 0; j < data.nodes_parent.size(); j++)
		{
			fprintf(f, "%i %f %f %f %f %f %f\n", j,
				data.skeleton[i].pos[j].x,
				data.skeleton[i].pos[j].y,
				data.skeleton[i].pos[j].z,
				data.skeleton[i].rot[j].x,
				data.skeleton[i].rot[j].y,
				data.skeleton[i].rot[j].z);
		}
	}
	fputs("end\n", f);

	if (data.triangles.size() != 0)
	{
		fputs("triangles\n", f);
		for (size_t i = 0; i < data.triangles.size(); i++)
		{
			fprintf(f, "%s\n", data.material[data.triangles[i].matid].c_str());
			for (int j = 0; j < 3; j++)
			{
				fprintf(f, "%i %f %f %f %f %f %f %f %f\n",
					data.triangles[i].bone[j],
					data.triangles[i].pos[j].x,
					data.triangles[i].pos[j].y,
					data.triangles[i].pos[j].z,
					data.triangles[i].norm[j].x,
					data.triangles[i].norm[j].y,
					data.triangles[i].norm[j].z,
					data.triangles[i].uv[j].x,
					data.triangles[i].uv[j].y);
			}
		}
		fputs("end\n", f);
	}

	fclose(f);
	return 1;
}

char* CSmd::ReadLine(char* line, int count, FILE* file)
{
	fgets(line, count, file);
	if (strlen(line) < 3) return 0;
	return strtok(line, " \t\r\n");
}

int CSmd::MaterialID(const char* name)
{
	int countmat = data.material.size();
	for (int i = 0; i < countmat; i++)
	{
		if (strcmp(name, data.material[i].c_str()) == 0)
			return i;
	}
	data.material.push_back(name);
	return countmat;
}
