#!/usr/bin/env bash

set -ex

cmake -E make_directory buildconda
cd buildconda

cmake_config_args=(
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_INSTALL_PREFIX=$PREFIX
    -DLIB_SUFFIX=""
    -DENABLE_DOXYGEN=OFF
    -DENABLE_TESTING=ON
)

cmake ${CMAKE_ARGS} -G "Ninja" .. "${cmake_config_args[@]}"
cmake --build . --config Release -- -j${CPU_COUNT}
cmake --build . --config Release --target install

if [[ "${CONDA_BUILD_CROSS_COMPILATION:-}" != "1" ]]; then
    SKIP_TESTS=(
        qa_estimator_fmcw
        qa_tracking_singletarget
    )
    SKIP_TESTS_STR=$( IFS="|"; echo "^(${SKIP_TESTS[*]})$" )
    ctest --build-config Release --output-on-failure --timeout 120 -j${CPU_COUNT} -E "$SKIP_TESTS_STR"
    ctest --build-config Release --output-on-failure --timeout 120 -j${CPU_COUNT} -R "$SKIP_TESTS_STR" || true
fi

# clean up fontconfig cache generated during testing so it's not in package
if [[ $target_platform == linux* ]] ; then
    rm -rf $PREFIX/var/cache
fi
