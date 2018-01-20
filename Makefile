#
# Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
#  Abstract: a makefile for 'Post-Browser Interface #1' project


CCFLAGS=-I./src  -I./stl 
 
CPPFLAGS=-I./src  -I./stl  -fpermissive -Wno-deprecated 
#
# -Wno-deprecated -- 
#
# -fpermissive -- for <src/Scene> warningless compilation 
#
# -g debug symbols (to explore dumped core, for instance)
#
# -fprofile-arcs -ftest-coverage  (to collect code coverage statistics)
#
# -Wno-deprecated -- to disable this: /usr/include/c++/4.8/backward/backward_warning.h:32:2: warning: #warning This file includes at least one deprecated or antiquated header which may be removed without further notice at a future date. Please use a non-deprecated interface with equivalent functionality instead. For a listing of replacement headers and interfaces, consult the file backward_warning.h. To disable this warning use -Wno-deprecated. [-Wcpp]

LDFLAGS=-lGL -lGLU -lglut

OBJS=src/unimp/unimp.o        src/AseFile.o src/Camera.o  src/Engine.o src/Errors.o src/Font.o src/Frustum.o src/Input.o src/Light.o src/LightSystem.o src/Manager.o src/Material.o src/Math.o src/Mesh.o src/Object3D.o src/Physic.o src/PhysicObject.o src/RenderObject.o src/Scene.o src/Settings.o src/Texture.o src/Types.o  \
src/Timer.o   src/File.o  src/GLLight.o  src/Render.o  src/Console.o   src/Window.o   src/Quaternion.o   src/Redraw.o  
# probs: src/Sound.o	-- reine Windows Sache, muss man vollkommen ersetzen

# Not cross-compiling till instance for <Linux/i585> platform is ready
PREFIX=

CC=$(PREFIX)gcc

CPP=$(PREFIX)g++

LD=$(PREFIX)ld

TARGET=_main

all:	$(OBJS) $(TARGET)

%.o: %.cpp 
	$(CPP) $(CPPFLAGS)  -c -o $@ $<

%.o: %.c
	$(CC) $(CCFLAGS)  -c -o $@ $<


_main: $(OBJS)
	$(CPP)  $(OBJS)       -o $@  $(LDFLAGS)

GRBG=./src/*.o ./src/*~   src/unimp/*.o src/unimp/*~       $(TARGET)          ./*.o ./*~

clean:
	rm $(GRBG) ; rm -r -v $(GRBGD)


