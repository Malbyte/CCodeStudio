#depending on how to application runs and its size, enable different optimization methods used by GCC, https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/Optimize-Options.html
#Main:BUILD Lib BUILD/GWrapper.o
#	ar rcs Lib/GWrapper.a BUILD/GWrapper.o

TEST:BUILD main.o GWrapper.o
	cc `pkg-config --cflags glfw3` -o Executable BUILD/main.o BUILD/GWrapper.o `pkg-config --static --libs glfw3` -lGL

main.o:Example.c
	clear
	cc `pkg-config --cflags glfw3` -I main -c Example.c -o BUILD/main.o `pkg-config --static --libs glfw3` -lGL
GWrapper.o:GWrapper/GWrapper.c
	cc `pkg-config --cflags glfw3` -I GWrapper -c GWrapper/GWrapper.c -o BUILD/GWrapper.o `pkg-config --static --libs glfw3` -lGL
BUILD:
	mkdir BUILD

CLEAN:

Lib:
	mkdir Lib

pushNOUSE:
	#auto command to let me push current project to current git branch
	#without needing to input username, password, or dealing with
	#git in the cmd
	git add .
	git commit -m "Autoupdate_Branch"
	git push
