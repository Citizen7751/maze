# Maze

Maze generation with the **Recursive Division** algorithm.

This is a "wall-adder" rather than a "path-carver". It's a simple algorithm: adds a wall, makes a hole in it and recursively divides the area on both sides of that wall, adding more wall and dividing more, until the area is too small.

[Visualisation](https://www.youtube.com/watch?v=1GENvb4y95s)

---
### Compile with GCC

Windows:
```
gcc -o maze.exe maze.c -s -O3
```

Linux:
```
gcc -o maze maze.c -s -O3
```
---
This software is under the MIT License.