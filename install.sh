#!/bin/bash

DIR=./visualizer
PIP=$DIR/bin/pip
PYTHON=python3

if ! command -v $PYTHON &> /dev/null; then
  PYTHON=python
fi

if ! command -v $PYTHON &> /dev/null; then
  echo >&2 "python or python3 executable can't be found. Did you install Python?"
  exit 1
fi

if ! python -m venv $DIR; then
  echo >&2 "Python virtual environment creation failed!"
  exit 1
fi

if ! $PIP install -r $DIR/req.txt; then
  echo >&2 "python package dependency installation failed!"
  exit 1
fi

if ! make; then
  echo >&2 "Makefile failed to compile!"
  exit 1
fi

printf "\nInstallation Success!"
