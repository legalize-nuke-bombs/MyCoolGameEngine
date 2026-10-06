import argparse
import math

import numpy as np
from PIL import Image, ImageDraw

from gen_tree import bleed_colors, pack

FIRE = [(66, 13, 9), (110, 21, 10), (158, 34, 12), (204, 58, 14), (238, 100, 18),
        (252, 148, 30), (255, 196, 62), (255, 229, 124), (255, 247, 210)]
BARK = [(13, 10, 9), (31, 22, 17), (54, 37, 26), (88, 54, 31), (132, 80, 38)]
REACH = 16


def pulse(x, sharpness):
    return (0.5 + 0.5 * np.cos(math.tau * x)) ** sharpness


def beat(rng, frames, lo, hi):
    return max(1, round(frames / rng.uniform(lo, hi))), rng.uniform(0, 1)


def cross(rng):
    logs = [((side * rng.uniform(102, 120), 6 + 16 * front + rng.normal(0, 2)),
             (-side * rng.uniform(10, 34), -34 + 20 * front + rng.normal(0, 3)), front)
            for side, front in ((-1, 0), (1, 0), (1, 1), (-1, 1))]
    return logs, -24, 0.25, 7


def teepee(rng):
    top = np.array([rng.normal(0, 4), rng.normal(-102, 4)])
    logs = []
    for x, y, front in ((-104, 8, 0), (102, 6, 0), (74, 26, 1), (-52, 28, 1)):
        foot = np.array([rng.normal(x, 4), rng.normal(y, 2)])
        logs.append((foot, foot + (top - foot) * rng.uniform(1.08, 1.22), front))
    return logs, -8, 0, 0


KINDS = {"cross": cross, "teepee": teepee}


def log(rng, s, frames, outer, inner, front):
    outer, inner = np.array(outer) * s, np.array(inner) * s
    length = np.linalg.norm(inner - outer)
    axis = (inner - outer) / length
    normal = np.array([axis[1], -axis[0]]) * (1 if axis[0] > 0 else -1)
    radius = rng.uniform(14, 17) * s
    cap = 0.42 * radius / length
    bark, embers = [], []
    for _ in range(250):
        across = rng.uniform(-1, 1)
        start = rng.uniform(-0.2, 0.8)
        bulge = cap * math.sqrt(1 - across ** 2)
        t = np.clip(np.linspace(start, start + rng.uniform(0.25, 0.6), rng.integers(4, 7)), -bulge, 1 + bulge)
        bend = across * 0.94 + np.cumsum(rng.normal(0, 0.05, len(t)))
        verts = outer + np.outer(t * length, axis) + np.outer(bend * radius, normal)
        near = t.mean()
        light = (0.06 + (0.5 + 0.5 * across) ** 1.3 * (0.3 + 0.7 * min(near / 0.75, 1))
                 - 0.6 * max(0, near - 0.75) / 0.25 + rng.normal(0, 0.08))
        bark.append((verts, BARK[int(np.clip(light, 0, 0.999) * len(BARK))]))
    if front:
        for _ in range(26):
            along = rng.uniform(-1, 1)
            t = np.linspace(-1, 1, 3) * rng.uniform(0.5, 1) * math.sqrt(1 - along ** 2)
            verts = (outer + np.outer(along * cap * length + rng.normal(0, 0.5 * s, 3), axis)
                     + np.outer(t * radius, normal))
            bark.append((verts, BARK[rng.choice([2, 3, 4], p=[0.3, 0.5, 0.2])]))
    for _ in range(40):
        near = rng.uniform(0.55, 1)
        t = near * length + np.array([-0.5, 0, 0.5]) * rng.uniform(4, 11) * s
        bend = rng.uniform(-0.3, 1) + rng.normal(0, 0.08, 3)
        verts = outer + np.outer(t, axis) + np.outer(bend * radius, normal)
        embers.append((verts, 0.2 + 0.6 * (near - 0.55) / 0.45 + rng.normal(0, 0.1),
                       rng.uniform(0.05, 0.2), *beat(rng, frames, 5, 16)))
    return bark, embers, front


