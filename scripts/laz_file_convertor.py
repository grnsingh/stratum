import laspy
import numpy as np

INPUT_FILE = "assets/points.laz"
OUTPUT_FILE = "assets/converted/terrain.txt"
TARGET_POINTS = 1000000

print(f"[1/4] Opening {INPUT_FILE}...")
try:
    las = laspy.read(INPUT_FILE)
except Exception as e:
    print(f"Error reading LAZ file: {e}")
    exit(1)

total_points = len(las.points)
print(f"[2/4] Found {total_points} points.")

# Determine Stride
stride = max(1, total_points // TARGET_POINTS)
print(f"      Subsampling: Keeping 1 out of every {stride} points.")

x = las.x[::stride]
y = las.y[::stride]
z = las.z[::stride]

print("[3/4] Centering coordinates...")
min_x, min_y, min_z = np.min(x), np.min(y), np.min(z)

x -= min_x
y -= min_y
z -= min_z

print(f"[4/4] Writing {len(x)} points to {OUTPUT_FILE}...")
with open(OUTPUT_FILE, "w") as f:
    for i in range(len(x)):
        f.write(f"{x[i]:.3f} {y[i]:.3f} {z[i]:.3f}\n")
        
        if i % 100000 == 0:
            print(f"      Written {i} lines...", end='\r')

print(f"\n[SUCCESS] Done. Saved to {OUTPUT_FILE}")
print(f"          Offset used: X={min_x:.2f}, Y={min_y:.2f}, Z={min_z:.2f}")