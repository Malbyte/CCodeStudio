#depending on how to application runs and its size, enable different optimization methods used by GCC, https://gcc.gnu.org/onlinedocs/gcc-4.5.3/gcc/Optimize-Options.html

TEST:BUILD main.o MWrapper.o GWrapper.o
	cc `pkg-config --cflags glfw3` -o Executable BUILD/main.o BUILD/GWrapper.o BUILD/MWrapper.o `pkg-config --static --libs glfw3` -lGL

main.o:Example.c
	clear
	cc `pkg-config --cflags glfw3` -I main -c Example.c -o BUILD/main.o `pkg-config --static --libs glfw3` -lGL
GWrapper.o:GWrapper/GWrapper.c
	cc `pkg-config --cflags glfw3` -I GWrapper -c GWrapper/GWrapper.c -o BUILD/GWrapper.o `pkg-config --static --libs glfw3` -lGL
MWrapper.o:MWrapper/MWrapper.c
	cc `pkg-config --cflags glfw3` -I MWrapper -c MWrapper/MWrapper.c -o BUILD/MWrapper.o `pkg-config --static --libs glfw3` -lGL

STATIC:BUILD Lib GWrapper.o MWrapper.o
	ar rcs Lib/CCode.a BUILD/GWrapper.o BUILD/MWrapper.o

BUILD:
	mkdir BUILD

CLEAN:

LIB: Lib
	#create Lib files
	make STATIC
	make ThirdPartyTools
	make HEADER
Lib:
	mkdir Lib
HEADER: Headers
	`cp GWrapper/GWrapper.h Lib/Headers`
	`cp MWrapper/MWrapper.h Lib/Headers`
Headers:
	`cd Lib && mkdir Headers`
ThirdPartyTools:THIRDPARTYTOOLS
	`cd Lib/THIRDPARTYTOOLS && git clone https://github.com/nothings/stb.git`
	cd ..
THIRDPARTYTOOLS:
	cd Lib && mkdir THIRDPARTYTOOLS
pushNOUSE:
	#auto command to let me push current project to current git branch
	#without needing to input username, password, or dealing with
	#git in the cmd
	git add .
	git commit -m "Autoupdate_Branch"
	git push
