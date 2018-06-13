#!/usr/bin/env bash

workdir=$(pwd)

#Clean the previous build
rm -rf cpp-aecmio 2>/dev/null || true
rm -rf tests 2>/dev/null || true
exec &> $workdir/buildlog.txt
export aecmio_TEST_PATH="$(pwd)/tests"

#Clonning Repositories
echo "Cloning Repositories"
git clone --depth 1 --single-branch https://github.com/aecmio/tests.git
git clone --recursive --depth 1 --single-branch https://github.com/aecmio/cpp-aecmio.git
cd tests
testHead=$(git rev-parse HEAD)
cd ..
cd tests/RPCTests
npm install
cd $workdir/cpp-aecmio
cppHead=$(git rev-parse HEAD)

#Prepare test results
mkdir build
cd build
echo "Make cpp-aecmio develop:"
cmake .. -DCOVERAGE=On
make -j8
echo "Running all tests:"
echo "cpp-aecmio repository at commit $cppHead"
echo "tests repository at commit $testHead"
exec 2> $workdir/testlog.txt
timestart=$(date +%s.%N)
TMPDIR=/dev/shm
test/testaec -- --all --exectimelog
cd $workdir/tests/RPCTests
echo "#--------------RPC TESTS--------------"
node main.js $workdir/cpp-aecmio/build/aec/aec
timeend=$(date +%s.%N)
date=$(date +%Y-%m-%d)

# Upload coverage report
if [ -z "$CODECOV_TOKEN" ]; then
    echo "Warning! CODECOV_TOKEN not set. See https://codecov.io/gh/aecmio/cpp-aecmio/settings."
else
    bash <(curl -s https://codecov.io/bash) -n alltests -b "$date" -F alltests -a '>/dev/null 2>&1'
fi

# Make report
cd $workdir
(
echo "REPORT"
exectime=$(echo "$timeend - $timestart" | bc)
echo "Test execution time: $exectime s"
echo "Coverage: https://codecov.io/gh/aecmio/cpp-aecmio/commit/$cppHead"
cat testlog.txt
cat buildlog.txt
) > report.txt

# Send mail
RECIPIENTS="dimitry@aecmio.org pawel@aecmio.org chris@aecmio.org andrei@aecmio.org yoichi@aecmio.org"
mail < report.txt -s "cpp-aecmio alltests $date" $RECIPIENTS
