import argparse
import colorsys
import math

import numpy as np
from PIL import Image, ImageDraw

PALETTES = {
    "green": {
        "bark": [(16, 13, 12), (36, 28, 23), (56, 43, 33)],
        "leaf": [(10, 16, 13), (20, 44, 30), (36, 78, 46), (62, 112, 60), (98, 146, 82)],
    },
    "teal": {
        "bark": [(14, 13, 14), (30, 28, 30), (48, 44, 44)],
        "leaf": [(10, 20, 20), (20, 58, 56), (34, 96, 88), (62, 138, 122), (104, 176, 152)],
    },
    "autumn": {
        "bark": [(16, 12, 10), (38, 27, 20), (58, 42, 30)],
        "leaf": [(26, 16, 10), (88, 46, 18), (140, 82, 26), (184, 126, 40), (214, 170, 72)],
    },
    "bone": {
        "bark": [(12, 12, 13), (26, 26, 29), (42, 42, 47)],
        "leaf": [(30, 32, 37), (72, 76, 84), (120, 124, 132), (168, 172, 178), (212, 214, 218)],
    },
    "ink": {
        "bark": [(5, 5, 6), (10, 10, 13), (17, 17, 22)],
        "leaf": [(4, 4, 6), (10, 10, 14), (18, 18, 25), (29, 29, 39), (46, 46, 60)],
    },
}
HUES = {"red": 2, "blue": 218, "violet": 275}


def hue_palette(hue):
    toward_yellow = ((60 - hue + 180) % 360) - 180
    shift = math.copysign(12, toward_yellow) if abs(toward_yellow) > 1 else 0

    def color(h, saturation, value):
        return tuple(round(c * 255) for c in colorsys.hsv_to_rgb((h % 360) / 360, saturation, value))

    leaf = [color(hue + shift * (i - 2) / 2, saturation, value)
            for i, (saturation, value) in enumerate([(0.5, 0.08), (0.75, 0.2), (0.8, 0.36), (0.7, 0.52), (0.5, 0.68)])]
    bark = [color(hue, 0.3, value) for value in (0.06, 0.12, 0.19)]
    return {"bark": bark, "leaf": leaf}


def palette_by_name(name):
    if name in PALETTES:
        return PALETTES[name]
    if name in HUES:
        return hue_palette(HUES[name])
    return hue_palette(float(name))


def crown_blobs(rng, s):
    cx, cy = rng.normal(0, 8 * s), -302 * s
    rx, ry = 132 * s, 98 * s
    blobs = [(cx, cy, rx, ry, 0.25)]
    start = rng.uniform(0, math.tau)
    count = rng.integers(5, 8)
    for i in range(count):
        a = start + math.tau * i / count + rng.normal(0, 0.25)
        blobs.append((cx + math.cos(a) * rx * 0.72, cy + math.sin(a) * ry * 0.62,
                      rx * rng.uniform(0.42, 0.6), ry * rng.uniform(0.48, 0.66), rng.uniform(0.3, 0.95)))
    return blobs


def sample_attractors(rng, blobs, count, s):
    lo_x, hi_x, lo_y, hi_y = -212 * s, 212 * s, -448 * s, -178 * s
    points = []
    while len(points) < count:
        x, y = rng.uniform(lo_x, hi_x), rng.uniform(lo_y, hi_y)
        for bx, by, rx, ry, _ in blobs:
            if ((x - bx) / rx) ** 2 + ((y - by) / ry) ** 2 < 1:
                points.append((x, y))
                break
    return np.array(points)


def grow_skeleton(rng, s):
    step, influence, kill = 8 * s, 70 * s, 13 * s
    blobs = crown_blobs(rng, s)
    attractors = sample_attractors(rng, blobs, 700, s)
    crown = attractors.copy()

    lean = rng.normal(0, 14 * s)
    wobble, wobble_phase = rng.uniform(2, 6) * s, rng.uniform(0, math.tau)
    trunk_top = 150 * s
    count = int(trunk_top / step) + 1
    nodes, parents = [], []
    for i in range(count):
        t = i / (count - 1)
        nodes.append((lean * t * t + wobble * math.sin(t * 5 + wobble_phase) * t, -trunk_top * t))
        parents.append(i - 1)

    for _ in range(400):
        if len(attractors) == 0:
            break
        pos = np.array(nodes)
        delta = attractors[:, None, :] - pos[None, :, :]
        dist = np.hypot(delta[..., 0], delta[..., 1])
        nearest = dist.argmin(axis=1)
        rows = np.arange(len(attractors))
        nearest_dist = dist[rows, nearest]
        active = nearest_dist < influence
        if not active.any():
            break
        pull = np.zeros_like(pos)
        np.add.at(pull, nearest[active], delta[rows, nearest][active] / nearest_dist[active, None])
        grown = False
        for i in np.nonzero((pull ** 2).sum(axis=1) > 1e-9)[0]:
            direction = pull[i] / np.linalg.norm(pull[i]) + rng.normal(0, 0.12, 2)
            direction /= np.linalg.norm(direction)
            candidate = pos[i] + direction * step
            every = np.array(nodes)
            if np.hypot(every[:, 0] - candidate[0], every[:, 1] - candidate[1]).min() < step * 0.5:
                continue
            nodes.append((candidate[0], candidate[1]))
            parents.append(int(i))
            grown = True
        if not grown:
            break
        pos = np.array(nodes)
        delta = attractors[:, None, :] - pos[None, :, :]
        attractors = attractors[np.hypot(delta[..., 0], delta[..., 1]).min(axis=1) > kill]

    return np.array(nodes), np.array(parents), crown, blobs


