CXX := g++
CXXFLAGS := $(shell root-config --cflags) -fPIC

UNAME := $(shell uname)

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp, bin/%.o, $(notdir $(SRCS)) )
EXES := $(patsubst %.cpp, bin/%.exe, $(notdir $(wildcard main/*.cpp)) )

# ifeq ($(UNAME),Linux)
# 	EIGEN_INC := $(shell pkg-config --cflags eigen3 )
# 	else
# 	EIGEN_INC := -I /usr/local/Cellar/eigen/3.4.0_1/include/eigen3
# endif	

ROOT_INC := -I $(shell root-config --incdir)
ROOT_LIB := $(shell root-config --libs)

VPATH = main:src

main: $(EXES)

lib: lib/libFC.a

lib/%.a: $(OBJS)
	@echo making lib...[$^]
	ar ruv $@ $^
	ranlib $@

bin/%.exe: bin/%.o lib/libFC.a 
	@echo executable... $< [$@]
	$(CXX) $(CXXFLAGS) -o $@ $< -I src $(EIGEN_INC) $(ROOT_INC) -L lib -l FC $(ROOT_LIB)

bin/%.o: %.cpp
	@echo compiling... $< [$@]
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I src $(EIGEN_INC) $(ROOT_INC)

clean:
	@echo cleaning...
	rm -f $(wildcard bin/*) $(wildcard lib/*)

dump:
	@echo SRCS...[$(SRCS)] [$(OBJS)] [$(EXES)]
	@echo $(UNAME)
	@echo src: $(SRCS)
	@echo obj: $(OBJS)
	@echo exe: $(EXES)