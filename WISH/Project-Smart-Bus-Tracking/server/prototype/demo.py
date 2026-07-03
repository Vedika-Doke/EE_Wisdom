"""
Demo: Bus 17 on Route 5 (Samayapuram -> Tollgate -> Trichy Bus Stand).

Three scenarios:
  A. Normal trip — packets arrive on time, ETA narrows, EWMA learns.
  B. Network blackout — packets go missing for 6 minutes; dead reckoning
     keeps the ETA flowing with a confidence drop.
  C. Divert — bus drifts off the corridor; geofence fires OFF_ROUTE.
"""

from __future__ import annotations
from eta_engine import (
    Route, Stop, EWMA, SegmentStats,
    snap_bus_to_route, compute_eta, dead_reckon, blended_speed,
    haversine, GeofenceTracker,
)

# ---------- Route 5 ----------

ROUTE_5 = Route(
    name="ROUTE_05",
    stops=[
        Stop("Samayapuram",      11.0510, 78.6940),
        Stop("Tollgate",         11.0612, 78.6831),
        Stop("Trichy Bus Stand", 11.0729, 78.6748),
    ],
    geofence_width_m=50.0,
)

# Seed segment_stats from a published schedule (cold-start, Section 8.2):
#   Samayapuram 14:00 -> Tollgate 14:04 -> Trichy 14:10  (4 min + 6 min)
STATS = [
    SegmentStats(tau=EWMA(value=240.0), stop_wait=30.0),  # P0->P1, dwell at P1
    SegmentStats(tau=EWMA(value=360.0), stop_wait=0.0),   # P1->P2, no dwell after dest
]


# ---------- Helpers ----------

def hr(ch="-"):
    print(ch * 64)


def show(packet_label: str, lat: float, lon: float, speed_mps: float):
    snap = snap_bus_to_route(ROUTE_5, lat, lon)
    eta_seconds = compute_eta(ROUTE_5, snap, target_stop_index=2, stats=STATS)
    status = "LIVE" if snap.on_route else "OFF_ROUTE_CANDIDATE"
    print(f"[{packet_label}] lat={lat:.4f} lon={lon:.4f}  v={speed_mps:.2f} m/s")
    print(f"   seg={snap.seg_index}  t*={snap.t_star:.3f}  s_k={snap.s_k:.0f}m  "
          f"delta={snap.delta_m:.1f}m  status={status}")
    print(f"   ETA to {ROUTE_5.stops[2].name}: {eta_seconds:.0f} s  "
          f"(~{eta_seconds/60:.1f} min)")
    return snap


# ---------- Scenario A — Normal trip ----------

def scenario_a():
    hr("=")
    print("SCENARIO A — normal trip")
    hr("=")
    # Three packets walking along segment P0->P1, then crossing into P1->P2.
    show("14:23:11  pkt#1", 11.0560, 78.6884, 8.5)
    show("14:25:30  pkt#2", 11.0590, 78.6855, 8.7)
    show("14:27:00  pkt#3", 11.0612, 78.6831, 7.9)  # arrives at P1

    # When the bus crosses a stop, we observe a real segment travel time
    # and feed it to EWMA. Suppose segment 0 actually took 260 s today.
    T_obs = 260.0
    old = STATS[0].tau.value
    new = STATS[0].tau.update(T_obs)
    print()
    print(f"   [LEARN] segment 0 actual time = {T_obs:.0f}s, "
          f"tau {old:.1f} -> {new:.1f}  (alpha={STATS[0].tau.alpha_warmup if STATS[0].tau.n_samples<=STATS[0].tau.warmup_threshold else STATS[0].tau.alpha_steady})")

    show("14:28:00  pkt#4", 11.0650, 78.6800, 8.0)  # into segment 1
    print()


# ---------- Scenario B — Network blackout ----------

