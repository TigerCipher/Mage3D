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
6. To be continued...