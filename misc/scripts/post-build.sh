#!/bin/bash
set -e

OUT_NSO=${OUT}/exefs/${BINARY_NAME}
OUT_NPDM=${OUT}/exefs/main.npdm

# Clear older build.
rm -rf ${OUT}

# Create out directory.
mkdir ${OUT}
mkdir ${OUT}/exefs
mkdir ${OUT}/romfs

${PYTHON} ${SCRIPTS_PATH}/build_romfs.py

# Copy build into out
mv ${NAME}.nso ${OUT_NSO}
mv ${NAME}.npdm ${OUT_NPDM}

# Copy ELF to user path if defined.
if [ ! -z $ELF_EXTRACT ]; then
    cp "$NAME.elf" "$ELF_EXTRACT"
fi
