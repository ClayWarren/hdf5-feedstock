@echo on
cmake -S "%RECIPE_DIR%\dll-reproducer" -B dll-reproducer-build -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit 1
cmake --build dll-reproducer-build --target procedure_consumer
if errorlevel 1 exit 1
dll-reproducer-build\procedure_consumer.exe
if errorlevel 1 exit 1
dumpbin /headers dll-reproducer-build\values.dll | findstr /I /C:"AA64 machine (ARM64)"
if errorlevel 1 exit 1
dumpbin /exports dll-reproducer-build\values.dll
dumpbin /linkermember:1 dll-reproducer-build\values.lib
cmake --build dll-reproducer-build --target data_consumer
if errorlevel 1 exit 1
dll-reproducer-build\data_consumer.exe
if errorlevel 1 exit 1
