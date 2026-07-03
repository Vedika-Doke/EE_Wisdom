---
title: "Finding the Bus's Distance from its Route"
subtitle: "The 'fast formula' explained, with worked numbers"
geometry: margin=1in
fontsize: 11pt
mainfont: "Helvetica"
monofont: "Menlo"
---

The route is a line on the map; the bus is somewhere off it. We want the perpendicular distance — the shortest gap between the bus and the road.

## Step 1 — Pretend the Earth is flat

Over a few kilometres, Earth's curvature is invisible. So we convert each latitude/longitude into plain $(x, y)$ metres, measured from a reference point near the route:

$$
x = R \cdot (\lambda - \lambda_0) \cdot \cos\varphi_0 \qquad \text{(metres east of reference)}
$$

$$
y = R \cdot (\varphi - \varphi_0) \qquad \text{(metres north of reference)}
$$

$R$ = Earth's radius, 6,371,000 m. The $\cos\varphi_0$ corrects for the fact that a degree of longitude is shorter away from the equator. After this conversion, every point is just $(x, y)$ in metres and we can use regular geometry.

## Step 2 — Example

Reference point = Stop P₀ (so P₀ itself sits at the origin).

| Point | x (m) | y (m) | What it means |
|---|---|---|---|
| P₀ | 0 | 0 | The reference itself |
| P₁ | −1192 | +1133 | 1192 m west, 1133 m north of P₀ |
| Bus 17 | −613 | +556 | 613 m west, 556 m north of P₀ |

Distance from P₀ to P₁: $\sqrt{1192^2 + 1133^2} \approx 1645$ m.

## Step 3 — Find the closest point on the segment to the bus

From P₀, draw two arrows:

- **Arrow S** = P₀ to P₁ (along the road) = (−1192, +1133)
- **Arrow V** = P₀ to bus = (−613, +556)

The fraction of the way along the segment that gets you closest to the bus is:

$$
t^\star = \frac{\vec V \cdot \vec S}{\|\vec S\|^2}
$$

where $\vec V \cdot \vec S$ is the dot product (multiply matching components, then add) and $\|\vec S\|^2$ is the segment length squared. **In plain English: how much of arrow V points the same direction as arrow S.**

Numbers:

- $\vec V \cdot \vec S = (-613)(-1192) + (556)(1133) = 730\,696 + 629\,948 = 1\,360\,644$
- $\|\vec S\|^2 = 1192^2 + 1133^2 = 2\,704\,553$
- $t^\star = 1\,360\,644 \,/\, 2\,704\,553 \approx \mathbf{0.503}$

So the bus is **50.3% of the way along the segment** — basically at the midpoint.

The `clip(…, 0, 1)` step in the original formula just snaps to an endpoint if the bus is past either end of the segment. With $t^\star = 0.503$ we're already inside [0, 1], so no clipping happens.

## Step 4 — Where is that closest point, and how far is the bus from it?

$$
\text{closest point} = P_0 + 0.503 \times (P_1 - P_0) = (-600,\; +570)
$$

$$
\text{offset} = (-613 - (-600),\; 556 - 570) = (-13,\; -14)
$$

$$
\delta = \sqrt{13^2 + 14^2} = \sqrt{365} \approx \mathbf{19\,m}
$$

## Step 5 — Verdict

Bus 17 is **19 m off the centre line** of its route. Our corridor is 50 m wide.

> 19 ≤ 50, so the bus is on route. Carry on.

## The picture

```
   P₀ --------------- midpoint --------------- P₁
                         |
                         |  19 m  (perpendicular drop)
                         |
                      Bus 17
```

The dot-product trick slides the bus's position sideways onto the road, then measures the leftover gap. That gap is $\delta$. If $\delta \le 50$ m the bus is on route; if it exceeds 50 m for **three consecutive packets**, we raise an `OFF_ROUTE` alert (and the divert-alert pipeline fires).
