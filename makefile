export CC := gcc
export CFLAGS := --std=c99
export LDLIBS := -lm

run: app
	./$^

app: App/app.c sobel.o
	${CC} ${CFLAGS} $^ -I"Include" -o $@ ${LDLIBS}

sobel.o: Src/sobel.c Include/sobel.h
	${CC} ${CFLAGS} -c Src/sobel.c -I"Include" -o $@ ${LDLIBS}

clean:
	rm app sobel.o

