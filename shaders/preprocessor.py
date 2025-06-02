#!/usr/bin/env python3
import sys
import os
import re

input_name = sys.argv[1]
loaded_shader = set()
loading_shader = set()
parsed_shader = ""

def parse_shader(filename: str):
    global parsed_shader

    loading_shader.add(filename)
    file = open(os.path.join("shaders", filename), 'r')
    for line_number, line in enumerate(file.read().split('\n')):
        match = re.search(r'^#include\s*"(.*?)"\s*$', line)
        if match is not None:
            include_source = match[1]
            if include_source in loading_shader:
                print(f"{filename}:{line_number + 1}: error: including shader file {include_source} causes cyclic inclusion", file=sys.stderr)
                exit(1)
            elif include_source in loaded_shader:
                parsed_shader += f'/// include file "{include_source}" skipped (duplicated include)\n'
            else:
                parsed_shader += f'/// file "{include_source}" start\n'
                try:
                    parse_shader(include_source)
                except FileNotFoundError:
                    print(f"{filename}:{line_number + 1}: error: included shader file {include_source} not found", file=sys.stderr)
                    exit(1)
                parsed_shader += f'/// file "{include_source}" end\n'
        else:
            parsed_shader += line + '\n'
    loading_shader.remove(filename)
    loaded_shader.add(filename)

try:
    parse_shader(input_name)
except FileNotFoundError:
    print(f"Input shader file {input_name} not found.", file=sys.stderr)
    exit(1)
print(parsed_shader)