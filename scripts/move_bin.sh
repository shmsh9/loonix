#!/bin/bash
set -e
shopt -s extglob
mv apps/!(*.c) image/bin