def describe_skeleton(pos, parents, s):
    count = len(pos)
    children = np.zeros(count, dtype=int)
    for p in parents[1:]:
        children[p] += 1
    tips = (children == 0).astype(float)
    for i in range(count - 1, 0, -1):
        tips[parents[i]] += tips[i]

    radius = np.maximum(0.8 * s, 12 * s * np.sqrt(tips / tips[0]))
    radius *= 1 + 0.8 * np.exp(pos[:, 1] / (22 * s))

    tangent = np.zeros_like(pos)
    tangent[1:] = pos[1:] - pos[parents[1:]]
    tangent[0] = tangent[1]
    smooth = tangent.copy()
    smooth[1:] += tangent[parents[1:]]
    smooth /= np.linalg.norm(smooth, axis=1, keepdims=True)
    normal = np.stack([-smooth[:, 1], smooth[:, 0]], axis=1)
    return children, tips, radius, normal


def wood_strands(rng, pos, parents, children, radius, normal, palette):
    strands = []
    tip_nodes = np.nonzero(children == 0)[0]
    for tip in np.repeat(tip_nodes, max(1, round(110 / len(tip_nodes)))):
        path = []
        i = int(tip)
        while i >= 0:
            path.append(i)
            i = int(parents[i])
        path = np.array(path[::-1])
        offset = rng.uniform(-1, 1)
        verts = pos[path] + normal[path] * (offset * radius[path])[:, None]
        color = palette["bark"][rng.choice(3, p=[0.45, 0.35, 0.2])]
        strands.append((verts, color))
    return strands


def leaf_tufts(rng, pos, children, crown, blobs, s, palette):
    tips = pos[(children == 0) & (pos[:, 1] < -185 * s)]
    centers = np.concatenate([tips, crown[rng.permutation(len(crown))[:380]]])
    centers = centers + rng.normal(0, 4 * s, centers.shape)

    crown_center = centers.mean(axis=0)
    segments = 6
    tufts = []
    for center in centers:
        bx, by, rx, ry, blob_depth = min(blobs, key=lambda b: ((center[0] - b[0]) / b[2]) ** 2 + ((center[1] - b[1]) / b[3]) ** 2)
        depth = float(np.clip(blob_depth + rng.normal(0, 0.16), 0, 1))
        height = float(np.clip(0.5 - (center[1] - by) / (2 * ry), 0, 1))
        outward = math.atan2(center[1] - crown_center[1], center[0] - crown_center[0])
        cycles = rng.choice([1, 2, 3], p=[0.3, 0.45, 0.25])
        phase = rng.uniform(0, math.tau)
        strands = []
        reach = 1.3 if depth < 0.4 else 1.0
        for _ in range(rng.integers(14, 21)):
            p = center + rng.normal(0, 6 * s, 2)
            angle = outward + rng.normal(0, 1.1)
            direction = np.array([math.cos(angle), math.sin(angle)])
            hop = rng.uniform(14, 36) * s * reach / segments
            verts = [p]
            for _ in range(segments):
                direction = direction + np.array([0.0, 0.24])
                direction /= np.linalg.norm(direction)
                turn = rng.normal(0, 0.22)
                c, sn = math.cos(turn), math.sin(turn)
                direction = np.array([direction[0] * c - direction[1] * sn, direction[0] * sn + direction[1] * c])
                p = p + direction * hop
                verts.append(p)
            shade = np.clip(height * (0.35 + 0.65 * depth) + rng.normal(0, 0.1), 0, 1) ** 1.2
            index = int(np.clip(shade * 5, 0, 4))
            if rng.uniform() < 0.06:
                index = 0
            strands.append((np.array(verts), palette["leaf"][index],
                            rng.uniform(1.2, 3.4) * s, phase + rng.normal(0, 0.5)))
        tufts.append((depth, cycles, strands))
    tufts.sort(key=lambda t: t[0])
    return tufts


