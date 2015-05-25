#Braintranscriber
Brainfuck and Ook! interpterer and translator.
##Usage

```
bt [-options] [Brainfuck/Ook code]
  -b	interpret Brainfuck (default)
  -o	interpret Ook!
  -i	run interpreted code (default)
  -t	translate Brainfuck to Ook and vice versa
```
Compile the source with a C++11 compatible compiler like below:

```
clang++ src/bt.cpp -std=c++11 -o bt
```

##License
This software is licensed under the zlib/libpng license.
See `LICENSE` in the project root directory for details.
