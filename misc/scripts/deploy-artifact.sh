#!/bin/bash
set -e

# Copy files to folder.
mkdir -p ./artifact/${SD_OUT}
cp ${OUT}/* ./artifact/${SD_OUT}