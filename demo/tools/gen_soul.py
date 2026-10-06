import argparse
import math

import numpy as np
from PIL import Image, ImageDraw

from gen_tree import bleed_colors, pack, palette_by_name

KINDS = {"spirit": (21, (0, 0, 0), False), "wisp": (15, None, False), "shade": (21, (255, 255, 255), True)}
REACH = 84


def lit(color, power):
    return tuple(round(255 * (1 - (1 - c / 255) ** power)) for c in color)


def kinked(rng, count, s):
    drift = np.cumsum(rng.normal(0, 1.2 * s, count))
    return drift - drift[0]


def lock(rng, s, colors, core, count, head, tail, span):
    face = head < core
    tilt, roll, start = rng.uniform(0.3, 1), rng.uniform(0, math.tau), rng.uniform(0, math.tau)
    sway, sway_phase = rng.uniform(0.12, 0.3), rng.uniform(0, math.tau)
    whip, whip_phase = rng.uniform(0.18, 0.4), rng.uniform(0, math.tau)
    shade = rng.normal(0, 0.07)
    strands = []
    for _ in range(count):
        near = head + rng.normal(0, 2.6 * s)
        far = near + (tail - head) * rng.uniform(0.5, 1)
        arc = span * rng.uniform(0.6, 1)
        t = np.linspace(0, 1, int(np.clip(round((near + far) / 2 * arc / (7 * s)), 5, 30)))
        radius = np.clip(near + (far - near) * t ** rng.uniform(0.9, 1.5) + kinked(rng, len(t), s), 0.5 * s, REACH * s)
        tone = 0.96 - 0.3 * (radius / core) ** 2 if face else 0.78 - 0.76 * radius / (REACH * s)
        tone = np.clip(tone + shade + rng.normal(0, 0.05), 0, 0.999)
        if rng.uniform() < 0.06:
            tone[:] = 0
        alpha = np.clip(1.25 - radius / (REACH * s), 0, 1)
        fills = [colors[int(v * len(colors))] + (int(a * 255),) for v, a in zip(tone, alpha)]
        strands.append((tone.mean() + rng.uniform(0, 0.5), face, radius, start + rng.normal(0, 0.06) - arc * t, fills, tilt, roll,
                        sway, sway_phase, whip * min(1, abs(far - near) / (30 * s)) * t ** 1.5, whip_phase - 3 * t))
    return strands


def soul(rng, s, colors, core):
    strands = []
    for _ in range(300):
        head = core * math.sqrt(rng.uniform())
        strands += lock(rng, s, colors, core, 1, head, head, rng.uniform(1, 2.6) * core / max(head, core / 4))
    for _ in range(60):
        head = core + (38 * s - core) * rng.uniform() ** 1.3
        strands += lock(rng, s, colors, core, 1, head, head * rng.uniform(0.85, 1.35), rng.uniform(0.7, 2))
    for _ in range(13):
        head = rng.uniform(core, 38 * s)
        tail = min(head * rng.uniform(1.2, 3.4), REACH * s * rng.uniform(0.85, 1))
        strands += lock(rng, s, colors, core, 9, head, tail, rng.uniform(0.8, 3))
    return strands


def render(kind, seed, tile, frames, palette_name):
    s = tile / 192
    rng = np.random.default_rng(seed)
    core, eye_color, dark = KINDS[kind]
    leaf = palette_by_name(palette_name)["leaf"]
    colors = [lit(color, 3.5) for color in leaf[1:]] + [lit(leaf[4], 7)]
    if dark:
        colors[3:] = [leaf[1], leaf[0]]
    strands = soul(rng, s, colors, core * s)
    strands.sort(key=lambda strand: strand[0])
    origin = np.array([tile / 2, tile / 2])
    width = max(1, round(s))
    eye_x, eye_w, eye_h = 11 * s, 7 * s, 5 * s

    images = []
    for frame in range(frames):
        phase = math.tau * frame / frames
        boil = np.random.default_rng([seed, frame])
        bob = 2 * s * np.array([math.cos(phase), math.sin(2 * phase)])
        layers = [Image.new("RGBA", (tile, tile), (0, 0, 0, 0)) for _ in range(3)]
        draws = [ImageDraw.Draw(layer) for layer in layers]
        for _, face, radius, angle, fills, tilt, roll, sway, sway_phase, whip, whip_phase in strands:
            turn = angle + phase + sway * math.sin(phase + sway_phase) + whip * np.sin(2 * phase + whip_phase)
            reach = radius * (1 + 0.06 * np.sin(phase + sway_phase + whip_phase))
            x, y = reach * np.cos(turn), reach * tilt * np.sin(turn)
            moved = np.stack([x * math.cos(roll) - y * math.sin(roll), x * math.sin(roll) + y * math.cos(roll)], axis=1)
            moved += boil.normal(0, 0.45 * s, moved.shape) + bob + origin
            for i in range(len(moved) - 1):
                draw = draws[1 if face else 2 * int(math.sin(turn[i]) > 0)]
                draw.line([tuple(moved[i]), tuple(moved[i + 1])], fill=fills[i], width=width)
        image = Image.alpha_composite(Image.alpha_composite(layers[0], layers[1]), layers[2])
        if eye_color:
            draw = ImageDraw.Draw(image)
            for side in (-1, 1):
                cx, cy = side * eye_x + bob[0] + origin[0], -s + bob[1] + origin[1]
                draw.polygon([(cx - eye_w, cy), (cx, cy - eye_h), (cx + eye_w, cy), (cx, cy + eye_h)], fill=eye_color + (255,))
        images.append(bleed_colors(np.asarray(image)))
    return images


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--kind", default="spirit", choices=sorted(KINDS))
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--tile", type=int, default=192)
    parser.add_argument("--frames", type=int, default=16)
    parser.add_argument("--columns", type=int, default=4)
    parser.add_argument("--palette", default="violet", help="same names as gen_tree.py, or a hue in degrees (0-360)")
    args = parser.parse_args()
    frames = render(args.kind, args.seed, args.tile, args.frames, args.palette)
    pack(frames, args.columns).save(args.out)
    print(f"{args.out}: tile {args.tile}x{args.tile}, {args.frames} tiles, {args.columns} per row")
