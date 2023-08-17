#!/bin/bash
#
# Copyright (c) 2020-present Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Require one argument for env file
if [ $# -ne 1 ]; then
  echo "Requires one argument (e.g. applyenv <env_file>)"
  exit 1
fi

ENV_FILE=$1

export $(cat "$ENV_FILE" | xargs)
