#!/usr/bin/env python3
import sys


def main() -> int:
    first_arg = len(sys.argv) > 1
    second_arg = len(sys.argv) > 2

    sys.stdin = open(sys.argv[1] if first_arg else sys.stdin.fileno(), 'r')
    sys.stdout = open(sys.argv[2] if second_arg else sys.stdout.fileno(), 'wb')

    offset = 0
    for c in sys.stdin.read():
        if offset > 0xfffc:
            print("`offset` has reached its maximum limit!\n"
                  "The original input is probably "
                  "too large. The output is trunced.", file=sys.stderr)
            sys.stdin.close()
            sys.stdout.close()
            return 1
        if c.isspace():
            continue
        if '1' <= c <= '4':
            out = ((offset << 2) & 0xffff) | (ord(c) - ord('1'))
            sys.stdout.write(out.to_bytes(2, "big", signed=False))
        offset += 1

    sys.stdin.close()
    sys.stdout.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
