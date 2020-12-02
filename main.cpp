/*
Contact: Manish Saroya (saroyam@oregonstate.edu)

Computes convex hull of a set of points.

References: Wiki Graham scan : https://en.wikipedia.org/wiki/Graham_scan
*/

#include <algorithm>
#include <iostream>
#include <stack>

struct Point {
  int x, y;
};

struct Compare {
  Point p1;
  Compare(Point p) {
    p1.x = p.x;
    p1.y = p.y;
  }
  bool operator()(const Point &p2, const Point &p3) {
    // order p2 and p3 based on polar angles
    int cross_product =
        (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);

    if (cross_product == 0) {
      // consider distances to order p2 and p3 when polar angles are same
      int dist2 = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
      int dist3 = (p3.x - p1.x) * (p3.x - p1.x) + (p3.y - p1.y) * (p3.y - p1.y);
      return dist2 < dist3;
    }
    return cross_product > 0;
  }
};

int get_direction(const Point &p1, const Point &p2, const Point &p3) {
  //   returns
  //   greater than zero : counter clock-wise direction
  //   less than zero : clock-wise direction
  //   equal to zero : collinear
  return (p2.x - p1.x) * (p3.y - p1.y) -
         (p2.y - p1.y) * (p3.x - p1.x); // using cross product
}

Point get_next_to_top(std::stack<Point> &local_stack) {
  Point top = local_stack.top();
  local_stack.pop();
  Point next_to_top = local_stack.top();
  local_stack.push(top);
  return next_to_top;
}

void convex_hull(Point points[], int n) {
  // find the bottom left point
  int bottom_left_index = 0;
  for (int i{0}; i < n; i++) {
    if (points[i].y < points[bottom_left_index].y) {
      bottom_left_index = i;
    } else if (points[i].y == points[bottom_left_index].y &&
               points[i].x < points[bottom_left_index].x) {
      bottom_left_index = i;
    }
  }
  // make the bottom left points as first
  Point temp = points[bottom_left_index];
  points[bottom_left_index] = points[0];
  points[0] = temp;

  // sort the points
  Compare comp{points[0]};
  std::sort(points + 1, points + n, comp);

  // remove clockwise turns
  std::stack<Point> stk;
  for (int i{0}; i < n; i++) {
    while (stk.size() > 1 and
           get_direction(get_next_to_top(stk), stk.top(), points[i]) <= 0) {
      stk.pop();
    }
    stk.push(points[i]);
  }

  // print
  std::cout << "Convex Hull" << std::endl;
  while (!stk.empty()) {
    std::cout << " x: " << stk.top().x << " y: " << stk.top().y << std::endl;
    stk.pop();
  }
}

int main() {
  Point points[] = {{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0},
                    {1, 2}, {4, 1}, {3, 3}, {0, 2}, {4, 2}};
  int n = sizeof(points) / sizeof(points[0]);
  convex_hull(points, n);
  return 0;
}

/*
Output:

Convex Hull
 x: 0 y: 3
 x: 4 y: 4
 x: 4 y: 1
 x: 0 y: 0

*/