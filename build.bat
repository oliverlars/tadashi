@echo off

set application_name=tadashi
set build_options= -DBUILD_WIN32=1
set compile_flags= -nologo /FS -W0 /Zc:preprocessor /I ../src/
set common_link_flags= -opt:ref -incremental:no /Debug:FULL /STACK:0x200000,0x200000

if not exist build mkdir build
pushd build
del *.pdb
start /b /wait "" "cl.exe"  %build_options% %compile_flags% ../src/tadashi.cc /link %common_link_flags% /out:%application_name%.exe
popd