#include "jointmesh.h"

int
main(int argc, char* argv[])
{
  struct joint_mesh graph_mesh = alloc_joint_mesh(malloc, 10, 20, 10);

  /* Evaluate mesh positions */
  for (int c = 0; c < 10; c++) {
    graph_mesh.positions[c].x = c + 3.2;
    graph_mesh.positions[c].y = c * 2.9;
    graph_mesh.positions[c].z = c + 5.8;
  };

  for (int c = 0; c < 10; c++) {
    printf("Position [%d]: x -> %f", c, graph_mesh.positions[c].x);
    printf("; y -> %f", graph_mesh.positions[c].y);
    printf("; z -> %f\n", graph_mesh.positions[c].z);
  };
  printf("\n");

  /* Evaluate mesh uvs */
  for (int c = 0; c < 10; c++) {
    graph_mesh.uvs[c].x = (c + 5.0) / 0.3;
    graph_mesh.uvs[c].y = c * 3.7;
  };

  for (int c = 0; c < 10; c++) {
    printf("UV [%d]: x -> %f", c, graph_mesh.uvs[c].x);
    printf("; y -> %f\n", graph_mesh.uvs[c].y);
  };

  struct joint_mesh mesh_copy = {0};
  struct joint_mesh *result = joint_mesh_copy(malloc, &graph_mesh, &mesh_copy);

  assert(result != NULL);

  /* Verify memory is copied to new mesh */
  printf("\nVerifying memory copy...\n");
  for (int c = 0; c < 10; c++) {
    printf("Position [%d]: x -> %f", c, graph_mesh.positions[c].x);
    printf("; y -> %f", graph_mesh.positions[c].y);
    printf("; z -> %f\n", graph_mesh.positions[c].z);
  };
  printf("\n");

  for (int c = 0; c < 10; c++) {
    printf("UV [%d]: x -> %f", c, graph_mesh.uvs[c].x);
    printf("; y -> %f\n", graph_mesh.uvs[c].y);
  };

  /* Don't forget to clean up memory block */
  free(graph_mesh.memory_block);
  free(mesh_copy.memory_block);
 
  return 0;
}
