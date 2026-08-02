"""Command-line entry point for seam carving."""

import sys

from seamcarving.core import compress_to_file


def main():
    if len(sys.argv) != 5:
        print(
            "Usage: seamcarving-cli <input> <output> <desired_w> <desired_h>",
            file=sys.stderr,
        )
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    desired_w = int(sys.argv[3])
    desired_h = int(sys.argv[4])

    try:
        v_seams, h_seams, elapsed = compress_to_file(
            input_file, output_file, desired_w, desired_h
        )
    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
    except FileNotFoundError:
        print(f"Error: cannot read image {input_file}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

    total_seams = len(v_seams) + len(h_seams)
    print(f"Saved compressed image to {output_file} ({desired_w}x{desired_h})")
    print(f"  Removed {len(v_seams)} vertical + {len(h_seams)} horizontal = "
          f"{total_seams} seams in {elapsed:.2f} ms")


if __name__ == "__main__":
    main()