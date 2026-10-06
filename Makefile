# CXX = C:/Users/HP/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT.LLVM_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin/g++.exe
# CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Iexternal/SFML/include
# LDFLAGS = -Lexternal/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

# SRC = src/main.cpp
# TARGET = bin/3D_Game_Engine.exe

# all: $(TARGET)

# $(TARGET): $(SRC)
# 	$(CXX) $(CXXFLAGS) -x c src/glad.c -x c++ src/main.cpp $(LDFLAGS) -o $(TARGET)

# run: all
# 	cd bin && 3D_Game_Engine.exe

# clean:
# 	del /Q bin\3D_Game_Engine.exe

CXX = C:/Users/HP/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT.LLVM_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin/g++.exe
# 1. Added -Iexternal/include to the flags so the compiler can find <glad/glad.h>
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Iexternal/SFML/include -Iexternal/include
LDFLAGS = -Lexternal/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

# 2. Added src/glad.c to your source files list
SRC = src/main.cpp src/glad.c
TARGET = bin/3D_Game_Engine.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -x c src/glad.c -x c++ src/main.cpp $(LDFLAGS) -o $(TARGET)

run: all
	cd bin && 3D_Game_Engine.exe

clean:
	del /Q bin\3D_Game_Engine.exe
