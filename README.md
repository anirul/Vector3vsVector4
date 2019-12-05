# Vector3vsVector4
Tiny project to test that vec3 are inefficient on current hardware.
## Windows

```powershell
default alignment            : 16
time spend (array<vec3>)     : 1.8e-06
total space used             : 12288
time spend (array<vec4>)     : 1.9e-06
total space used             : 16384
time spend (array<vec3SoA>)  : 4.2e-06
total space used             : 12288
time spend (array<vec4SoA>)  : 4.4e-06
total space used             : 16384
time spend (vector<vec3>)    : 0.0019076
total space used             : 12582912
time spend (vector<vec4>)    : 0.0020377
total space used             : 16777216
time spend (vector<vec3SoA>) : 0.0044297
total spend used             : 12582912
time spend (vector<vec4SoA>) : 0.0046761
total spend used             : 16777216
```

We don't see anyting... Changes are meaningless and the improvement are non significative.

## OSX
```bash
> mkdir build
> cd build
> cmake -DCMAKE_BUILD_TYPE=Release .. -G "Unix Makefiles"
> ./test
time spend (array<vec3>)  : 2.042e-06
total space used          : 12288
time spend (array<vec4>)  : 1.213e-06
total space used          : 16384
time spend (vector<vec3>) : 0.00209635
total space used          : 12582912
time spend (vector<vec4>) : 0.00175815
total space used          : 16777216
```
We see a 20% increase in speed from vec3 to vec4 in static array (size 1024) and a 15% increase in speed increase in vector (1024 * 1024).
