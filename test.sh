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
		echo "Input:  \"${@:2}\" should be recognized as valid"
		echo
		((FAIL_COUNT++))
	fi
}

function expect_success
{
	expect_any 0 "$@"
}

function expect_failure
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
	expect_success	"0:0"
	expect_success	"0:00"
	expect_success	"0:01"
	expect_success	"0:30"
	expect_success	"1:30"
	expect_success	"10:30"
	expect_success	"100:30"
	expect_success	"0/0"
	expect_success	"0/00"
	expect_success	"0/01"
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
	expect_success	".01"
	expect_success	".02"
	expect_success	".11"
	expect_success	".12"
	expect_success	".98"
	expect_success	".99"
	expect_success	"now"
	output "Testing BAD time formats:"
	output "-------------------------"
	expect_failure	"1"
	expect_failure	"10"
	expect_failure	"100"
	expect_failure	"h"
	expect_failure	"10hour"
	expect_failure	"10hours"
	expect_failure	"10 h"
	expect_failure	"10 hour"
	expect_failure	"10 hours"
	expect_failure	"m"
	expect_failure	"10min"
	expect_failure	"10minutes"
	expect_failure	"10 m"
	expect_failure	"10 min"
	expect_failure	"10 minutes"
	expect_failure	"10/60"
	expect_failure	"/"
	expect_failure	"/0"
	expect_failure	"/30"
	expect_failure	"0/"
	expect_failure	"10/"
	expect_failure	"100/"
	expect_failure	"10/3"
	expect_failure	"10/300"
	expect_failure	"10:60"
	expect_failure	":"
	expect_failure	":0"
	expect_failure	":30"
	expect_failure	"0:"
	expect_failure	"10:"
	expect_failure	"100:"
	expect_failure	"10:3"
	expect_failure	"10:300"
	expect_failure	"."
	expect_failure	"0."
	expect_failure	"10."
	expect_failure	"100."
	expect_failure	"0.500"
	expect_failure	"1.500"
	expect_failure	"10.500"
	expect_failure	"100.500"
	expect_failure	".500"
	expect_failure	"Now"
	# Trailing characters
	expect_failure	"X"
	expect_failure	"0X"
	expect_failure	"0hX"
	expect_failure	"1hX"
	expect_failure	"10hX"
	expect_failure	"100hX"
	expect_failure	"0mX"
	expect_failure	"1mX"
	expect_failure	"10mX"
	expect_failure	"100mX"
	expect_failure	"0:00X"
	expect_failure	"0:30X"
	expect_failure	"1:30X"
	expect_failure	"10:30X"
	expect_failure	"100:30X"
	expect_failure	"0/00X"
	expect_failure	"0/30X"
	expect_failure	"1/30X"
	expect_failure	"10/30X"
	expect_failure	"100/30X"
	expect_failure	"0.0X"
	expect_failure	"0.5X"
	expect_failure	"1.5X"
	expect_failure	"10.5X"
	expect_failure	"100.5X"
	expect_failure	"0.00X"
	expect_failure	"0.50X"
	expect_failure	"1.50X"
	expect_failure	"10.50X"
	expect_failure	"100.50X"
	expect_failure	".0X"
	expect_failure	".5X"
	expect_failure	".50X"
	expect_failure	"nowX"
	# Leading characters
	expect_failure	"X0"
	expect_failure	"X0h"
	expect_failure	"X1h"
	expect_failure	"X10h"
	expect_failure	"X100h"
	expect_failure	"X0m"
	expect_failure	"X1m"
	expect_failure	"X10m"
	expect_failure	"X100m"
	expect_failure	"X0:00"
	expect_failure	"X0:30"
	expect_failure	"X1:30"
	expect_failure	"X10:30"
	expect_failure	"X100:30"
	expect_failure	"X0/00"
	expect_failure	"X0/30"
	expect_failure	"X1/30"
	expect_failure	"X10/30"
	expect_failure	"X100/30"
	expect_failure	"X0.0"
	expect_failure	"X0.5"
	expect_failure	"X1.5"
	expect_failure	"X10.5"
	expect_failure	"X100.5"
	expect_failure	"X0.00"
	expect_failure	"X0.50"
	expect_failure	"X1.50"
	expect_failure	"X10.50"
	expect_failure	"X100.50"
	expect_failure	"X.0"
	expect_failure	"X.5"
	expect_failure	"X.50"
	expect_failure	"Xnow"
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
	expect_failure	"Clear"
	expect_failure	"Help"
	expect_failure	"Exit"
	expect_failure	"quit"
	expect_failure	"Quit"
	# Trailing characters
	expect_failure	"clearX"
	expect_failure	"helpX"
	expect_failure	"exitX"
	# Leading characters
	expect_failure	"Xclear"
	expect_failure	"Xhelp"
	expect_failure	"Xexit"
	output
}

function expect_equal
{
	RESULT=$(echo $1 | ./vreme --clean | cut -c4-)
	if [[ "$RESULT" == "$2" ]]; then
		if [ "$PRINT_ALL" -eq 1 ]; then
			echo -e "[\e[32m PASS \e[39m] $1 => $2"
		fi
		((PASS_COUNT++))
	else
		echo -e "[\e[31m FAIL \e[39m] $1 => $2"
		echo "Result:  $1 => $RESULT is incorrect"
		echo
		((FAIL_COUNT++))
	fi
}

