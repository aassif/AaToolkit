VER=            0.0.1
GPP=            g++ -fPIC -ggdb3 -W -Wall -pedantic -O3

H_TOOLKIT=          include
S_TOOLKIT=          src
D_TOOLKIT=          doc
I_TOOLKIT=          -I$(H_TOOLKIT)

#P_GLEW=         $(HOME)/lib/glew
#I_GLEW=         -I$(P_GLEW)/include

#P_FFMPEG=		$(HOME)/3rdPartyLibs/ffmpeg
#I_FFMPEG=		-I$(P_FFMPEG) #-I(P_FFMPEG)/libavutil -I$(P_FFMPEG)/libavformat -I$(P_FFMPEG)/libavcodec
#L_FFMPEG=		-L$(P_FFMPEG)/libavformat -lavformat -L$(P_FFMPEG)/libavcodec -lavcodec -L$(P_FFMPEG)/libavutil -lavutil -lbz2 -lz

# Règles #

HDR=            $(wildcard $(H_TOOLKIT)/*.h $(H_TOOLKIT)/*.hh)
SRC=            $(wildcard $(S_TOOLKIT)/*.cc)
OBJ=            $(SRC:%.cc=%.o)
ALL=            make.depend lib/libAaToolkit.a lib/libAaToolkit.so

all:            $(ALL)

lib/libAaToolkit.a:   $(OBJ)
								ar rcs lib/libAaToolkit.a $(OBJ)

lib/libAaToolkit.so:  make.depend $(OBJ)
								$(GPP) -shared $(OBJ) -o lib/libAaToolkit.so

#src/AaFFmpeg.o:            src/AaFFmpeg.cc
#								g++ -W -Wall -O3 -c $(I_TOOLKIT) $(I_FFMPEG) $(I_GLEW) src/AaFFmpeg.cc -o src/AaFFmpeg.o
#								g++ -ggdb3 -o0 -c $(I_TOOLKIT) $(I_FFMPEG) $(I_GLEW) src/AaFFmpeg.cc -o src/AaFFmpeg.o

%.o:            %.cc
								$(GPP) -c $(I_TOOLKIT) $< -o $@

# Ménage #

doxygen:                
								rm -f $(D_TOOLKIT)/*
								doxygen doxygen.cfg

clean:
								rm -f src/*.o
								rm -f doc/*
								rm -f $(ALL)
								find . -name "*~" -exec rm {} \;

archive:        clean
								tar zcvf ~/tgz/AaToolkit-$(VER).tgz *

# Dépendances #

dep:
								$(GPP) -MM $(I_TOOLKIT) $(I_GLEW) $(SRC) >make.depend

make.depend:    $(HDR) $(SRC)
								$(GPP) -MM $(I_TOOLKIT) $(I_GLEW) $(SRC) >make.depend

include make.depend

