#!/bin/sh

grep -rnw . -e "TODO" | sed '/binaire/d;$d'
