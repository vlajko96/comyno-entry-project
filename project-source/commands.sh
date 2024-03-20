#!/bin/bash
COMMAND=$1
EXECUTABLE=$2
WORKING_DIR=$PWD
BUILD_DIR_NAME="build"
BUILD_PATH="$WORKING_DIR/$BUILD_DIR_NAME"
OUTPUTS_DIR_NAME="output"
OUTPUTS_PATH="$WORKING_DIR/$OUTPUTS_DIR_NAME"
LIBRARY_OUTPUT_DIR_NAME="lib"
LIBRARY_OUTPUT_PATH="$OUTPUTS_PATH/$LIBRARY_OUTPUT_DIR_NAME"
BINARY_OUTPUT_DIR_NAME="bin"
BINARY_OUTPUT_PATH="$OUTPUTS_PATH/$BINARY_OUTPUT_DIR_NAME"
QPID_LIB_PATH="$BUILD_PATH/qpid-messaging"
PGCLIENT_LIB_PATH="$BUILD_PATH/pg-client"
TEST_BIN_PATH="$BUILD_PATH/test"
QPID_UNIT_TEST_BIN_PATH="$BUILD_PATH/qpid-messaging/unit-test"
PGCLIENT_UNIT_TEST_BIN_PATH="$BUILD_PATH/pg-client/unit-test"

function build(){
        echo "Building project"
        mkdir $BUILD_DIR_NAME
        mkdir $OUTPUTS_DIR_NAME
        cd $OUTPUTS_PATH
        mkdir $LIBRARY_OUTPUT_DIR_NAME
        mkdir $BINARY_OUTPUT_DIR_NAME
        cd $BUILD_PATH
        cmake ..
        ERROR=$?
        if [[ $ERROR -ne 0 ]]; then
            echo "ERROR value is $ERROR"
            exit $ERROR
        fi
        make VERBOSE=1
        cp "$QPID_LIB_PATH/libqpidmessagingcomyno.so" "$LIBRARY_OUTPUT_PATH"
        cp "$PGCLIENT_LIB_PATH/libpgclientcomyno.so" "$LIBRARY_OUTPUT_PATH"
        cp "$TEST_BIN_PATH/test-cm" "$BINARY_OUTPUT_PATH"
        cp "$QPID_UNIT_TEST_BIN_PATH/test-qpid" "$BINARY_OUTPUT_PATH"
        cp "$PGCLIENT_UNIT_TEST_BIN_PATH/test-pgclient" "$BINARY_OUTPUT_PATH"
        cd -
}

function clean (){
    rm -rf $BUILD_PATH
    rm -rf $OUTPUTS_PATH
}

function run_target (){
    if [[ "$1" == 'test' ]]; then
        echo "Runing tests"
        cd $BINARY_OUTPUT_PATH
        ERROR=$?
        if [[ $ERROR -ne 0 ]]; then
            echo "ERROR value is $ERROR  (try run rebuild)"
            exit $ERROR
        fi
        ./test-qpid
        ./test-pgclient
        cd -
    elif [[ "$1" == 'generate_coverage' ]]; then
        echo "Runing generating coverage report"
        cd $BUILD_PATH
        make coverage_report
        cp -r coverage_report ../output
        cd -
    else 
        echo "Wrong target to run please change input"
        exit -1;
    fi
}

function main (){
    while true; do
        if [[ "$COMMAND" == 'build' ]]; then
            build
            break;
        elif [[ "$COMMAND" == 'rebuild' ]]; then
            echo "Rebuilding project"
            clean
            build
            break;
        elif [[ "$COMMAND" == 'clean' ]]; then
            echo "Cleaning project"
            clean
            break;
        elif [[ "$COMMAND" == 'run_target' ]]; then
            echo "Runing target $EXECUTABLE"
            run_target $EXECUTABLE
            break;
        else
            echo "Wrong input please change input"
            exit -1;
        fi
    done
        echo "End of the script"
}

main
