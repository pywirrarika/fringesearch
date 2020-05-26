# fringesearch

This is an implementation of the Fringe Search Algorithm (Björnsson, et. al. 2005) in C. The aim of this algorithm is to find the shortest path between two points in a map. This implementation only handles the case of a 2D lattice map, and reads it from a black and white bmp file. 

## Usage

The command line instructions are as follows:

`$ fs X Y X2 Y2 map.bmp output.bmp MAX_PROC DEBUG`

where (X, Y) is the starting point and (X2, Y2) the goal point. The bmp files should be 24bit images without compression. 


## Compilation

A simple compilation should be enough:

```
$ autoreconfig --install
$ ./configure
$ make
```

## Citation

If you found this implementation useful and used it in scientific research you can cite it as follows:
```
@misc{manuel2015algoritmo,
  title={El algoritmo fringe search como soluci{\'o}n superior aa* en la b{\'u}squeda de caminos sobre gr{\'a}ficos de malla},
  author={Manuel, Mager Hois Jes{\'u}s},
  year={2015},
  publisher={May}
}
```
## References

Björnsson, Y., Enzenberger, M., Holte, R. C., & Schaeffer, J. (2005). Fringe Search: Beating A* at Pathfinding on Game Maps. CIG, 5, 125-132.
