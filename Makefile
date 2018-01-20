#
# Copyright (c) 2018 [n/a] info@embeddora.com All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#          notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#          notice, this list of conditions and the following disclaimer in the
#          documentation and/or other materials provided with the distribution.
#        * Neither the name of The Linux Foundation nor
#          the names of its contributors may be used to endorse or promote
#          products derived from this software without specific prior written
#          permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NON-INFRINGEMENT ARE DISCLAIMED.    IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  Abstract: a makefile for 'Post-Browser Interface #1' project

export CC=gcc

#CFLAGS=-I./src  -I./stl   -ftemplate-depth=255  -fpermissive -Wno-deprecated   -lz -lc -lm -lrt -ldl -lpthread  -lcstdc++
CFLAGS=-I./src  -I./stl   -ftemplate-depth=255  -fpermissive -Wno-deprecated 
#
# -std=c++11 -- not strictly needed, but let's have an exact standard 
# defined firmly (at least for a while)
#
# -fpermissive -- for <src/Scene> warningless compilation 
#

LDFLAGS= -lGL -lGLU -lglut      

# -g debug symbols (to explore dumped core, for instance)

# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

# -Wno-deprecated -- to disable this: /usr/include/c++/4.8/backward/backward_warning.h:32:2: warning: #warning This file includes at least one deprecated or antiquated header which may be removed without further notice at a future date. Please use a non-deprecated interface with equivalent functionality instead. For a listing of replacement headers and interfaces, consult the file backward_warning.h. To disable this warning use -Wno-deprecated. [-Wcpp]


GRBG=./src/*.o ./src/*~   src/unimp/*.o src/unimp/*~             ./*.o ./*~

OBJS=src/unimp/unimp.o        src/AseFile.o src/Camera.o  src/Engine.o src/Errors.o src/Font.o src/Frustum.o src/Input.o src/Light.o src/LightSystem.o src/Manager.o src/Material.o src/Math.o src/Mesh.o src/Object3D.o src/Physic.o src/PhysicObject.o src/RenderObject.o src/Scene.o src/Settings.o src/Texture.o src/Types.o  \
src/Timer.o   src/File.o  src/GLLight.o  src/Render.o  src/Console.o   src/Window.o   src/Redraw.o  

# probs: src/Sound.o	-- reine Windows Sache, muss man vollkommen ersetzen

CC=$(PREFIX)gcc

CPP=$(PREFIX)g++

LD=$(PREFIX)ld

all:	_main $(OBJS)

%.o: %.cpp 
	$(CPP) $(CFLAGS)  -c -o $@ $<


_main: $(OBJS)
	$(CPP) $(LDLAGS) $(OBJS) -lGL -lGLU -lglut      -lm  -lrt  -ldl -lc  -o $@

clean:
	rm $(GRBG) ; rm -r -v $(GRBGD)


