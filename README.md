# Vector3vsVector4
Tiny project to test vec3 efficiency on current hardware.

## Linux

```bash
time spend (array<vec3>)     : 4.57e-07
total space used             : 12288
time spend (array<vec4>)     : 1.607e-06
total space used             : 16384
time spend (array<vec3SoA>)  : 2.34e-07
total space used             : 12288
time spend (array<vec4SoA>)  : 2.21e-07
total space used             : 16384
time spend (vector<vec3>)    : 0.000933589
total space used             : 12582912
time spend (vector<vec4>)    : 0.00201589
total space used             : 16777216
time spend (vector<vec3SoA>) : 0.000715386
total spend used             : 12582912
time spend (vector<vec4SoA>) : 0.00110701
total spend used             : 16777216
```
Revert from what we should hope this was done under Ubuntu with clang compiler.


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
time spend (array<vec3>)     : 3.1019e-05
total space used             : 12288
time spend (array<vec4>)     : 2.5768e-05
total space used             : 16384
time spend (array<vec3SoA>)  : 4.3835e-05
total space used             : 12288
time spend (array<vec4SoA>)  : 4.833e-05
total space used             : 16384
time spend (vector<vec3>)    : 0.0424802
total space used             : 12582912
time spend (vector<vec4>)    : 0.0364368
total space used             : 16777216
time spend (vector<vec3SoA>) : 0.0482203
total spend used             : 12582912
time spend (vector<vec4SoA>) : 0.0526487
total spend used             : 16777216
```
We see a 20% increase in speed from vec3 to vec4 in static array (size 1024) and a 15% increase in speed increase in vector (1024 * 1024).
