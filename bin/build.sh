#!/usr/bin/env bash
# Pulled from https://github.com/urob/zmk-config/, with some minor modifications

if ! command -v yq &> /dev/null; then
    echo "Did not find 'yq', it is required for this script to function."
    exit 1
fi

if ! command -v jq &> /dev/null; then
    echo "Did not find 'jq', it is required for this script to function."
    exit 1
fi


# Parse input arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        # needed when user isn't in docker group
        -s|--su)
            SUDO="sudo"
            ;;

        -m|--multithread)
            MULTITHREAD="true"
            ;;

        -c|--clear-cache)
            CLEAR_CACHE="true"
            ;;

        # comma or space separated list of boards (use quotes if space separated)
        # if ommitted, will compile list of boards in build.yaml
        -b|--board)
            shift
            IFS=',' read -ra BOARDS <<< "$1"
            ;;

        -s|--shield)
            shift
            IFS=',' read -ra SHIELDS <<< "$1"
            ;;

        -v|--version)
            ZEPHYR_VERSION="$2"
            shift
            ;;

        -o|--output-dir)
            OUTPUT_DIR="$2"
            shift
            ;;

        --log-dir)
            LOG_DIR="$2"
            shift
            ;;

        --host-config-dir)
            HOST_CONFIG_DIR="$2"
            shift
            ;;

        --host-zmk-dir)
            HOST_ZMK_DIR="$2"
            shift
            ;;

        --docker-config-dir)
            DOCKER_CONFIG_DIR="$2"
            shift
            ;;

        --docker-zmk-dir)
            DOCKER_ZMK_DIR="$2"
            shift
            ;;

        --)
            WEST_OPTS="${@:2}"
            break
            ;;

        *)
            echo "Unknown option $1"
            exit 1
            ;;

    esac
    shift
done

# Set defaults
[[ -z $ZEPHYR_VERSION ]] && ZEPHYR_VERSION="3.2"

[[ -z $OUTPUT_DIR ]] && OUTPUT_DIR="$HOME/Downloads"
[[ -z $LOG_DIR ]] && LOG_DIR="/tmp"

[[ -z $HOST_ZMK_DIR ]] && HOST_ZMK_DIR="$HOME/_code/zmk"
[[ -z $HOST_CONFIG_DIR ]] && HOST_CONFIG_DIR="$HOME/_code/zmk-config"

[[ -z $DOCKER_ZMK_DIR ]] && DOCKER_ZMK_DIR="/workspace/zmk"
[[ -z $DOCKER_CONFIG_DIR ]] && DOCKER_CONFIG_DIR="/workspace/zmk-config"

[[ -z $CLEAR_CACHE ]] && CLEAR_CACHE="false"

DOCKER_IMG="zmkfirmware/zmk-dev-arm:$ZEPHYR_VERSION"
DOCKER_BIN="docker"

