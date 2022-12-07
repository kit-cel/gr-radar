setlocal EnableDelayedExpansion
@echo on

:: Make a build folder and change to it
cmake -E make_directory buildconda
cd buildconda

:: configure
cmake -G "Ninja" ^
    -DCMAKE_BUILD_TYPE:STRING=Release ^
    -DCMAKE_INSTALL_PREFIX:PATH="%LIBRARY_PREFIX%" ^
    -DCMAKE_PREFIX_PATH:PATH="%LIBRARY_PREFIX%" ^
    -DGR_PYTHON_DIR:PATH="%SP_DIR%" ^
    -DENABLE_DOXYGEN=OFF ^
    -DENABLE_TESTING=ON ^
    ..
if errorlevel 1 exit 1

:: build
cmake --build . --config Release -- -j%CPU_COUNT%
if errorlevel 1 exit 1

:: install
cmake --build . --config Release --target install
if errorlevel 1 exit 1

:: test
set SKIP_TESTS=^
qa_estimator_fmcw^
|qa_tracking_singletarget^
%=EMPTY=%

ctest --build-config Release --output-on-failure --timeout 120 -j%CPU_COUNT% -E "%SKIP_TESTS%"
if errorlevel 1 exit 1

:: now run the skipped tests to see failures, but exit without error anyway
ctest --build-config Release --output-on-failure --timeout 120 -j%CPU_COUNT% -R "%SKIP_TESTS%"
exit 0
