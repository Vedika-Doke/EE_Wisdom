"""
ETA & Prediction Engine — Person 2's prototype.

Implements the 6 equations from Server_Side_Math_Explained.md:
  1. Haversine distance
  2. Projection / snapping (bus -> nearest segment, t*, perpendicular delta)
  3. Geofence check (with 3-consecutive-packet anti-twitch)
  4. ETA (three-piece sum: rest of current seg + later segs + stop waits)
  5. EWMA learning (with warmup)
  6. Dead reckoning (blended speed, decay weight gamma)
"""

from __future__ import annotations
from dataclasses import dataclass, field
from math import sin, cos, asin, sqrt, radians, exp
from typing import Optional

R_EARTH = 6_371_000.0  # metres


# ---------- 1. Haversine ----------

def haversine(lat1: float, lon1: float, lat2: float, lon2: float) -> float:
    """Great-circle distance between two (lat, lon) points, in metres."""
    p1, p2 = radians(lat1), radians(lat2)
    dp, dl = radians(lat2 - lat1), radians(lon2 - lon1)
    a = sin(dp / 2) ** 2 + cos(p1) * cos(p2) * sin(dl / 2) ** 2
    return 2 * R_EARTH * asin(sqrt(a))


# ---------- 2. Flat projection + snapping ----------

def to_xy(lat: float, lon: float, lat0: float, lon0: float) -> tuple[float, float]:
    """Equirectangular projection: lat/lon -> flat (x, y) metres around (lat0, lon0)."""
    x = R_EARTH * radians(lon - lon0) * cos(radians(lat0))
    y = R_EARTH * radians(lat - lat0)
    return x, y


def clip(v: float, lo: float, hi: float) -> float:
    return max(lo, min(hi, v))


def project_to_segment(
    bus_lat: float, bus_lon: float,
    p_lat: float, p_lon: float,
    q_lat: float, q_lon: float,
) -> tuple[float, float]:
    """
    Drop the bus onto segment P->Q. Returns (t_star, delta_metres).
      t_star in [0,1] = fraction along the segment of the foot of the perpendicular.
      delta = perpendicular distance from bus to the segment, in metres.
    """
    bx, by = to_xy(bus_lat, bus_lon, p_lat, p_lon)
    qx, qy = to_xy(q_lat, q_lon, p_lat, p_lon)
    seg_len_sq = qx * qx + qy * qy
    if seg_len_sq == 0.0:
        return 0.0, sqrt(bx * bx + by * by)
    t = (bx * qx + by * qy) / seg_len_sq
    t = clip(t, 0.0, 1.0)
    cx, cy = t * qx, t * qy
    dx, dy = bx - cx, by - cy
    return t, sqrt(dx * dx + dy * dy)


# ---------- Route representation ----------

@dataclass
class Stop:
    name: str
    lat: float
    lon: float


@dataclass
class Route:
    name: str
    stops: list[Stop]
    geofence_width_m: float = 50.0

    @property
    def segment_lengths(self) -> list[float]:
        """L_i = haversine length of segment i (i from 0 to N-2)."""
        return [
            haversine(self.stops[i].lat, self.stops[i].lon,
                      self.stops[i + 1].lat, self.stops[i + 1].lon)
            for i in range(len(self.stops) - 1)
        ]


# ---------- Snap bus to route ----------

@dataclass
class SnapResult:
    seg_index: int       # which segment the bus is on (k)
    t_star: float        # fraction along that segment
    s_k: float           # metres into segment k
    delta_m: float       # perpendicular distance to that segment
    on_route: bool       # delta <= W?


def snap_bus_to_route(route: Route, bus_lat: float, bus_lon: float) -> SnapResult:
    """Find the closest segment to the bus and project onto it."""
    best: Optional[SnapResult] = None
    L = route.segment_lengths
    for i in range(len(route.stops) - 1):
        p, q = route.stops[i], route.stops[i + 1]
        t, delta = project_to_segment(bus_lat, bus_lon, p.lat, p.lon, q.lat, q.lon)
        if best is None or delta < best.delta_m:
            best = SnapResult(
                seg_index=i,
                t_star=t,
                s_k=t * L[i],
                delta_m=delta,
                on_route=(delta <= route.geofence_width_m),
            )
    assert best is not None
    return best


