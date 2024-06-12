#!/bin/bash

DIR=./visualizer
PIP=$DIR/bin/pip
PYTHON=python3
OS=$(uname -s)

if ! command -v $PYTHON &> /dev/null; then
  PYTHON=python
fi

if ! command -v $PYTHON &> /dev/null; then
  echo >&2 "python or python3 executable can't be found. Is python in the PATH?"
  exit 1
fi

VIR=$($PYTHON --version)
VIR=${VIR#Python *}
VIR=${VIR%.*}
NETGRAPH=$DIR/lib/python$VIR/site-packages/netgraph

if ! $PYTHON -m venv $DIR; then
  echo >&2 "Python virtual environment creation failed!"
  exit 1
fi

if ! $PIP install -r $DIR/req.txt; then
  echo >&2 "Python package dependency installation failed!"
  exit 1
fi

if [ "$OS" = "Darwin" ]; then ## stupid macos
  if ! sed -i.macos_sucks $'359i\\\t
    np.seterr(divide="ignore", invalid="ignore")' $NETGRAPH/_utils.py; then
    echo >&2 "Failed to silence warnings from Netgraph. It's no big deal though."
  fi
else
  if ! sed -i $'360i\\\tnp.seterr(divide="ignore", invalid="ignore")' $NETGRAPH/_utils.py; then
    echo >&2 "Failed to silence warnings from Netgraph. It's no big deal though."
  fi
fi

if ! make; then
  echo >&2 "Makefile failed to compile!"
  exit 1
fi

printf "\nInstallation Success!"

# https://stackoverflow.com/questions/5190966/using-sed-to-insert-tabs
# https://stackoverflow.com/a/8394273
# https://stackoverflow.com/a/19482947
