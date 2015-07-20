CC=g++ 
FLAGS=-Wno-deprecated
LDLIBS= -L/opt/local/lib -lglui
FRAMEWORKS=-framework OpenGL -framework GLUT
INC=-I./ -I/opt/local/include
EXE=pool

$(EXE): *.cpp
	$(CC) $(FLAGS) $(INC) *.cpp -o $@ $(LDLIBS) $(FRAMEWORKS)

clean:
	rm -rf *.o *~ $(EXE)
