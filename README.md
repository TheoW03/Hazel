# Hazel

hazel is a declartive functional paradigm programming langauge. its syntax and name is inpsired by haskell.

It uses a LLVM compiler, hazelc.


## build instrcutions

make sure you have CMake v3.22.1 and the LLVM version 14.0.0
if not download them here

- [cmake]
- [llvm]


now run this in your terminal

```sh
cmake -S . -B build 
cmake --build build --config Release
./hazelc --help
```

[documentation]

[documentation]: <https://docs.google.com/document/d/12JWLuCte1r-tfAHKesndeBFRdnv7tT663QnIC2gSafY/edit?tab=t.0#heading=h.85eivsfcywf4>

[cmake]: <https://cmake.org/download/>
[llvm]:<https://releases.llvm.org/download.html>