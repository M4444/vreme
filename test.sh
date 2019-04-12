#!/bin/bash

PASS_COUNT=0
FAIL_COUNT=0

PRINT_ALL=0

function output
{
	if [ "$PRINT_ALL" -eq 1 ]; then
		echo $@
	fi
}

# $1 = expectation
# ${@:2} = input
function expect_any
{
	echo -e "${@:2}" | ./vreme &> /dev/null
	if [ "$?" -eq "$1" ]; then
		if [ "$PRINT_ALL" -eq 1 ]; then
			echo -e "[\e[32m PASS \e[39m] ${@:2}"
		fi
		((PASS_COUNT++))
	else
		echo -e "[\e[31m FAIL \e[39m] ${@:2}"
		((FAIL_COUNT++))
	fi
}

function expect_success
{
	expect_any 0 "$@"
}

function expect_fail
{
	expect_any 1 "$@"
}

function test_time_formats
{
	output "Testing GOOD time formats:"
	output "--------------------------"
	expect_success	""
	expect_success	"0"
	expect_success	"0h"
	expect_success	"1h"
	expect_success	"10h"
	expect_success	"100h"
	expect_success	"0m"
	expect_success	"1m"
	expect_success	"10m"
	expect_success	"100m"
	expect_success	"0:00"
	expect_success	"0:30"
	expect_success	"1:30"
	expect_success	"10:30"
	expect_success	"100:30"
	expect_success	"0/00"
	expect_success	"0/30"
	expect_success	"1/30"
	expect_success	"10/30"
	expect_success	"100/30"
	expect_success	"0.0"
	expect_success	"0.5"
	expect_success	"1.5"
	expect_success	"10.5"
	expect_success	"100.5"
	expect_success	"0.00"
	expect_success	"0.50"
	expect_success	"1.50"
	expect_success	"10.50"
	expect_success	"100.50"
	expect_success	".0"
	expect_success	".5"
	expect_success	".50"
	expect_success	"now"
	output "Testing BAD time formats:"
	output "-------------------------"
	expect_fail	"1"
	expect_fail	"10"
	expect_fail	"100"
	expect_fail	"h"
	expect_fail	"10hour"
	expect_fail	"10hours"
	expect_fail	"10 h"
	expect_fail	"10 hour"
	expect_fail	"10 hours"
	expect_fail	"m"
	expect_fail	"10min"
	expect_fail	"10minutes"
	expect_fail	"10 m"
	expect_fail	"10 min"
	expect_fail	"10 minutes"
	expect_fail	"10/60"
	expect_fail	"/"
	expect_fail	"/0"
	expect_fail	"/30"
	expect_fail	"0/"
	expect_fail	"10/"
	expect_fail	"100/"
	expect_fail	"10/3"
	expect_fail	"10/300"
	expect_fail	"10:60"
	expect_fail	":"
	expect_fail	":0"
	expect_fail	":30"
	expect_fail	"0:"
	expect_fail	"10:"
	expect_fail	"100:"
	expect_fail	"10:3"
	expect_fail	"10:300"
	expect_fail	"."
	expect_fail	"0."
	expect_fail	"10."
	expect_fail	"100."
	expect_fail	"0.500"
	expect_fail	"1.500"
	expect_fail	"10.500"
	expect_fail	"100.500"
	expect_fail	".500"
	expect_fail	"Now"
	output
}

function test_commands
{
	output "Testing GOOD commands:"
	output "----------------------"
	expect_success	"clear"
	expect_success	"help"
	expect_success	"exit"
	output "Testing BAD commands:"
	output "---------------------"
	expect_fail	"Clear"
	expect_fail	"Help"
	expect_fail	"Exit"
	expect_fail	"quit"
	expect_fail	"Quit"
	output
}

function run_tests
{
	if [[ "$1" == "--all" ]]; then
		PRINT_ALL=1
	fi
	test_time_formats
	test_commands

	echo "Results:"
	echo "--------"
	echo "PASS: $PASS_COUNT"
	echo "FAIL: $FAIL_COUNT"
	if [ "$FAIL_COUNT" -eq 0 ]; then
		PERCENT=100
	elif [ "$PASS_COUNT" -eq 0 ]; then
		PERCENT=0
	else
		PERCENT="$(( 10000*$PASS_COUNT / ($PASS_COUNT+$FAIL_COUNT) ))"
		PERCENT="${PERCENT:0:-2}.${PERCENT: -2}"
	fi
	echo "$PERCENT% passing"
}

run_tests $@

if [ "$FAIL_COUNT" -eq 0 ]; then
	exit 0
else
	exit 1
fi
