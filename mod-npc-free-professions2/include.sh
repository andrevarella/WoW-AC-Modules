#!/usr/bin/env bash

MOD_PROFESSION_NPC2_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_PROFESSION_NPC2L_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_PROFESSION_NPC2_ROOT"/conf/conf.sh" ]; then
    source $MOD_PROFESSION_NPC2_ROOT"/conf/conf.sh"
fi