query=".include[]"
if [ ${#BOARDS[@]} -gt 0 ]; then
    board_filters=$(printf "\"%s\"," "${BOARDS[@]}")
    board_filters=${board_filters%,} # Remove the trailing comma
    query+=" | select(.board | IN($board_filters))"
fi
if [ ${#SHIELDS[@]} -gt 0 ]; then
    shield_filters=$(printf "\"%s\"," "${SHIELDS[@]}")
    shield_filters=${shield_filters%,} # Remove the trailing comma
    query+=" | select(.shield? | IN($shield_filters))"
fi
to_build=$(yq -e "$query" "$HOST_CONFIG_DIR/build.yaml")

# +--------------------+
# | BUILD THE FIRMWARE |
# +--------------------+

echo "Build mode: docker"
# DOCKER_CMD="$DOCKER_BIN run --name zmk-$ZEPHYR_VERSION --rm \
DOCKER_CMD="$DOCKER_BIN run --rm \
    --mount type=bind,source=$HOST_ZMK_DIR,target=$DOCKER_ZMK_DIR \
    --mount type=bind,source=$HOST_CONFIG_DIR,target=$DOCKER_CONFIG_DIR,readonly \
    --mount type=volume,source=zmk-root-user-$ZEPHYR_VERSION,target=/root \
    --mount type=volume,source=zmk-zephyr-$ZEPHYR_VERSION,target=$DOCKER_ZMK_DIR/zephyr \
    --mount type=volume,source=zmk-zephyr-modules-$ZEPHYR_VERSION,target=$DOCKER_ZMK_DIR/modules \
    --mount type=volume,source=zmk-zephyr-tools-$ZEPHYR_VERSION,target=$DOCKER_ZMK_DIR/tools"

# Reset volumes
if [[ $CLEAR_CACHE = true ]]
then
    $DOCKER_BIN volume rm $($DOCKER_BIN volume ls -q | grep "^zmk-.*-$ZEPHYR_VERSION$")
fi

# Update west if needed
OLD_WEST="/root/west.yml.old"
$DOCKER_CMD -w "$DOCKER_ZMK_DIR" "$DOCKER_IMG" /bin/bash -c " \
    if [[ ! -f .west/config ]]; then west init -l app/; fi \
    && if [[ -f $OLD_WEST ]]; then md5_old=\$(md5sum $OLD_WEST | cut -d' ' -f1); fi \
    && [[ \$md5_old != \$(md5sum app/west.yml | cut -d' ' -f1) ]] \
    && west update \
    && cp app/west.yml $OLD_WEST"

# Build parameters
DOCKER_PREFIX="$DOCKER_CMD -w $DOCKER_ZMK_DIR/app $DOCKER_IMG"
SUFFIX="${ZEPHYR_VERSION}_docker"
CONFIG_DIR="$DOCKER_CONFIG_DIR/config"

# usage: compile_board board
compile_board () {
    board=$(echo "$1" | jq -r '.board')
    shield=$(echo "$1" | jq -r '.shield // ""')
    [[ -z $shield ]] && shield_opts="" || shield_opts="-DSHIELD=${shield}"

    BUILD_DIR="${board}_${shield}_$SUFFIX"
    LOGFILE="$LOG_DIR/zmk_build_$board_$shield.log"
    [[ $MULTITHREAD = "true" ]] || echo -en "\n$(tput setaf 2)Building $1... $(tput sgr0)"
    [[ $MULTITHREAD = "true" ]] && echo -e "$(tput setaf 2)Building $1... $(tput sgr0)"
    $DOCKER_PREFIX west build -d "build/$BUILD_DIR" -b $board $WEST_OPTS \
        -- -DZMK_CONFIG="$CONFIG_DIR" $shield_opts  -Wno-dev > "$LOGFILE" 2>&1
    if [[ $? -eq 0 ]]
    then
        [[ $MULTITHREAD = "true" ]] || echo "$(tput setaf 2)done$(tput sgr0)"
        echo "Build log saved to \"$LOGFILE\"."
        if [[ -f $HOST_ZMK_DIR/app/build/$BUILD_DIR/zephyr/zmk.uf2 ]]
        then
            TYPE="uf2"
        else
            TYPE="bin"
        fi
        OUTPUT="$OUTPUT_DIR/$board-$shield-zmk.$TYPE"
        [[ -f $OUTPUT ]] && [[ ! -L $OUTPUT ]] && mv "$OUTPUT" "$OUTPUT.bak"
        cp "$HOST_ZMK_DIR/app/build/$BUILD_DIR/zephyr/zmk.$TYPE" "$OUTPUT"
    else
        echo
        cat "$LOGFILE"
        echo "$(tput setaf 1)Error: $1 failed$(tput sgr0)"
    fi
}

cd "$HOST_ZMK_DIR/app"
if [[ $MULTITHREAD = "true" ]]; then
    i=1
    echo "$to_build" | jq -c '.' | while read -r line; do
        compile_board $line &
        eval "T${i}=\${!}"
        eval "B${i}=\$line"  # Store the board name in a corresponding variable
        ((i++))
    done

    echo "Starting $(($i - 1)) background threads:"
    for ((x=1; x<i; x++))
    do
        pid="T$x"
        wait "${!pid}"
        board="B$x"  # Retrieve the board name from the corresponding variable
        echo -e "$(tput setaf 3)Thread $x with PID ${!pid} has finished: ${!board}$(tput sgr0)"
    done
else
    echo "$to_build" | jq -c '.' | while read -r line; do
        compile_board $line
    done
fi
