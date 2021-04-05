#! /bin/bash

if ! [[ -x reverse ]]; then
    echo "reverse executable does not exist"
    exit 1
fi

rm -rf tests-out
../tester/run-tests.sh $* -v -d modified-tests