def wind(verts, phase, s):
    x, y = verts[:, 0], verts[:, 1]
    weight = np.clip(-y / (440 * s), 0, 1) ** 1.7
    dx = (7.0 * np.sin(phase - 0.006 * x / s + 0.004 * y / s) + 2.2 * np.sin(2 * phase + 1.3 + 0.011 * x / s)) * weight
    dy = 1.2 * np.sin(phase + 0.7 - 0.006 * x / s) * weight
    return np.stack([dx, dy], axis=1) * s


def bleed_colors(pixels):
    opaque = pixels[..., 3] > 0
    rgb = pixels[..., :3].astype(np.float32)
    fallback = rgb[opaque].mean(axis=0) if opaque.any() else np.zeros(3, np.float32)
    known = opaque.copy()
    for _ in range(2):
        total = np.zeros_like(rgb)
        weight = np.zeros(known.shape, np.float32)
        for dy in (-1, 0, 1):
            for dx in (-1, 0, 1):
                if dx == 0 and dy == 0:
                    continue
                shifted = np.roll(known, (dy, dx), axis=(0, 1))
                total += np.roll(rgb, (dy, dx), axis=(0, 1)) * shifted[..., None]
                weight += shifted
        fill = ~known & (weight > 0)
        rgb[fill] = total[fill] / weight[fill][:, None]
        known |= fill
    rgb[~known] = fallback
    out = pixels.copy()
    out[..., :3] = np.clip(rgb + 0.5, 0, 255).astype(np.uint8)
    return out


def render(seed, tile, frames, palette_name):
    s = tile / 512
    rng = np.random.default_rng(seed)
    palette = palette_by_name(palette_name)
    pos, parents, crown, blobs = grow_skeleton(rng, s)
    children, tips, radius, normal = describe_skeleton(pos, parents, s)
    wood = wood_strands(rng, pos, parents, children, radius, normal, palette)
    tufts = leaf_tufts(rng, pos, children, crown, blobs, s, palette)
    back = sum(1 for t in tufts if t[0] < 0.4)
    origin = np.array([tile / 2, tile - 14 * s])
    width = max(1, round(s))

    images = []
    for frame in range(frames):
        phase = math.tau * frame / frames
        boil = np.random.default_rng([seed, frame])
        image = Image.new("RGBA", (tile, tile), (0, 0, 0, 0))
        draw = ImageDraw.Draw(image)

        def draw_tufts(chunk):
            for _, cycles, strands in chunk:
                for verts, color, flutter, flutter_phase in strands:
                    t = (np.arange(len(verts)) / (len(verts) - 1)) ** 1.5 * flutter
                    wiggle = np.stack([t * math.sin(cycles * phase + flutter_phase),
                                       0.6 * t * math.sin(cycles * phase + flutter_phase + 1.1)], axis=1)
                    moved = verts + wind(verts, phase, s) + wiggle + boil.normal(0, 0.45 * s, verts.shape) + origin
                    draw.line([tuple(v) for v in moved], fill=color + (255,), width=width)

        draw_tufts(tufts[:back])
        for verts, color in wood:
            moved = verts + wind(verts, phase, s) + boil.normal(0, 0.35 * s, verts.shape) + origin
            draw.line([tuple(v) for v in moved], fill=color + (255,), width=width)
        draw_tufts(tufts[back:])
        images.append(bleed_colors(np.asarray(image)))

    print(f"seed {seed}: nodes {len(pos)}, tips {int(tips[0])}, tufts {len(tufts)}, "
          f"wood strands {len(wood)}, leaf strands {sum(len(t[2]) for t in tufts)}")
    return images


def pack(images, columns):
    tile = images[0].shape[0]
    rows = math.ceil(len(images) / columns)
    sheet = np.zeros((rows * tile, columns * tile, 4), np.uint8)
    sheet[..., :3] = images[0][0, 0, :3]
    for i, image in enumerate(images):
        y, x = (i // columns) * tile, (i % columns) * tile
        sheet[y:y + tile, x:x + tile] = image
    return Image.fromarray(sheet, "RGBA")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("out")
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--tile", type=int, default=512)
    parser.add_argument("--frames", type=int, default=16)
    parser.add_argument("--columns", type=int, default=4)
    parser.add_argument("--palette", default="green",
                        help="one of " + ", ".join(sorted(PALETTES) + sorted(HUES)) + ", or a hue in degrees (0-360)")
    args = parser.parse_args()
    frames = render(args.seed, args.tile, args.frames, args.palette)
    pack(frames, args.columns).save(args.out)
    print(f"{args.out}: tile {args.tile}x{args.tile}, {args.frames} tiles, {args.columns} per row")
