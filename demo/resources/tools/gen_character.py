import argparse
import math

import numpy as np
from PIL import Image, ImageDraw

from gen_tree import bleed_colors, pack

THREADS = 330
TUFTS = 46


def crown(u):
    return 82 + 58 * abs(u) ** 5


def threads(rng, s):
    result = []
    for _ in range(THREADS):
        u = float(np.clip(rng.normal(0, 0.6), -1, 1))
        top = (crown(u) + 34 * rng.uniform() ** 2) * s
        bottom = (452 - 95 * u * u - 135 * rng.uniform() ** 1.2) * s
        result.append((45 * s * u, top, bottom))
    return result


def tufts(rng, s):
    result = []
    for _ in range(TUFTS):
        u = rng.uniform(-0.9, 0.9)
        result.append((40 * s * u, (crown(u) + rng.uniform(8, 40)) * s, rng.normal(-math.pi / 2, 0.55), rng.uniform(12, 30) * s))
    return result


def render(seed, tile, frames, turn=0.0):
    s = tile / 256
    base = np.random.default_rng(seed)
    body, hair = threads(base, s), tufts(base, s)
    eye_w, eye_h = 8.5 * s, 4 * s
    look, trail = 14 * s * turn, -26 * s * turn
    width = max(1, round(s))

    images = []
    for frame in range(frames):
        rng = np.random.default_rng([seed, frame])
        image = Image.new("RGBA", (tile, 2 * tile), (0, 0, 0, 0))
        draw = ImageDraw.Draw(image)
        for x, top, bottom in body:
            count = max(3, round((bottom - top) / (17 * s)))
            t = np.linspace(0, 1, count)
            xs = tile / 2 + x + np.cumsum(rng.normal(0, 1.9 * s, count)) + trail * t ** 1.5
            ys = top + (bottom - top) * t
            draw.line(list(zip(xs, ys)), fill=(0, 0, 0, 255), width=width)
        for x, y, angle, reach in hair:
            angle += rng.normal(0, 0.25)
            t = np.linspace(0, 1, 4)
            xs = tile / 2 + x + math.cos(angle) * reach * t + rng.normal(0, 2 * s, 4)
            ys = y + math.sin(angle) * reach * t + rng.normal(0, 2 * s, 4)
            draw.line(list(zip(xs, ys)), fill=(0, 0, 0, 255), width=width)
        for side in (-1, 1):
            cx, cy = tile / 2 + side * 17.5 * s + look, 143 * s
            w = eye_w * (1 - 0.45 * turn) if side > 0 else eye_w
            draw.polygon([(cx - w, cy), (cx, cy - eye_h), (cx + w, cy), (cx, cy + eye_h)], fill=(255, 255, 255, 255))
        images.append(bleed_colors(np.asarray(image)))
    return images


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--tile", type=int, default=256, help="tile width, the height is twice as big")
    parser.add_argument("--frames", type=int, default=64)
    parser.add_argument("--columns", type=int, default=8)
    parser.add_argument("--turn", type=float, default=0, help="0 looks at the camera, 1 is turned to the right")
    args = parser.parse_args()
    frames = render(args.seed, args.tile, args.frames, args.turn)
    pack(frames, args.columns).save(args.out)
    print(f"{args.out}: tile {args.tile}x{2 * args.tile}, {args.frames} tiles, {args.columns} per row")
