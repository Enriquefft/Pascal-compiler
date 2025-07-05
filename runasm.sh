if [ $# -ne 1 ]; then
  echo "Usage: $0 file.asm"
  exit 1
fi

ASM="$1"
OBJ="${ASM%.*}.o"
BIN="${ASM%.*}"

nasm -felf64 "$ASM" -o "$OBJ"                # assemble
gcc "$OBJ" -o "$BIN" -Wl,-z,noexecstack     # link, disable exec stack :contentReference[oaicite:0]{index=0}
if [ $? -ne 0 ]; then
  echo "link failed"
  exit 1
fi

./"$BIN"                                    # run
