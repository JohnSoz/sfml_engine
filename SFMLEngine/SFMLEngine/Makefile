CCOPTION = Wall std=gnu++11 D__USE_MINGW_ANSI_STDIO=1
FLAGS = $(addprefix -,$(CCOPTION))
CC = g++
OBJ = DebuggingSystem.o Math.o Object.o Source.o World.o 
SRC_DIR = E:\SfmlProject\SFMLEngine\SFMLEngine

output.exe: $(OBJ) 
	 $(CC) $(FLAGS) $(OBJ) -o output.exe 

DebuggingSystem.o: $(SRC_DIR)\DebuggingSystem.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\DebuggingSystem.cpp -o DebuggingSystem.o 

Math.o: $(SRC_DIR)\Math.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\Math.cpp -o Math.o 

Object.o: $(SRC_DIR)\Object.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\Object.cpp -o Object.o 

Source.o: $(SRC_DIR)\Source.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\Source.cpp -o Source.o 

World.o: $(SRC_DIR)\World.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)\World.cpp -o World.o 


clean: 
	del /Q $(OBJ_DIR)\*.o
	del /Q $(BUILD_DIR)\*.exe
