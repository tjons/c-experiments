#!/bin/bash

rm test.txt && rm copy.txt

echo "Testing the _cp utility"

test_content="Here is some sample content we will use"

echo $test_content >> test.txt

./_cp test.txt copy.txt

actual_content=$(cat copy.txt)

if [ "$test_content" = "$actual_content" ]; then
        echo "Tests pass ✅"
        exit 0
else
        echo "Tests did not pass ❌"
        exit 1
fi

