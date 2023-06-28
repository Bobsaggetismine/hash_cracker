## Modern hash cracking tool


#Build instructions:
    Note: permake5 is required for all systems, please download/install that before continuing
        Windows: run `premake5.exe vs2022` in the root directory of the project, then open the .sln file in vs2022 and build accordingly
                 Note: Previous versions of VS are unlikely to work and not supported given the project reqires c++20 at a minimum.
        Linux: run `premake5 gmake` in the root directory and then run `make config=release` or `make config=debug` depending on
                                                                                                    the wanted build
