import argparse
import math

import numpy as np
from PIL import Image, ImageDraw

from gen_tree import bleed_colors, pack, palette_by_name

SHADES = 5


def kinked(rng, x, y, s):
    drift = np.cumsum(rng.normal(0, 1.6 * s, len(x)))
    return np.stack([x + drift - drift[0], y], axis=1)


def pick_color(rng, palette, shade):
    index = int(np.clip(shade + rng.normal(0, 0.2), 0, 0.999) * SHADES)
    return index, palette["leaf"][index]


def mop(rng, s, palette):
    half_width, top, bottom = 54 * s, -72 * s, 48 * s
    strands = []
    for _ in range(520):
        x0 = float(np.clip(rng.normal(0, half_width * 0.5), -half_width * 0.95, half_width * 0.95))
        edge = (x0 / half_width) ** 2
        y0 = top + 46 * s * edge + rng.uniform(0, 16) * s
        y1 = bottom - 14 * s * edge + rng.normal(0, 9) * s
        t = np.linspace(0, 1, rng.integers(6, 10))
        verts = kinked(rng, x0 * (1 + rng.uniform(0.05, 0.3) * t), y0 + (y1 - y0) * t, s)
        index, color = pick_color(rng, palette, 0.78 - 0.45 * edge)
        if rng.uniform() < 0.06:
            index, color = SHADES, palette["leaf"][1]
        strands.append((index, verts, color, t ** 1.6, rng.uniform(1.5, 4.5) * s, 0.045 * x0 / s + rng.normal(0, 0.4)))
    for _ in range(26):
        x0 = rng.normal(0, half_width * 0.35)
        y0 = top + 46 * s * (x0 / half_width) ** 2 + rng.uniform(0, 8) * s
        angle = -math.pi / 2 + rng.normal(0, 0.6)
        t = np.linspace(0, 1, 4)
        reach = rng.uniform(8, 20) * s
        verts = kinked(rng, x0 + math.cos(angle) * reach * t, y0 + math.sin(angle) * reach * t, s * 0.6)
        index, color = pick_color(rng, palette, 0.7)
        strands.append((index, verts, color, t ** 1.6, rng.uniform(1, 2.5) * s, 0.045 * x0 / s + rng.normal(0, 0.4)))
    eyes = [(-20 * s, top + 40 * s), (20 * s, top + 40 * s)]
    return strands, eyes, np.array([0.0, top])


def ball(rng, s, palette):
    center, radius = np.array([0.0, 6 * s]), 46 * s
    strands = []
    for _ in range(560):
        around = rng.uniform(0, math.tau)
        inner = math.sqrt(rng.uniform())
        p = center + radius * 0.55 * inner * np.array([math.cos(around), math.sin(around)])
        angle = around + rng.normal(0, 0.5)
        direction = np.array([math.cos(angle), math.sin(angle)])
        hop = rng.uniform(0.55, 1.15) * radius / 6
        verts = [p]
        for _ in range(6):
            direction = direction + np.array([0.0, 0.18])
            direction /= np.linalg.norm(direction)
            turn = rng.normal(0, 0.2)
            c, sn = math.cos(turn), math.sin(turn)
            direction = np.array([direction[0] * c - direction[1] * sn, direction[0] * sn + direction[1] * c])
            p = p + direction * hop
            verts.append(p)
        verts = np.array(verts)
        t = np.linspace(0, 1, len(verts))
        index, color = pick_color(rng, palette, 0.8 - 0.4 * inner ** 2)
        if rng.uniform() < 0.06:
            index, color = SHADES, palette["leaf"][1]
        strands.append((index, verts, color, t ** 1.6, rng.uniform(1.5, 4) * s, 0.045 * verts[0][0] / s + rng.normal(0, 0.4)))
    eyes = [(-15 * s, -4 * s), (15 * s, -4 * s)]
    return strands, eyes, center


KINDS = {"mop": mop, "ball": ball}


def render(kind, seed, tile, frames, palette_name, eye_color):
    s = tile / 192
    rng = np.random.default_rng(seed)
    palette = palette_by_name(palette_name)
    strands, eyes, anchor = KINDS[kind](rng, s, palette)
    strands.sort(key=lambda strand: strand[0] + rng.uniform(0, 2.5))
    if eye_color is None:
        eye_color = (0, 0, 0) if sum(palette["leaf"][3]) > 300 else (255, 255, 255)
    origin = np.array([tile / 2, tile / 2])
    width = max(1, round(s))
    eye_w, eye_h = 13 * s, 8 * s

    images = []
    for frame in range(frames):
        phase = math.tau * frame / frames
        boil = np.random.default_rng([seed, frame])
        bob = np.array([0.0, 1.6 * s * math.sin(phase)])
        breath = np.array([1 - 0.02 * math.sin(phase + 0.6), 1 + 0.03 * math.sin(phase + 0.6)])
        image = Image.new("RGBA", (tile, tile), (0, 0, 0, 0))
        draw = ImageDraw.Draw(image)
        for _, verts, color, weight, sway, sway_phase in strands:
            moved = anchor + (verts - anchor) * breath
            moved[:, 0] += weight * sway * math.sin(phase + sway_phase)
            moved += boil.normal(0, 0.45 * s, moved.shape) + bob + origin
            draw.line([tuple(v) for v in moved], fill=color + (255,), width=width)
        for ex, ey in eyes:
            cx, cy = ex + bob[0] + origin[0], ey + bob[1] + origin[1]
            draw.polygon([(cx - eye_w, cy), (cx, cy - eye_h), (cx + eye_w, cy), (cx, cy + eye_h)], fill=eye_color + (255,))
        images.append(bleed_colors(np.asarray(image)))
    return images


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--kind", default="mop", choices=sorted(KINDS))
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--tile", type=int, default=192)
    parser.add_argument("--frames", type=int, default=16)
    parser.add_argument("--columns", type=int, default=4)
    parser.add_argument("--palette", default="bone", help="same names as gen_tree.py, or a hue in degrees (0-360)")
    parser.add_argument("--eyes", default=None, help="r,g,b; by default black on a light body and white on a dark one")
    args = parser.parse_args()
    eye_color = tuple(int(c) for c in args.eyes.split(",")) if args.eyes else None
    frames = render(args.kind, args.seed, args.tile, args.frames, args.palette, eye_color)
    pack(frames, args.columns).save(args.out)
    print(f"{args.out}: tile {args.tile}x{args.tile}, {args.frames} tiles, {args.columns} per row")