def coals(rng, s, frames):
    embers = []
    for _ in range(220):
        around, inside = rng.uniform(0, math.tau), math.sqrt(rng.uniform())
        p = np.array([math.cos(around) * 46 * inside, 19 + math.sin(around) * 12 * inside]) * s
        angle = rng.normal(0, 0.5)
        t = np.array([-0.5, 0, 0.5]) * rng.uniform(3, 8) * s
        verts = p + np.outer(t, [math.cos(angle), math.sin(angle)]) + rng.normal(0, 0.8 * s, (3, 2))
        embers.append((verts, 0.85 - 0.6 * inside ** 2 + rng.normal(0, 0.1),
                       rng.uniform(0.05, 0.2), *beat(rng, frames, 5, 16)))
    return embers


def flames(rng, s, frames, hearth, slope, lip):
    tongues = [(0.0, 54, 200, 0.6, 0.8)]
    tongues += [((i + rng.uniform(0.2, 0.8)) / 6 * 2 - 1, rng.uniform(18, 28), 110, rng.uniform(0.72, 1.1), 1)
                for i in range(6)]
    tongues += [(rng.uniform(-1, 1), rng.uniform(5, 10), 20, rng.uniform(0.8, 1.1), 0.9) for _ in range(10)]
    rows = []
    for i, (place, half, count, reach, heat) in enumerate(tongues):
        central = 1 - place ** 2
        longest = min(13.5, (6.5 + 7 * central) * reach)
        cycles, phase = beat(rng, frames, 6, 12)
        wave_cycles, wave_phase = beat(rng, frames, 6, 14)
        shared = (place * 56 * s, longest, cycles, (i * 0.382 + 0.15 * phase) % 1, rng.uniform(0, 1),
                  wave_cycles, wave_phase, rng.uniform(6, 13) * s, rng.normal(-0.2 * place, 0.25) * 70 * s)
        for _ in range(count):
            across = rng.triangular(-1, 0, 1)
            sunk = rng.uniform(0, 22)
            base = hearth + slope * max(abs(place * 56 + across * half) - 24, 0) + sunk
            rows.append(shared + (base * s, across * half * s, rng.uniform(0, 1),
                                  (1 - 0.45 * abs(across) ** 1.5) * rng.uniform(0.85, 1),
                                  heat * (0.78 + 0.15 * central) * (1 - 0.2 * across ** 2) * rng.uniform(0.92, 1.04),
                                  rng.uniform(0, 1.6), sunk < lip))
    walks = np.cumsum(rng.normal(0, 1, (len(rows), frames)) * rng.uniform(1, 2.6, (len(rows), 1)) * s, axis=1)
    walks -= np.linspace(1 / frames, 1, frames) * walks[:, -1:]
    return np.array(rows).T[..., None], walks


def flame_lines(fire, walks, frame, frames, s, boil):
    (x0, longest, cycles, phase, echo, wave_cycles, wave_phase, amplitude, lean,
     y0, offset, delta, taper, hot, depth, front) = fire
    # vertex k is gas that rose from the base k frames ago: its surge, sway and kink were set at birth and travel up with it
    step = np.arange(REACH)
    age = step + delta
    born = (frame - age) / frames
    surge = 0.6 * pulse(cycles * born + phase, 1.5) + 0.4 * pulse((cycles + 1) * born + echo, 1.5)
    whole = longest * (0.6 + 0.4 * surge)
    vital = whole * taper - age
    spent = np.clip(age / whole, 0, 1)
    spread = (spent + 0.15) ** 0.4 * (1 - spent) ** 1.3 * 2
    sway = amplitude * np.sin(math.tau * (wave_cycles * born + wave_phase))
    x = (x0 + lean * (age / longest) ** 1.5 + offset * spread * (1 + 0.35 * surge)
         + sway * (age / 8) ** 1.3 + walks[:, (frame - step) % frames] * np.minimum(age / 2.5, 1))
    y = y0 - (13 * age + 0.6 * age ** 2) * s
    heat = np.clip(vital / whole, 0, 1) ** 0.55 * hot * (1 + 0.2 * surge)
    points = np.stack([x, y], axis=2) + boil.normal(0, 0.45 * s, x.shape + (2,))

    a, b, va, vb = points[:, :-1], points[:, 1:], vital[:, :-1], vital[:, 1:]
    cut = np.clip(va / np.where(va == vb, 1, va - vb), 0, 1)
    t0, t1 = np.where(va > 0, 0, cut), np.where(vb > 0, 1, cut)
    shown = (va > 0) | (vb > 0)
    middle = heat[:, :-1] + (heat[:, 1:] - heat[:, :-1]) * (t0 + t1) / 2
    index = np.clip((middle * len(FIRE)).astype(int), 0, len(FIRE) - 1)
    lines = np.concatenate([a + (b - a) * t0[..., None], a + (b - a) * t1[..., None]], axis=2)
    layers = []
    for layer in (shown & (front == 0), shown & (front == 1)):
        order = np.argsort((index + depth)[layer])
        layers.append((lines[layer][order], index[layer][order]))
    return layers


