import argparse

import numpy as np
from PIL import Image


def render(size, peak, edge, color):
    half = (size - 1) / 2
    y, x = np.mgrid[0:size, 0:size]
    r = np.clip(np.hypot(x - half, y - half) / half, 0, 1)
    brightness = peak * (1 - r * r) ** edge
    pixels = np.empty((size, size, 4), np.uint8)
    pixels[..., :3] = np.clip(brightness[..., None] * np.array(color) / 255 + 0.5, 0, 255)
    pixels[..., 3] = 255
    return Image.fromarray(pixels, "RGBA")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--size", type=int, default=512)
    parser.add_argument("--peak", type=float, default=220, help="brightness in the centre, 0-255")
    parser.add_argument("--edge", type=float, default=2,
                        help="1 is a plain 1 - r^2 with a visible rim, 2 fades without a rim, higher gathers the light closer to the centre")
    parser.add_argument("--color", default="255,255,255", help="r,g,b")
    args = parser.parse_args()
    color = tuple(int(c) for c in args.color.split(","))
    render(args.size, args.peak, args.edge, color).save(args.out)
    print(f"{args.out}: {args.size}x{args.size}, peak {args.peak:g}, edge {args.edge:g}")
