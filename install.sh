#!/bin/bash

DIR=./visualizer
PIP=$DIR/bin/pip

if ! (python -m venv $DIR); then
  echo "Python virtual environment creation failed!"
  exit 1
fi

if ! ($PIP install -r $DIR/req.txt); then
  echo "python package dependency installation failed!"
  exit 1
fi

if ! make; then
  echo "Makefile failed to compile!"
  exit 1
fi

printf "\nInstallation Success!"
