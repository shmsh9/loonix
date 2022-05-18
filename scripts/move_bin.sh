#!/bin/bash
set -e
shopt -s extglob
mv apps/!(*.c||*.h) image/bin
