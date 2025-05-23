#!/bin/bash
set -e

# Copy files to folder.
mkdir -p ./artifact/${SD_OUT}
mkdir -p ./artifact/config/ironmon
cp -r ${OUT}/* ./artifact/${SD_OUT}
cp config.toml ./artifact/config/ironmon/