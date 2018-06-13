#!/usr/bin/env bash
# author: Lefteris Karapetsas <lefteris@refu.co>
#
# Just upload the generated .rb file to homebrew aecmio

echo ">>> Starting the script to upload .rb file to homebrew aecmio"
rm -rf homebrew-aecmio
git clone git@github.com:aecmio/homebrew-aecmio.git
cp webthree-umbrella/build/cpp-aecmio.rb homebrew-aecmio
cd homebrew-aecmio
git add . -u
git commit -m "update cpp-aecmio.rb"
git push origin
cd ..
rm -rf homebrew-aecmio
echo ">>> Succesfully uploaded the .rb file to homebrew aecmio"
