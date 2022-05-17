#!/bin/bash

# Require one argument for env file
if [ $# -ne 1 ]; then
  echo "Requires one argument (e.g. applyenv <env_file>)"
  exit 1
fi

ENV_FILE=$1
export $(xargs <"$ENV_FILE")