def scenario_b():
    hr("=")
    print("SCENARIO B — network blackout, dead reckoning")
    hr("=")
    last_snap = show("14:23:11  pkt#1 (last live)", 11.0560, 78.6884, 8.5)
    v_last = 8.5

    # Historical speed for this segment: L0 / tau0
    L0 = ROUTE_5.segment_lengths[0]
    v_hist = L0 / STATS[0].tau.value
    print(f"   v_last={v_last:.2f}  v_hist={v_hist:.2f} m/s  L0={L0:.0f}m")

    print()
    print("   ... no packets for 3 minutes ...")
    dt = 180.0
    gamma, v_hat = blended_speed(v_last, v_hist, dt)
    est_snap, v_hat2, conf = dead_reckon(ROUTE_5, last_snap, v_last, v_hist, dt)
    print(f"   gamma={gamma:.2f}  v_hat={v_hat:.2f} m/s  d_hat={v_hat*dt:.0f}m")
    if est_snap is None:
        print("   STATUS=LAST_KNOWN (silent too long, or off-end of route)")
        return
    eta = compute_eta(ROUTE_5, est_snap, target_stop_index=2, stats=STATS)
    print(f"   STATUS=ESTIMATED  seg={est_snap.seg_index}  t*={est_snap.t_star:.3f}  "
          f"s_k={est_snap.s_k:.0f}m  confidence={conf:.2f}")
    print(f"   ETA to Trichy ~{eta/60:.1f} min (estimated)")
    print()


# ---------- Scenario C — Divert / geofence ----------

def scenario_c():
    hr("=")
    print("SCENARIO C — bus drifts off route")
    hr("=")
    tracker = GeofenceTracker(consecutive=3)
    # Three consecutive packets ~200m perpendicular to the route.
    drifting = [
        ("14:23:11 pkt#1", 11.0560, 78.6884),  # on route, 19m delta
        ("14:25:00 pkt#2", 11.0560, 78.6920),  # drifting east
        ("14:26:30 pkt#3", 11.0560, 78.6960),  # further
        ("14:28:00 pkt#4", 11.0560, 78.7000),  # well off
    ]
    for label, lat, lon in drifting:
        snap = show(label, lat, lon, 7.0)
        alert = tracker.update(snap.on_route)
        print(f"   geofence streak={tracker.streak}  alert_fires={alert}")
        if alert:
            print("   >>> OFF_ROUTE alert raised — divert pipeline kicks in")
            print("       (DB write, dashboard banner, SMS/IVR to recent queriers)")
            break
    print()


# ---------- Sanity-check against the writeup ----------

def sanity_check():
    hr("=")
    print("SANITY CHECK — numbers from Server_Side_Math_Explained.md")
    hr("=")

    # Haversine: Bus(11.0560,78.6884) -> P0(11.0510,78.6940) should be ~826 m
    d = haversine(11.0560, 78.6884, 11.0510, 78.6940)
    print(f"   haversine(Bus, P0) = {d:.1f} m   (writeup says ~826 m)")

    # Projection onto P0->P1: t* ~ 0.503, delta ~ 19 m
    snap = snap_bus_to_route(ROUTE_5, 11.0560, 78.6884)
    print(f"   snap: seg={snap.seg_index}  t*={snap.t_star:.3f}  delta={snap.delta_m:.1f}m   "
          f"(writeup: t*=0.503, delta=19m)")

    # ETA: 0.5*240 + 30 + 360 = 510 s
    eta = compute_eta(ROUTE_5, snap, target_stop_index=2, stats=STATS)
    print(f"   ETA to Trichy = {eta:.0f} s   (writeup: 510 s)")

    # EWMA: 0.25 * 260 + 0.75 * 240 -> warmup will use 0.5, but the writeup
    # example uses the steady alpha; force steady to reproduce.
    e = EWMA(value=240.0, n_samples=99)  # past warmup
    new = e.update(260.0)
    print(f"   EWMA(240, 260, alpha=0.25) = {new:.1f} s   (writeup: 245 s)")

    # Dead reckon blended speed: v_last=8.5, v_hist=6.85, dt=360, kappa=300
    # gamma = e^-1.2 ~ 0.30, v_hat ~ 7.35
    gamma, v_hat = blended_speed(8.5, 6.85, 360.0)
    print(f"   blended_speed(8.5, 6.85, 6min) = gamma={gamma:.2f}, v_hat={v_hat:.2f}   "
          f"(writeup: gamma=0.30, v_hat=7.35)")
    print()


if __name__ == "__main__":
    sanity_check()
    scenario_a()
    scenario_b()
    scenario_c()
