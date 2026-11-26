#!/bin/sh
set -eu

root=$(git rev-parse --show-toplevel)
git config --local core.hooksPath "$root/.githooks"
printf 'core.hooksPath -> %s/.githooks\n' "$root"
