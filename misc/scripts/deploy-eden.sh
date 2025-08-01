#!/bin/bash
set -e

# Verify the path is set.
if [ -z "${EDEN_PATH}" ]; then
    echo "EDEN_PATH appears to not be set! Check your exlaunch.sh?"
    exit 1
fi

# Setup the path to the game's mods folder.
export MODS_PATH=${EDEN_PATH}/load/${PROGRAM_ID}/swsh-mods-exl

# Ensure directory exists.
mkdir -p ${MODS_PATH}/
mkdir -p ${EDEN_PATH}/sdmc/config/swsh-mods-exl

# Copy over files.
cp -r ${OUT}/* ${MODS_PATH}/
cp config.toml ${EDEN_PATH}/sdmc/config/swsh-mods-exl/