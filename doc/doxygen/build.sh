#!/bin/bash

set -e # exit on error

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

REPO_ROOT="$DIR/../.."
BUILD_DIR="$REPO_ROOT/cmake-build-doxygen"

mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"

cmake ..

make doc_uml
make doc_doxygen

echo "Doxygen build done"
