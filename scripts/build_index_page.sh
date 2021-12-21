#!/bin/bash

cd nodejs; ./node_modules/.bin/remark ../../CppCoreGuidelines.md --no-color -q --config-path ./remark/.remarkrc-index-page 1> ../build/CppCoreGuidelines.md.indexed
cd ../build
marked -o output.html < CppCoreGuidelines.md.indexed
