#include "smd.h"

int CSmd::Load(const char* file)
{
	if (strcmp(strrchr(file, '.'), ".smd") != 0) return 0;

	FILE* f = fopen(file, "rt");
	if (!f) return 0;

#define readline() \
fgets(line, sizeof(line), f);\
if (strlen(line) < 3) continue;\
tok = strtok(line, DELIM);\
end = strcmp(tok, "end");\
if (end == 0) break

#define floatok() (float)atof(strtok(NULL, DELIM))
#define vec2tok() vec::vec2f{ floatok(), floatok() }
#define vec3tok() vec::vec3f{ floatok(), floatok(), floatok() }

	const char* DELIM = " \t\r\n";
	char line[256];
	char* tok;
	int end = 0;

	while (!feof(f))
	{
		fgets(line, sizeof(line), f);
		if (strlen(line) < 3) continue;
		tok = strtok(line, DELIM);

		if (strcmp(tok, "version") == 0)
		{
			printf("[SMD] version %s\n", strtok(NULL, DELIM));
			continue;
		}

		if (strcmp(tok, "nodes") == 0)
		{
			printf("[SMD] nodes\n");
			while (!feof(f))
			{
				readline();

				nodes.name = strtok(NULL, "\t\r\n\"");
				if (nodes.name.empty()) return 0;
				nodes.parent = atoi(strtok(NULL, DELIM));

				data.nodes.push_back(nodes);
			}
			if (end != 0) return 0;
			continue;
		}

		if (strcmp(tok, "skeleton") == 0)
		{
			printf("[SMD] skeleton\n");
			while (!feof(f))
			{
				readline();

				if (strcmp(tok, "time") == 0)
				{
					skeleton.time = atoi(strtok(NULL, DELIM));

					for (size_t i = 0; i < data.nodes.size(); i++)
					{
						if (feof(f)) return 0;
						readline();

						skeleton.pos.push_back(vec3tok());
						skeleton.rot.push_back(vec3tok());
					}
				}
				data.skeleton.push_back(skeleton);
			}
			if (end != 0) return 0;
			continue;
		}

		if (strcmp(tok, "triangles") == 0)
		{
			printf("[SMD] triangles\n");
			while (!feof(f))
			{
				readline();

				triangles.name = tok;
				if (triangles.name.empty()) return 0;

				for (int i = 0; i < 3; i++)
				{
					if (feof(f)) return 0;
					fgets(line, sizeof(line), f);
					if (strlen(line) < 3) continue;

					triangles.bone[i] = atoi(strtok(line, DELIM));
					triangles.pos[i] = vec3tok();
					triangles.norm[i] = vec3tok();
					triangles.uv[i] = vec2tok();
				}
				data.triangles.push_back(triangles);
			}
			if (end != 0) return 0;
			continue;
		}
	}

#undef floatok
#undef vec2tok
#undef vec3tok
#undef readline

	fclose(f);
	return 1;
}

int CSmd::Save(const char* file)
{
	if (strcmp(strrchr(file, '.'), ".smd") != 0) return 0;

	FILE* f = fopen(file, "wt");
	if (!f) return 0;

	fputs("version 1\n", f);

	fputs("nodes\n", f);
	for (size_t i = 0; i < data.nodes.size(); i++)
		fprintf(f, "%i \"%s\" %i\n", i, data.nodes[i].name.c_str(), data.nodes[i].parent);
	fputs("end\n", f);

	fputs("skeleton\n", f);
	for (size_t i = 0; i < data.skeleton.size(); i++)
	{
		fprintf(f, "time %i\n", data.skeleton[i].time);
		for (size_t j = 0; j < data.nodes.size(); j++)
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

	fputs("triangles\n", f);
	for (size_t i = 0; i < data.triangles.size(); i++)
	{
		fprintf(f, "%s\n", data.triangles[i].name.c_str());
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

	fclose(f);
	return 1;
}