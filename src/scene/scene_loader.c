#include "scene_loader.h"

#include "../player/player.h"
#include "../util/constants.h"

scene_t* current_scene = NULL;

void load_collision(scene_t* scene) {
	int file_handle = dfs_open(scene->collision.path);
	int size = dfs_size(file_handle);
	char* buffer = malloc(size);
	dfs_read(buffer, 1, size, file_handle);
	dfs_close(file_handle);

	char* line;
	char* buffer_dup = strdup(buffer);
	char* rest = buffer_dup;
	while ((line = strtok_r(rest, "\n", &rest))) {
		if (line[0] == 'v') {
			scene->collision.mesh.vertex_count++;
		} else if (line[0] == 'n') {
			scene->collision.mesh.normal_count++;
		} else if (line[0] == 't') {
			scene->collision.mesh.tri_count++;
		}
	}
	free(buffer_dup);

	scene->collision.mesh.vertices = malloc(scene->collision.mesh.vertex_count * sizeof(fm_vec3_t));
	scene->collision.mesh.normals = malloc(scene->collision.mesh.normal_count * sizeof(fm_vec3_t));
	scene->collision.mesh.tris = malloc(scene->collision.mesh.tri_count * sizeof(collision_tri_t));

	int vertex_i = 0;
	int normal_i = 0;
	int tri_i = 0;
	buffer_dup = strdup(buffer);
	rest = buffer_dup;
	while ((line = strtok_r(rest, "\n", &rest))) {
		if (line[0] == 'v') {
			fm_vec3_t vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			scene->collision.mesh.vertices[vertex_i] = vertex;
			vertex_i++;
		} else if (line[0] == 'n') {
			fm_vec3_t normal;
			sscanf(line, "n %f %f %f", &normal.x, &normal.y, &normal.z);
			fm_vec3_norm(&scene->collision.mesh.normals[normal_i], &normal);
			normal_i++;
		} else if (line[0] == 't') {
			collision_tri_t tri;
			sscanf(line, "t %hd %hd %hd %hd", &tri.v1, &tri.v2, &tri.v3, &tri.normal);
			scene->collision.mesh.tris[tri_i] = tri;
			tri_i++;
		}
	}
	free(buffer_dup);

	free(buffer);
}

void scene_load(scene_t* scene) {
	if (current_scene != NULL) {
		scene_kill(current_scene);
	}

	current_scene = scene;
	player_reset();
	current_scene->load(current_scene);

	if (current_scene->collision.path) {
		load_collision(current_scene);
	}
}
