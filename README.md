# explode
3D game
Written by [Mikołaj Ciesielski](https://github.com/PanMaczeta), [Patryk Brudz](https://github.com/DirtyDeedsDoneDirtCheapD4C) and [Robert Furman](https://github.com/furrob)

# building
Orginally built with Visual Studio 2019. Repo contains solution/project config files set in such a way that all libraries should be arranged like this:
- some directory
  - LIBS (*.rar file with LIBS directory can be found [here](https://github.com/furrob/explode/releases))
  - explode


Additionally, after compiling, place additional resources from package (*.rar can be found in [Releases tab](https://github.com/furrob/explode/releases)) in compiled exe file directory like this:
- ...
  - Debug/Release
    - *.exe
    - glew32.dll
    - models
      - (suppiled *.obj files)
    - shaders
      - (*.glsl from source directory/ from *.rar file)
    - sounds
      - (suppiled *.raw files)
    - textures
      - (suppiled *.png/ *.jpg files)

# libraries used
- [glew-2.1.0](http://glew.sourceforge.net/)
- [glm](https://glm.g-truc.net/0.9.9/index.html)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)