function test_now
{
	# Two controls are used because time could have changed between date
	# acquiring the time and vreme acquiring it afterwards. This test also
	# implicitly assumes that no more than 30 seconds will pass between the
	# first and the second date acquiring the time.
	CONTROL1=$(date +"%R %S")
	RESULT=$(echo now | ./vreme --clean | cut -c4-)
	CONTROL2=$(date +"%R %S")

	SECONDS1=$(echo "$CONTROL1" | cut -d' ' -f2)
	CONTROL1=$(echo "$CONTROL1" | cut -d' ' -f1)
	SECONDS2=$(echo "$CONTROL2" | cut -d' ' -f2)
	CONTROL2=$(echo "$CONTROL2" | cut -d' ' -f1)

	# Assumes adding 1 minute works in vreme
	if [ "$SECONDS1" -gt 30 ]; then
		CONTROL1=$(echo "$CONTROL1+1m" | ./vreme --clean | tail -1 | cut -c4-)
	fi
	if [ "$SECONDS2" -gt 30 ]; then
		CONTROL2=$(echo "$CONTROL2+1m" | ./vreme --clean | tail -1 | cut -c4-)
	fi

	if [[ "$RESULT" == "$CONTROL1" ]]; then
		if [ "$PRINT_ALL" -eq 1 ]; then
			echo -e "[\e[32m PASS \e[39m] now => $CONTROL1"
		fi
		((PASS_COUNT++))
	elif [[ "$RESULT" == "$CONTROL2" ]]; then
		if [ "$PRINT_ALL" -eq 1 ]; then
			echo -e "[\e[32m PASS \e[39m] now => $CONTROL2"
		fi
		((PASS_COUNT++))
	else
		echo -e "[\e[31m FAIL \e[39m] now => $CONTROL1 or $CONTROL2"
		((FAIL_COUNT++))
	fi
}

function test_values
{
	output "Testing values"
	output "--------------"
	expect_equal	"0"		"00:00"
	expect_equal	"0h"		"00:00"
	expect_equal	"1h"		"01:00"
	expect_equal	"10h"		"10:00"
	expect_equal	"100h"		"100:00"
	expect_equal	"0m"		"00:00"
	expect_equal	"1m"		"00:01"
	expect_equal	"10m"		"00:10"
	expect_equal	"100m"		"01:40"
	expect_equal	"0:0"		"00:00"
	expect_equal	"0:00"		"00:00"
	expect_equal	"0:01"		"00:01"
	expect_equal	"0:30"		"00:30"
	expect_equal	"1:30"		"01:30"
	expect_equal	"10:30"		"10:30"
	expect_equal	"100:30"	"100:30"
	expect_equal	"0/0"		"00:00"
	expect_equal	"0/00"		"00:00"
	expect_equal	"0/01"		"00:01"
	expect_equal	"0/30"		"00:30"
	expect_equal	"1/30"		"01:30"
	expect_equal	"10/30"		"10:30"
	expect_equal	"100/30"	"100:30"
	expect_equal	"0.0"		"00:00"
	expect_equal	"0.5"		"00:30"
	expect_equal	"1.5"		"01:30"
	expect_equal	"10.5"		"10:30"
	expect_equal	"100.5"		"100:30"
	expect_equal	"0.00"		"00:00"
	expect_equal	"0.50"		"00:30"
	expect_equal	"1.50"		"01:30"
	expect_equal	"10.50"		"10:30"
	expect_equal	"100.50"	"100:30"
	expect_equal	".0"		"00:00"
	expect_equal	".5"		"00:30"
	expect_equal	".50"		"00:30"
	expect_equal	".01"		"00:01"
	expect_equal	".02"		"00:01"
	expect_equal	".11"		"00:07"
	expect_equal	".12"		"00:07"
	expect_equal	".98"		"00:59"
	expect_equal	".99"		"00:59"
	test_now
	output
}

function run_tests
{
	if [[ "$1" == "--all" ]]; then
		PRINT_ALL=1
	fi

	echo "Starting testing"
	time {
		# Syntactic tests
		test_time_formats
		test_commands
		# Semantic tests
		test_values

		echo "Done"
		echo
		echo -n "Testing times:"
	}
	echo
	echo "Results:"
	echo "--------"
	echo "PASS: $PASS_COUNT"
	echo "FAIL: $FAIL_COUNT"
	if [ "$FAIL_COUNT" -ne 0 ] || [ "$PASS_COUNT" -ne 0 ]; then
		if [ "$FAIL_COUNT" -eq 0 ]; then
			PERCENT=100
		elif [ "$PASS_COUNT" -eq 0 ]; then
			PERCENT=0
		else
			PERCENT="$(( 10000*$PASS_COUNT / ($PASS_COUNT+$FAIL_COUNT) ))"
			PERCENT="${PERCENT:0:-2}.${PERCENT: -2}"
		fi

		echo "$PERCENT% passing"
	else
		echo -e "[\e[31m ERROR \e[39m] No tests run, or no results counted"
		exit 1
	fi
}

run_tests $@

if [ "$FAIL_COUNT" -eq 0 ]; then
	exit 0
else
	exit 1
fi