def sparks(rng, s, frames):
    return [(rng.normal(0, 26) * s, -rng.uniform(90, 190) * s, rng.normal(0, 22) * s, rng.uniform(80, 150) * s,
             rng.uniform(4, 10) * s, rng.uniform(0, math.tau), rng.uniform(0.45, 0.9), *beat(rng, frames, 10, 22))
            for _ in range(8)]


def render(kind, seed, tile, frames):
    s = tile / 384
    rng = np.random.default_rng(seed)
    layout, hearth, slope, lip = KINDS[kind](rng)
    logs = [log(rng, s, frames, *ends) for ends in layout]
    bed = coals(rng, s, frames)
    fire, walks = flames(rng, s, frames, hearth, slope, lip)
    flecks = sparks(rng, s, frames)
    origin = np.array([tile / 2, tile - 54 * s])
    width = max(1, round(s))

    images = []
    for frame in range(frames):
        phase = math.tau * frame / frames
        boil = np.random.default_rng([seed, frame])
        image = Image.new("RGBA", (tile, tile), (0, 0, 0, 0))
        draw = ImageDraw.Draw(image)

        def stroke(verts, color):
            moved = verts + boil.normal(0, 0.45 * s, verts.shape) + origin
            draw.line([tuple(v) for v in moved], fill=color + (255,), width=width)

        def glow(embers):
            for verts, heat, flicker, cycles, shift in embers:
                level = heat + flicker * math.sin(cycles * phase + math.tau * shift)
                stroke(verts, FIRE[int(np.clip(level, 0, 0.999) * 7)])

        def pile(side):
            for bark, embers, front in logs:
                if front == side:
                    for verts, color in bark:
                        stroke(verts, color)
                    glow(embers)

        def lick(lines, index):
            for (x0, y0, x1, y1), i in zip(lines + np.tile(origin, 2), index):
                draw.line([(x0, y0), (x1, y1)], fill=FIRE[i] + (255,), width=width)

        behind, ahead = flame_lines(fire, walks, frame, frames, s, boil)
        pile(0)
        glow(bed)
        lick(*behind)
        pile(1)
        lick(*ahead)
        for x, y, drift, rise, wobble, turn, span, cycles, shift in flecks:
            u = (cycles * frame / frames + shift) % 1
            if u < span:
                t = np.array([max(u - 0.025, 0), u])
                verts = np.stack([x + drift * t + wobble * np.sin(3 * math.tau * t + turn), y - rise * t ** 0.8], axis=1)
                stroke(verts, FIRE[int((1 - u / span) ** 0.7 * 6.99)])
        images.append(bleed_colors(np.asarray(image)))
    return images


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--kind", default="cross", choices=sorted(KINDS))
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--tile", type=int, default=384)
    parser.add_argument("--frames", type=int, default=16)
    parser.add_argument("--columns", type=int, default=4)
    args = parser.parse_args()
    frames = render(args.kind, args.seed, args.tile, args.frames)
    pack(frames, args.columns).save(args.out)
    print(f"{args.out}: tile {args.tile}x{args.tile}, {args.frames} tiles, {args.columns} per row")
