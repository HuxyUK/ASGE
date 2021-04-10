# Game Engine Development Notes
## OpenGL Rendering
### OpenGL 4.5 
OpenGL 4.5 is now the default target for ASGE. This allows a host of new 
performance functions and impacts on the renderer's design decisions. This rules 
out support for the Broadwell and earlier iGPUs. 

| GPU                                       | OpenGL    | Vulkan |
| ----------------------------------------- |:---------:| ------:|
| Intel UHD 620/630                         | [x] (4.5) | 1.1.93 |
| Intel® HD Graphics 610/615/620/630        | [x] (4.5) | 1.1.93 |
| Intel® Iris® Plus Graphics 640/650/655    | [x] (4.5) | 1.1.93 |
| Intel® HD Graphics 500/505                | [x] (4.5) | 1.1.93 |
| Intel® HD Graphics 515/520/530            | [x] (4.5) | 1.1.93 |
| Intel® Iris® Plus Graphics 540/550/580    | [x] (4.5) | 1.1.93 |
| Nvidia® 400/500/600/700/900/1000/Turing   | [x] (4.6) |        | 

### Bulk Upload Prototype
```cpp
/// loop through and attempt to populate the quads
auto quad     = range.begin;
auto distance = std::distance(range.begin, range.end);
distance      = std::min(int64_t(GLRenderConstants::QUAD_UBO_LIMIT), distance);

// dereference the quad, and copy contigous quads as well
auto gpu_quad = &static_cast<GPUQuad*>(p)[0];
memcpy(gpu_quad, &quad->gpu_data, sizeof(GPUQuad) * distance);
uploaded = distance;
```