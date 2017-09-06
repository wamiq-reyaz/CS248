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

and copy the `mesh.o` file to your favourite library storage.

## Testing

Just run

```bash
make && ./main 
```

This will run all tests by default.
