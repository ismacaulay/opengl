#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export _OLD_PATH=$PATH
export _OLD_PS1=$PS1

export PATH=$DIR/tools:$PATH
export PS1="(env)$PS1"

alias app=$DIR/.build/src/opengl

deactivate() {
    export PATH=$_OLD_PATH
    export PS1=$_OLD_PS1

    unset _OLD_PATH
    unset _OLD_PS1
    unset -f deactivate
    unalias app
}
