@echo off

set application_name=tadashi
set build_options= -DBUILD_WIN32=1
set compile_flags= -nologo /FS /W0 /Zi /FC /Zc:preprocessor /I ../src/
set common_link_flags= -opt:ref -incremental:no /Debug:FULL

if not exist build mkdir build
pushd build
del *.pdb
start /b /wait "" "cl.exe"  %build_options% %compile_flags% ../src/tadashi.cc /link %common_link_flags% /out:%application_name%.exe
popd