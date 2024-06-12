#!/bin/bash

DIR=./visualizer
PIP=$DIR/bin/pip
PYTHON=python3
OS=$(uname -s)

BLACK=$(tput setaf 0)
RED=$(tput setab 1)
GREEN=$(tput setab 2)
YELLOW=$(tput setaf 3)
LIME_YELLOW=$(tput setaf 190)
POWDER_BLUE=$(tput setaf 153)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
WHITE=$(tput setaf 7)
BOLD=$(tput bold)
NORMAL=$(tput sgr0)
BLINK=$(tput blink)
REVERSE=$(tput smso)
UNDERLINE=$(tput smul)

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
  echo >&2 "Python virtual environment creation ${BOLD}${RED}FAILED!${NORMAL}"
  exit 1
fi

if ! $PIP install -r $DIR/req.txt; then
  echo >&2 "Python package dependency installation ${BOLD}${RED}FAILED!${NORMAL}"
  exit 1
fi

if [ "$OS" = "Darwin" ]; then ## stupid macos
  if ! sed -i.macos_sucks $'359i\\\t
    np.seterr(divide="ignore", invalid="ignore")' $NETGRAPH/_utils.py; then
    echo >&2 "Failed to silence warnings from Netgraph. It's ${UNDERLINE}no big deal${NORMAL} though."
  fi
else
  if ! sed -i $'360i\\\tnp.seterr(divide="ignore", invalid="ignore")' $NETGRAPH/_utils.py; then
    echo >&2 "Failed to silence warnings from Netgraph. It's ${UNDERLINE}no big deal${NORMAL} though."
  fi
fi

if ! make; then
  echo >&2 "Makefile ${BOLD}${RED}FAILED${NORMAL} to compile!"
  exit 1
fi

printf "\nInstallation ${BOLD}${GREEN}SUCCEEDED!${NORMAL}\n"

# https://stackoverflow.com/questions/5190966/using-sed-to-insert-tabs
# https://stackoverflow.com/a/8394273
# https://stackoverflow.com/a/19482947
# https://stackoverflow.com/questions/4332478/read-the-current-text-color-in-a-xterm/4332530#4332530
# https://stackoverflow.com/a/20983251
# https://stackoverflow.com/a/39325792
# https://stackoverflow.com/a/7573438
