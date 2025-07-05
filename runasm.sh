if [ $# -ne 1 ]; then
  echo "Usage: $0 file.asm"
  exit 1
fi

ASM="$1"
OBJ="${ASM%.*}.o"
BIN="${ASM%.*}"

nasm -felf64 "$ASM" -o "$OBJ"                # assemble
# link object with no PIE to avoid relocation errors and disable executable stack
gcc -no-pie "$OBJ" -o "$BIN" -Wl,-z,noexecstack
if [ $? -ne 0 ]; then
  echo "link failed"
  exit 1
fi

./"$BIN"                                    # run
