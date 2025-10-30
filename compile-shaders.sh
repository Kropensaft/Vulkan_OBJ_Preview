#!/bin/bash
# Directory containing the shader files
SHADER_DIR="../shaders"

# Ensure the shader directory exists
if [ ! -d "$SHADER_DIR" ]; then
  echo "Shader directory $SHADER_DIR does not exist."
  exit 1
fi

# Ensure the output directory exists
OUTPUT_DIR="shaders/"
mkdir -p "$OUTPUT_DIR"

# Compile all .vert and .frag files
for shader_file in "$SHADER_DIR"/*.vert "$SHADER_DIR"/*.frag; do
  # Skip if no files match the pattern
  [[ ! -f "$shader_file" ]] && continue

  # Get the filename without extension
  filename=$(basename "$shader_file" .vert)
  filename=${filename%.frag}

  # Output file path
  output_file="$OUTPUT_DIR/${filename}.spv"

  # Compile using glslc
  glslc "$shader_file" -o "$output_file"
  echo "Compiled $shader_file -> $output_file"
done

echo "All shaders compiled."
