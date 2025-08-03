@echo off

set "current_dir=%cd%"

cd ../../
cmake -B ./build/example -DCMAKE_BUILD_TYPE=Release -DKBDT_BUILD_EXAMPLE=ON
make -C ./build/example -j

cd %current_dir%
