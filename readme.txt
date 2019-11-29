-------------------------------------------------------------------------------
--- Visual Studio 2013/15/17 Template Project for C++/DirectX11 Application ---
--- Marc Treib, Florian Ferstl, Sept 2013 -------------------------------------
--- Updated: Florian Ferstl, Sept 2014 ----------------------------------------
--- Updated: Mina Saad Aziz, May 2016 -----------------------------------------
--- Updated: Mengyu Chu, Nov 2017    ------------------------------------------
--- Updated: You Xie, Nov 2019    ---------------------------------------------
-------------------------------------------------------------------------------

This solution contains the following components:

 - AntTweakBar 1.16 
   "AntTweakBar is a small and easy-to-use C/C++ library that allows program-
   mers to quickly add a light and intuitive graphical user interface into 
   graphic applications based on OpenGL (compatibility and core profiles), 
   DirectX 9, DirectX 10 or DirectX 11 to interactively tweak parameters 
   on-screen."
   Homepage: http://anttweakbar.sourceforge.net/

 - DirectX Tool Kit, version "September 22, 2017" 
   (the 'MakeSpriteFont' project is part of the DirectXTK)
   "The DirectX Tool Kit (aka DirectXTK) is a collection of helper classes for
   writing Direct3D 11 code in C++."
   Homepage: https://github.com/Microsoft/DirectXTK
 
 - DXUT, version "March 10, 2017 (11.15)"
   "DXUT is a "GLUT"-like framework for Direct3D 11.x Win32 desktop 
   applications; primarily samples, demos, and prototypes."
   Homepage: https://github.com/Microsoft/DXUT

 - Effects 11 (FX11), version "March 10, 2017 (11.19)"
   Homepage: https://github.com/Microsoft/FX11

 - Main Project, Simulations:
   > "main.cpp": Structured like a typical DXUT-based application. It contains
     example code that correctly integrates and demonstrates how to use 
	 AntTweakBar (for a simple GUI) and DirectXTK (for simple, "fixed-function"
	 rendering of common primitives).
   > "effect.fx": Starting point for custom shaders, already loaded in 
     main.cpp. If  you don't need custom shaders, you can safely ignore or even
	 delete it (and any code related to "g_pEffect" in main.cpp)
	 
Further Note:

 - DirectXMath (all functions/types starting with XM*): DirectXTK and the new 
   DXUT are both based on the new DirectXMath API for linear algebra that comes
   with the Windows 8.* SDKs (replacing the old D3DXMath). You can use it for
   all of your linear algebra tasks.
   Documentation: https://msdn.microsoft.com/en-us/library/windows/desktop/hh437833(v=vs.85).aspx
