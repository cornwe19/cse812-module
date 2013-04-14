#!/bin/sh

fail() {
    echo "Failed in step '$1'"
    exit 1
}

echo "Building and inserting the module..."
cd module
make > /dev/null 2>&2 || fail "Module Build"
make install > /dev/null  2>&2 || fail "Module Insert"
cd - > /dev/null

echo "Making keyreader app..."
cd keyreader
make > /dev/null 2>&2 || fail "Keyreader Build"
cd - > /dev/null

