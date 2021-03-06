SOURCES=$(wildcard *.c)
PROGRAMS=$(basename $(SOURCES))
LIBRARIES=libtga.a

all:	$(PROGRAMS) $(LIBRARIES)

%:	%.c
%.o:	%.c
	$(CC) $(CFLAGS) -std=c99 -c -o $@ $<

%:	%.o	
	$(CC) $(CFLAGS) -o $@ $< $(LIBRARIES) -lm

%.a:	tga.o
	$(AR) cr $@ $<

.PHONY:	clean
clean:	
	@rm -rf ~* $(PROGRAMS)
	@rm -rf $(LIBRARIES)
