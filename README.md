# Braintranscriber

Brainfuck and Ook! interpterer and translator.

## Description

The interpreter uses signed 8 bit cells, wrapping in both directions.

The tape is unbounded to the right and bounded to the left at position zero.

Cells remain untouched on EOF.

## Usage

```
bt [-options] [Brainfuck/Ook! code/path]
  -b	interpret Brainfuck (default)
  -o	interpret Ook!
  -i	run interpreted code (default)
  -t	translate Brainfuck to Ook and vice versa
  -f	read code from file
```

Compile the source with a C++11 compatible compiler like below:

```
clang++ src/bt.cpp -O3 -std=c++11 -o bt
```

To run one of the examples:

```
./bt -f data/hanoi.b
```


## License

This software is licensed under the zlib/libpng license.
See `LICENSE` in the project root directory for details.
