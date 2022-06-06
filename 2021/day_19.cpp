/*
 * AOC
 */

#include <stdio.h>

#include <vector>
#include <array>

union V4i {
  struct {
    int x, y, z, w;
  };

  int e[4];
};

union m4x4i {
  int e[4*4];
  struct {
    V4i rows[4];
  };
};

static int doti(V4i a, V4i b) {
  return a.x * b.x + 
         a.y * b.y +
         a.z * b.z +
         a.w * b.w;
}

struct Foo {
  int a, b;
  float c;
};

struct Scanner {
  std::vector<V4i> beacons;
};

static V4i v4i_mul_m4x4i(V4i p, m4x4i mat) {
  V4i result = { };
  result.x = doti(p, mat.rows[0]);
  result.y = doti(p, mat.rows[1]);
  result.z = doti(p, mat.rows[2]);
  result.w = doti(p, mat.rows[3]);

  return result;
}

int main(void) {
  std::vector<Scanner> scanners;

  {
    Scanner s;
    s.beacons.push_back({ 0, 2 });
    s.beacons.push_back({ 4, 1 });
    s.beacons.push_back({ 3, 3 });

    scanners.push_back(s);
  }

  {
    Scanner s;
    s.beacons.push_back({ -1, -1 });
    s.beacons.push_back({ -5,  0 });
    s.beacons.push_back({ -2,  1 });

    scanners.push_back(s);
  }

  for (int i = 0; i < scanners.size(); i++) {
    Scanner *s1 = &scanners[i];
    for (int j = 0; j < scanners.size(); j++) {
      if (i == j) { continue; } 
      Scanner *s2 = &scanners[j];
      int radius = 5;
      for (int y = radius; y >= -radius; y--) {
        for (int x = radius; x >= -radius; x--) {
          V4i s2_pos = { x, y };
          int no_overlap = 0;
          for (V4i pos : s1->beacons) {
            // We need all 24 rotation matrices
            // X [ 1  0  0] -> x,  x,  x,  x,
            // Y [ 0  1  0] -> y, -y,  y, -z,
            // Z [ 0  0  1] -> z,  z, -z,  y,
            //
            // X [ 1  0  0]
            // Y [ 0 -1  0]
            // Z [ 0  0  1]
            //
            // X [ 1  0  0]
            // Y [ 0  0 -1]
            // Z [ 0  1  0]

            m4x4i rot = {
              -1, 0, 0, 0,
               0, 1, 0, 0,
               0, 0, 1, 0,
               0, 0, 0, 1,
            };

            V4i rel_pos = { pos.x - s2_pos.x, pos.y - s2_pos.y };
            V4i rot_rel_pos = v4i_mul_m4x4i(rel_pos, rot);
            for (V4i p : s2->beacons) {
              if (p.x == rel_pos.x && p.y == rel_pos.y) {
                printf("Overlapping { %d, %d }\n", p.x, p.y);
                no_overlap++;
                break;
              }
            }
          }
          if (no_overlap) {
            printf("Number of overlaps: %d\n", no_overlap);
          }
        }
      }
    }
  }
  
  return 0;
}
