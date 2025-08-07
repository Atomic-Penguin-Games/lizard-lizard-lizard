#!/bin/bash

# Simple web server for testing the web build
# Serves the web build directory on localhost

PORT=${1:-8000}
BUILD_DIR="build_web"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Web build directory '$BUILD_DIR' not found."
    echo "Run './build.sh -t web' first to create a web build."
    exit 1
fi

if [ ! -f "$BUILD_DIR/lizard_meme.html" ]; then
    echo "Error: lizard_meme.html not found in $BUILD_DIR."
    echo "Make sure the web build completed successfully."
    exit 1
fi

echo "Starting web server on port $PORT..."
echo "Open your browser to: http://localhost:$PORT/lizard_meme.html"
echo "Press Ctrl+C to stop the server."

cd "$BUILD_DIR"
python3 -m http.server "$PORT"
