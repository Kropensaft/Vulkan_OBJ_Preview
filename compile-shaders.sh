#!/bin/bash

SHADER_DIR="shaders"

if [ ! -d "$SHADER_DIR" ]; then
  echo "Error: Shader source directory '$SHADER_DIR' does not exist."
  echo "Please ensure this script is run from the project's root directory."
  exit 1
fi

echo "Starting shader compilation..."

for shader_file in "$SHADER_DIR"/*.vert "$SHADER_DIR"/*.frag; do

  [ -e "$shader_file" ] || continue

  output_file="${shader_file%.*}.spv"

  glslc "$shader_file" -o "$output_file"

  if [ $? -eq 0 ]; then
    echo "  Successfully compiled $shader_file -> $output_file"
  else
    echo "  ERROR: Failed to compile $shader_file."
    exit 1
  fi
done

echo "Shader compilation finished successfully."
