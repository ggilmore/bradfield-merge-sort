#!/usr/bin/env bash

cd "$(dirname "${BASH_SOURCE[0]}")"
set -euxo pipefail

gcc -o merge merge.c
