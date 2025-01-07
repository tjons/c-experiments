#!/bin/bash

rm test.txt && rm copy.txt

echo "Testing the _cp utility"

test_content="Here is some sample content we will use"

echo $test_content >> test.txt

./_cp test.txt copy.txt

actual_content=$(cat copy.txt)
single_file_tests_successful=0
directory_tests_successful=0

if [ "$test_content" = "$actual_content" ]; then
        single_file_tests_successful=1
fi

mkdir _test_dir
echo $test_content >> _test_dir/test.txt

./_cp -R ./_test_dir ./_new_test_dir

actual_content=$(cat ./_new_test_dir/test.txt)
if [ "$test_content" = "$actual_content" ]; then
        directory_tests_successful=1
fi

if [ "$directory_tests_successful" = "1" ]; then
        echo "Directory test suite passed ✅"
else
        echo "Directory test suite failed ❌"
fi

if [ "$single_file_tests_successful" = "1" ]; then
        echo "Single file test suite passed ✅"
else
        echo "Single file test suite failed ❌"
fi
