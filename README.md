# MufiZ

> This project uses the Zig `v0.11.0`

This project aims to integrate the Mufi-Lang compiler with the Zig language by using the 
Zig Build system. We hope to integrate more features with this language and see how nicely 
we can utilize both languages in unity. The advantage of Zig's Build system is easy cross-compatibility and caching, and as we integrate more, 
we can ensure more memory safety. 


## Usage: 

```shell
$ zig build run # for repl 
$ zig build run -- version # for version 
$ zig build run -- <path> # to run script 
```