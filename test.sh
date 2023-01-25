#!/bin/sh -e
# wrapper around ./libertytest

LD_LIBRARY_PATH=output output/libertytest "$@"
