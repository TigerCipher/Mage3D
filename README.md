# Mage3D
Mage3D started off as an OpenGL game engine, however as I've been teaching myself DirectX, I've grown fond of it. In fact, especially since its a game engine, I don't really see a huge need to target non-Windows platforms so why not utilize DirectX and the debugging tools provided for it?

Therefore, as of October 2nd, 2020, this engine is now officially Mage3DX - a DirectX Game Engine.
I may at some point add OpenGL back in as an optional rendering API (much like how many games these days let you pick between DirectX and Vulkan)

--------------

## Features:
1. Nothing "planned" as I'm just adding stuff as I feel like it
2. Shader system
   - Might implement a shader factory so a cso file doesn't need to be loaded each time that shader is reused by a different entity
   - Currently supports a basic Vertex and Pixel shader system
3. Input management
   - Currently maintained by the Display architecture. Allows for my private features of Windows functionality, but eventually I will probably extract it outside of there, same with the graphics system
4. ImGui
   - Mainly for debugging purposes
   - If I do end up making this a game editor rather than engine I will likely create my own UI system
5. Basic texture mapping
   - Complete for the most part, just want to support more types, such as dds, tga, etc
   - Diffuse, Specular, and Normal mapping currently implemented (as of 01/26/2021)
6. Model loading / rendering
7. Basic font loading / text rendering
8. First person camera
   - Might also support the creation of fixed camera games (i.e a side scroller using 3D models)
9. To be continued...


-------------------

#### Contributing
This section is intentionally left mostly blank for now as the project is not to the point of public usage, so I don't expect anyone to happen along and want to contribute, but if you do happen to come along and feel like contributing just know that I use VCPKG for my libraries where possible. Easiest way for you to get the project running for yourself would be to do the same, and insure you do the "integrate install" for vcpkg. `BMD` is my own C utilities library and is not officially released, hence not available in vcpkg as of this moment. The compiled static BMD library supplied by this project was built by the VS 2019 compiler. If for some reason you need an older compiled version, check out the BMD project on my [team's github](https://github.com/Blue-Moon-Development/BMD)

Current libraries used: DirectX 11 (and D3D Compiler), BMD, Assimp, DirectXTK (for font rendering only right now), FMT, Spdlog, ImGui, StackWalker, dxerr, DirectXTex, dxguid
