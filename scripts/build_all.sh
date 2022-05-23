#!/bin/bash
set -e
bash scripts/clean.sh
bash scripts/build_bootloader.sh
bash scripts/build_kernel.sh