# ---------- 5. EWMA learning (with warmup) ----------

@dataclass
class EWMA:
    """Exponential weighted moving average with warmup."""
    value: float                # current running average (seconds)
    n_samples: int = 0          # how many observations have come in
    alpha_steady: float = 0.25
    alpha_warmup: float = 0.5
    warmup_threshold: int = 10  # first N samples use warmup alpha

    def update(self, observation: float) -> float:
        a = self.alpha_warmup if self.n_samples < self.warmup_threshold else self.alpha_steady
        self.value = a * observation + (1 - a) * self.value
        self.n_samples += 1
        return self.value


# ---------- 4. ETA ----------

@dataclass
class SegmentStats:
    """Per-segment learned travel times. In real life this is per (segment, time-bucket)."""
    tau: EWMA            # mean travel time, seconds
    stop_wait: float = 0.0  # d_j: average dwell at the END stop of this segment, seconds


def compute_eta(
    route: Route,
    snap: SnapResult,
    target_stop_index: int,
    stats: list[SegmentStats],
) -> float:
    """
    ETA from current position to stops[target_stop_index], in seconds.

    Three-piece sum:
      (remaining fraction of current segment) * tau_k
      + sum of tau_i for later segments
      + sum of stop waits at intermediate stops
    """
    k = snap.seg_index
    L = route.segment_lengths
    if target_stop_index <= k:
        return 0.0

    # 1. remaining time in current segment
    frac_left = (L[k] - snap.s_k) / L[k] if L[k] > 0 else 0.0
    eta = frac_left * stats[k].tau.value

    # 2. later segment times
    for i in range(k + 1, target_stop_index):
        eta += stats[i].tau.value

    # 3. dwells at intermediate stops (not the destination)
    for j in range(k + 1, target_stop_index):
        eta += stats[j - 1].stop_wait  # dwell at stop P_j sits on the seg arriving at it
    return eta


# ---------- 6. Dead reckoning ----------

def blended_speed(v_last: float, v_historical: float, dt_seconds: float,
                  kappa_seconds: float = 300.0) -> tuple[float, float]:
    """Returns (gamma, v_hat). kappa default 5 minutes."""
    gamma = exp(-dt_seconds / kappa_seconds)
    v_hat = gamma * v_last + (1 - gamma) * v_historical
    return gamma, v_hat


def dead_reckon(
    route: Route,
    last_snap: SnapResult,
    v_last: float,
    v_historical: float,
    dt_seconds: float,
) -> tuple[Optional[SnapResult], float, float]:
    """
    Slide the bus forward along the polyline by v_hat * dt.
    Returns (new_snap_or_None_if_off_end, v_hat, confidence_0_to_1).
    If dt > 15 minutes, return (None, ...) — stop guessing.
    """
    if dt_seconds > 15 * 60:
        return None, 0.0, 0.0
    gamma, v_hat = blended_speed(v_last, v_historical, dt_seconds)
    distance = v_hat * dt_seconds

    L = route.segment_lengths
    k = last_snap.seg_index
    remaining_in_seg = L[k] - last_snap.s_k

    # walk forward segment by segment
    while distance > remaining_in_seg and k < len(L) - 1:
        distance -= remaining_in_seg
        k += 1
        remaining_in_seg = L[k]

    if distance > remaining_in_seg:
        # past the end of the route
        return None, v_hat, 0.0

    new_s_k = (L[k] - remaining_in_seg) + distance
    new_t = new_s_k / L[k] if L[k] > 0 else 0.0
    confidence = max(0.0, gamma)  # rough: confidence decays with staleness
    return (SnapResult(
        seg_index=k, t_star=new_t, s_k=new_s_k,
        delta_m=0.0,  # synthetic — we're on the polyline by construction
        on_route=True,
    ), v_hat, confidence)


# ---------- 3. Geofence with anti-twitch ----------

@dataclass
class GeofenceTracker:
    """Raises OFF_ROUTE only after `consecutive` packets fail."""
    consecutive: int = 3
    streak: int = 0

    def update(self, on_route_now: bool) -> bool:
        """Returns True if OFF_ROUTE alert should fire."""
        if on_route_now:
            self.streak = 0
            return False
        self.streak += 1
        return self.streak >= self.consecutive
