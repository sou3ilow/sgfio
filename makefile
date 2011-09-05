
# macro definition for program behavior
CONFIG =

# test 
TEST = sgf -ply ./sample/13.sgf

# compile option
OPTIMAL_FLAGS = -DNDEBUG -O3
DEBUG_FLAGS = -g

# default program name 'make name=hoge' changes the name of program
name = sgf

# add needed source files here
SOURCE = \
		main.cpp

FILES = \
	$(SORCE) \
	Const.h \
	DiskBoard.h \
	Sgf.h \
	SgfCommand.h \
	String.h \
	StringBoard.h \
	console.h \
	option.h \
	types.h \
	util.h

STRING_TABLE_CHARSET=--input-charset=cp932 --exec-charset=cp932
HEADER = $(wildcard *.h)
CC = g++
LIBS=
WORK = Work
OBJECT = $(patsubst %.cpp,$(WORK)/%.obj,$(SOURCE))
DEPEND = $(patsubst %.cpp,$(WORK)/%.dep,$(SOURCE))

# create program with optimal option (default)
.PHONY: optimal
optimal: FLAGS = $(CONFIG) $(OPTIMAL_FLAGS)
optimal: $(name)

# create program with debug option
.PHONY: debug
debug: FLAGS = $(CONFIG) $(DEBUG_FLAGS)
debug: $(name)

.PHONY: clean
clean:
	rm -rf $(WORK)
	rm -f $(name)

clean-windows:
	rm -f $(name).exe
	rm -f *.suo 

tags: $(HEADER) $(SOURCE)
	ctags $(HEADER) $(SOURCE)


# create the program
$(name): $(DEPEND) $(OBJECT)
	$(CC) $(FLAGS) -o $(name) $(LIBS) $(OBJECT)

# create object files
$(WORK)/%.obj: %.cpp makefile
	@mkdir -p $(WORK)
	$(CC) $(FLAGS)  -c $< -o $@

test:
	$(TEST)

doc: $(FILES)
	doxygen $(FILES)

# create dependencies
$(WORK)/%.dep: %.cpp makefile
	@mkdir -p $(WORK)
	@set -e; $(CC) -MM $(FLAGS) $< \
		| sed 's=\(.*\)\.o[ :]*=$(WORK)/\1.obj : $@ =g' > $@;
-include $(DEPEND)

