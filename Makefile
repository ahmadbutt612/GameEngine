CXX = C:/Users/HP/AppData/Local/Microsoft/WinGet/Packages/BrechtSanders.WinLibs.POSIX.UCRT.LLVM_Microsoft.Winget.Source_8wekyb3d8bbwe/mingw64/bin/g++.exe
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Iexternal/SFML/include
LDFLAGS = -Lexternal/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

SRC = src/main.cpp
TARGET = bin/3D_Game_Engine.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

run: all
	cd bin && 3D_Game_Engine.exe

clean:
	del /Q bin\3D_Game_Engine.exe
