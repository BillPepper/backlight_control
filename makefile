NAME=backlightctrl

all: build

build:
	gcc src/main.c -o ${NAME}

clean:
	rm ${NAME}

install:
	mv ${NAME} /usr/local/bin/
