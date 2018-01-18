#
# Copyright (c) 2017 TP-Link konstantin.mauch@tp-link.com All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#        * Redistributions of source code must retain the above copyright
#          notice, this list of conditions and the following disclaimer.
#        * Redistributions in binary form must reproduce the above copyright
#          notice, this list of conditions and the following disclaimer in the
#          documentation and/or other materials provided with the distribution.
#        * Neither the name of The TP-Link Company (Shenzhen, PRC) nor
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
#  Abstract: 

export CC=gcc

BUILD_CURL=

# Well, let's start to...

CFLAGS=-I./src  -I./stl  -IGL  -ftemplate-depth=255  -fpermissive 
# -std=c++11
# -fpermissive -- for <src/Scene>

LDFLAGS= -lGL -lGLU -lglut

# -g debug symbols (to explore dumped core, for instance)

# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)

GRBG=src/*.o *~   config.status config.log

#GRBG += Makefile configure

GRBGD=./autom4te.cache  ./lintstats
 
	


OBJS=src/AseFile.o src/Camera.o  src/Engine.o src/Errors.o src/Frustum.o src/Input.o src/Light.o src/LightSystem.o src/Manager.o src/Material.o src/Math.o src/Mesh.o src/Object3D.o src/Physic.o src/PhysicObject.o src/RenderObject.o src/Scene.o src/Settings.o src/Texture.o src/Types.o _main.o


# probs: src/Console.o src/File.o src/Timer.o
# probs: src/GLLight.o src/Render.o src/Sound.o src/Window.o

#src/AseFile.o src/Camera.o src/Console.o src/Engine.o src/Errors.o src/File.o src/Font.o src/Frustum.o src/GLLight.o src/Input.o src/Light.o src/LightSystem.o src/Manager.o src/Material.o src/Math.o src/Mesh.o src/Object3D.o src/Physic.o src/PhysicObject.o src/Render.o src/RenderObject.o src/Scene.o src/Settings.o src/Sound.o src/Texture.o src/Timer.o src/Types.o src/Window.o src/_main.o

CC=$(PREFIX)gcc

CPP=$(PREFIX)g++

LD=$(PREFIX)ld

all:	$(OBJS)  #_main

%.o: %.cpp 
	$(CPP) $(CFLAGS)  -c -o $@ $<

_main: $(OBJS)
	$(LD) $(LDLAGS)   $(OBJS)  -lcurl -o $@

clean:
	rm $(GRBG) ; rm -r -v $(GRBGD)
