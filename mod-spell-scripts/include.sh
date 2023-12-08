#!/usr/bin/env bash

MOD_SPELL_SCRIPTS_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/" && pwd )"

source $MOD_SPELL_SCRIPTS_ROOT"/conf/conf.sh.dist"

if [ -f $MOD_SPELL_SCRIPTS_ROOT"/conf/conf.sh" ]; then
    source $MOD_SPELL_SCRIPTS_ROOT"/conf/conf.sh"
fi
