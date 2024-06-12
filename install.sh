#!/bin/bash

DIR=./visualizer
PIP=$DIR/bin/pip
PYTHON=python3

if ! command -v $PYTHON $> /dev/null; then
  PYTHON=python
fi

if ! command -v $PYTHON $> /dev/null; then
  echo "python or python3 executable can't be found. Did you install Python?"
fi

if ! python -m venv $DIR; then
  echo "Python virtual environment creation failed!"
  exit 1
fi

if ! $PIP install -r $DIR/req.txt; then
  echo "python package dependency installation failed!"
  exit 1
fi

if ! make; then
  echo "Makefile failed to compile!"
  exit 1
fi

printf "\nInstallation Success!"
