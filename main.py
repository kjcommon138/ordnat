#!/usr/bin/python3
import argparse
from contextlib import redirect_stdout
import os
import sys
import toml


def get_toml_from_file(filename):
    """Returns the TOML structure for the given filename."""
    with open(filename, 'r') as f:
        parsed_toml = toml.load(f)

        if len(parsed_toml) != 0:
            return parsed_toml

    return None


def generate_c_code(filename, output):
    """Create header file for joint allocated structures in the TOML file."""
    parsed_toml = get_toml_from_file(filename)

    if parsed_toml is None:
        return

    if output:
        with open(output, 'w') as f:
            with redirect_stdout(f):
                print_data_structs(parsed_toml, f.name)
    else:
        print_data_structs(parsed_toml)


def print_data_structs(parsed_toml, output='joint_memory.h'):
    """Write header file with function declarations and library imports"""
    filename = os.path.basename(output)
    header_name = filename.upper().replace('.', '_')

    # Single include pragma prologue
    print("/* Code generated by \"ordnat\". DO NOT EDIT. */")
    print("#ifndef __{0}__".format(header_name))
    print("#define __{0}__\n".format(header_name))

    # Create library imports
    print("#include <stdlib.h>")
    print("#include <stdint.h>")
    print("#include <string.h>")
    print("#include <assert.h>")

    if parsed_toml['header']:
        for library in parsed_toml['header']['lib']:
            print("#include {0}".format(library))

    # Generate data structure with joint memory
    for (struct_name, struct_members) in parsed_toml['data'].items():
        field_count = len(struct_members)
        if field_count == 0:
            print("[WARN] Struct {0} has no data members".format(struct_name), file=sys.stderr)
            continue

        print("\n/* {0} structure with joint memory allocation */".format(struct_name))

        print("struct joint_{0} {{".format(struct_name))

        for (i, field) in enumerate(struct_members):
            print("  {0} *{1};".format(field['type'], field['name']))

        print("\n  void *memory_block;")
        print("  int array_count;")
        print("  int32_t offset[{0}];".format(field_count))
        print("};\n")

        # Generate allocation method
        print_alloc_method(struct_name, field_count, struct_members)

        # Generate copy method
        print_copy_method(struct_name, field_count, struct_members)

    # Header file epilogue
    print("#endif /* __{0}__ */".format(header_name))


def print_alloc_method(struct_name, field_count, struct_members):
    """Generate function to allocate joint data structure."""
    field_params = [('n_' + field['name']) for field in struct_members]
    param_str = ', '.join(["size_t {0}".format(i) for i in field_params])

    print("static inline struct joint_{0} alloc_joint_{0}(void *(*mem_allocator)(size_t), {1}) {{".format(struct_name, param_str))
    print("  int32_t current_offset = 0;")
    print("  struct joint_{0} data = {{ 0 }};".format(struct_name))
    print("  data.array_count = {0};\n".format(field_count))

    for (i, field) in enumerate(struct_members):
        print("  current_offset += ((sizeof({0})) * ({1}));".format(field['type'], field_params[i]))
        print("  data.offset[{0}] = current_offset;".format(i))
        print()

    print("  data.memory_block = mem_allocator(current_offset);\n")

    for (i, field) in enumerate(struct_members):
        if i == 0:
            print("  data.{0} = ({1} *)(data.memory_block);".format(field['name'], field['type']))
        else:
            print("  data.{0} = ({1} *)((char *)(data.memory_block) + (data.offset[{2}]));".format(field['name'], field['type'], i - 1))

    print("\n  return data;")
    print("}\n")


def print_copy_method(struct_name, field_count, struct_members):
    """Generate copy function for joint data structure."""
    print("struct joint_{0} *joint_{0}_copy(void *(*mem_allocator)(size_t), struct joint_{0} *lhs, struct joint_{0} *rhs) {{".format(struct_name))
    print("  if (lhs == rhs) {")
    print("    return rhs;")
    print("  }")

    print("  assert(((void) \"lhs cannot be null\", lhs != NULL));")
    print("  assert(((void) \"rhs cannot be null\", rhs != NULL));")

    # Copy memory_block from lhs to rhs
    print("\n  if (lhs->memory_block != rhs->memory_block) {")
    print("    rhs->memory_block = mem_allocator(lhs->offset[{0}]);".format(field_count - 1))
    print("    if (rhs->memory_block == NULL) {")
    print("      return NULL;")
    print("    }")
    print("    memcpy(rhs->memory_block, lhs->memory_block, lhs->offset[{0}]);".format(field_count - 1))
    print("  }")

    # Copy offsets from lhs to rhs
    print("\n  rhs->array_count = lhs->array_count;\n")

    for (i, field) in enumerate(struct_members):
        print("  rhs->offset[{0}] = lhs->offset[{0}];".format(i))

    print()
    for (i, field) in enumerate(struct_members):
        if i == 0:
            print("  rhs->{0} = ({1} *)(rhs->memory_block);".format(field['name'], field['type']))
        else:
            print("  rhs->{0} = ({1} *)((char *)(rhs->memory_block) + (rhs->offset[{2}]));".format(field['name'], field['type'], i - 1))

    print("\n  return rhs;")
    print("}\n")


def parse_args():
    argparser = argparse.ArgumentParser('Create C code for joint memory allocation')
    argparser.add_argument('filename',
                           default='',
                           help='name of file to parse')

    argparser.add_argument('-o',
                           '--output',
                           default='',
                           help='output file')

    return argparser.parse_args()


def main():
    args = parse_args()
    generate_c_code(args.filename, args.output)


if __name__ == "__main__":
    main()
