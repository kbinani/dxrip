#!/bin/bash

directory=$(cd $(dirname $0); pwd)

rm $directory/report/*.xml

(
  cd $directory
  for test_module in $(find -type f | grep '/test_[^/]*\.py$' | grep -v '__pycache__' | sed 's:^\./::g' | tr '/' '.' | sed 's:\.py$::g'); do
    test_module="test.$test_module"
    (cd ../ && python -m $test_module)
  done
)