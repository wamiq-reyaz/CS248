# Project 1-2

This is an implementation of the Half-Edge data structure.
The code provides a simple interface to load `.obj` files and construct a mesh.

## Using

Include `mesh.h` and link against `mesh.o`. Done.

## Compiling

Simply grab `mesh.c` for the function definitions. To link against `mesh.o`, run

```bash
make mesh
```

and move the `mesh.o` file to your library path.

To see a dump of the meshes on `stdout`, run

```bash
make
```

and then run `./main <model_name>`

## Testing

Just run

```bash
make && ./test 
```

This will run all tests by default. To find the list of available tests, run

```bash
./test -l
```

and test a particular module.
