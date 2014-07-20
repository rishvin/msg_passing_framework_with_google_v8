#/bin/bash
export PYTHONPATH=$PYTHONPATH:$PWD
./cleanup.py all
./framework.py
if [[ $? = 0 ]]; then
    ./modules.py
fi
