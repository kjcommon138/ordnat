#include "jointmesh.h"

int
main(int argc, char* argv[])
{
  struct joint_mesh graph_mesh = alloc_joint_mesh(malloc, 10, 20, 10);

  /* Evaluate mesh positions */
  foreach_range_joint_mesh_position(graph_mesh, position, c, {
    position->x = c + 3.2;
    position->y = c * 2.9;
    position->z = c + 5.8;
  });

  foreach_range_joint_mesh_position(graph_mesh, position, c, {
    printf("Position [%d]: x -> %f", c, position->x);
    printf("; y -> %f", position->y);
    printf("; z -> %f\n", position->z);
  });
  printf("\n");

  /* Evaluate indices */
  foreach_range_joint_mesh_index(graph_mesh, index, c, {
    *index = c * 8;
  });

  foreach_range_joint_mesh_index(graph_mesh, index, c, {
    printf("Index [%d]: x -> %d\n", c, *index);
  });
  printf("\n");

  /* Evaluate mesh uvs */
  foreach_range_joint_mesh_uv(graph_mesh, uv, c, {
    uv->x = (c + 5.0) / 0.3;
    uv->y = c * 3.7;
  });

  foreach_range_joint_mesh_uv(graph_mesh, uv, c, {
    printf("UV [%d]: x -> %f", c, uv->x);
    printf("; y -> %f\n", uv->y);
  });

  struct joint_mesh mesh_copy = {0};

  struct joint_mesh *mesh_copy_result = copy_joint_mesh(malloc, free, &graph_mesh, &mesh_copy);
  assert(mesh_copy_result != NULL);

  graph_mesh = realloc_joint_mesh(malloc, free, &graph_mesh, 5, 20, 19);

  assert(graph_mesh.memory_block != NULL);

  /* Verify memory is copied to new mesh */
  printf("\nVerifying memory copy...\n");

  foreach_range_joint_mesh_position(*mesh_copy_result, position, c, {
    printf("Position [%d]: x -> %f", c, position->x);
    printf("; y -> %f", position->y);
    printf("; z -> %f\n", position->z);
  });
  printf("\n");

  foreach_range_joint_mesh_index(*mesh_copy_result, index, c, {
    printf("Index [%d]: x -> %i\n", c, *index);
  });
  printf("\n");

  foreach_range_joint_mesh_uv(*mesh_copy_result, uv, c, {
    printf("UV [%d]: x -> %f", c, uv->x);
    printf("; y -> %f\n", uv->y);
  });

  /* Verify memory is realloc'd correctly */
  printf("\nVerifying reallocated memory...\n");

  foreach_range_joint_mesh_position(graph_mesh, position, c, {
    printf("Position [%d]: x -> %f", c, position->x);
    printf("; y -> %f", position->y);
    printf("; z -> %f\n", position->z);
  });
  printf("\n");

  foreach_range_joint_mesh_index(graph_mesh, index, c, {
    printf("Index [%d]: x -> %i\n", c, *index);
  });
  printf("\n");

  foreach_range_joint_mesh_uv(graph_mesh, uv, c, {
    printf("UV [%d]: x -> %f", c, uv->x);
    printf("; y -> %f\n", uv->y);
  });

  /* Don't forget to clean up memory block */
  free_joint_mesh(free, &graph_mesh);
  free_joint_mesh(free, mesh_copy_result);

  return 0;
}
