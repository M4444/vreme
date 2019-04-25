#!/bin/bash

PASS_COUNT=0
FAIL_COUNT=0

PRINT_ALL=0

#============================== Test arrays ===================================#

declare -a GOOD_TIME_FORMATS=(
	"0"
	"0h" "1h" "10h" "100h"
	"0m" "1m" "10m" "100m"
	"0:0" "0:00" "0:01" "0:30" "1:30" "10:30" "100:30"
	"0/0" "0/00" "0/01" "0/30" "1/30" "10/30" "100/30"
	      "0.0"  "0.1"  "0.5"  "1.5"  "10.5"  "100.5"
	      "0.00" "0.10" "0.50" "1.50" "10.50" "100.50"
	".0" ".5" ".50" ".01" ".02" ".11" ".12" ".98" ".99"
	"now"
)

declare -a GOOD_TIME_VALUES=(
	"00:00"
	"00:00" "01:00" "10:00" "100:00"
	"00:00" "00:01" "00:10" "01:40"
	"00:00" "00:00" "00:01" "00:30" "01:30" "10:30" "100:30"
	"00:00" "00:00" "00:01" "00:30" "01:30" "10:30" "100:30"
	        "00:00" "00:06" "00:30" "01:30" "10:30" "100:30"
	        "00:00" "00:06" "00:30" "01:30" "10:30" "100:30"
	"00:00" "00:30" "00:30" "00:01" "00:01" "00:07" "00:07" "00:59" "00:59"
)

declare -a BAD_TIME_FORMATS=(
	"1" "10" "100"
	"h" "10hour" "10hours" "10 h" "10 hour" "10 hours"
	"m" "10min" "10minutes" "10 m" "10 min" "10 minutes"
	"10/60" "/" "/0" "/30" "0/" "10/" "100/" "10/3" "10/300"
	"10:60" ":" ":0" ":30" "0:" "10:" "100:" "10:3" "10:300"
	"." "0." "10." "100." "0.500" "1.500" "10.500" "100.500" ".500"
	"Now"
)

declare -a GOOD_COMMAND_FORMATS=(
	"clear" "help" "exit"
)

declare -a BAD_COMMAND_FORMATS=(
	"Clear" "Help" "Exit"
	"quit" "Quit"
)

#============================== Helper functions ==============================#
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

# $1 = input
# $2 = expected output
# $3 = result of the calculation
function expect_equal
{
	if [[ "$3" == "$2" ]]; then
		if [ "$PRINT_ALL" -eq 1 ]; then
			echo -e "[\e[32m PASS \e[39m] $1 => $2"
		fi
		((PASS_COUNT++))
	else
		echo -e "[\e[31m FAIL \e[39m] $1 => $2"
		echo "Result:  $1 => $3 is incorrect"
		echo
		((FAIL_COUNT++))
	fi
}

#============================== Testing functions =============================#
function test_time_formats
{
	output "Testing GOOD time formats:"
	output "--------------------------"
	for time_format in "${GOOD_TIME_FORMATS[@]}"; do
		expect_success "$time_format"
	done
	output "Testing BAD time formats:"
	output "-------------------------"
	for time_format in "${BAD_TIME_FORMATS[@]}"; do
		expect_failure "$time_format"
	done
	# Trailing characters
	for time_format in "${GOOD_TIME_FORMATS[@]}"; do
		expect_failure "${time_format}X"
	done
	# Leading characters
	for time_format in "${GOOD_TIME_FORMATS[@]}"; do
		expect_failure "X${time_format}"
	done
	output
}

function test_commands
{
	output "Testing GOOD commands:"
	output "----------------------"
	for command_format in "${GOOD_COMMAND_FORMATS[@]}"; do
		expect_success "$command_format"
	done
	output "Testing BAD commands:"
	output "---------------------"
	for command_format in "${BAD_COMMAND_FORMATS[@]}"; do
		expect_failure "$command_format"
	done
	# Trailing characters
	for command_format in "${GOOD_COMMAND_FORMATS[@]}"; do
		expect_failure "${command_format}X"
	done
	# Leading characters
	for command_format in "${GOOD_COMMAND_FORMATS[@]}"; do
		expect_failure "X${command_format}"
	done
	output
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
	for (( i = 0; i < "${#GOOD_TIME_FORMATS[@]}"; i++ )); do
		if [[ ${GOOD_TIME_FORMATS[i]} != "now" ]]; then
			RESULT=$(echo "${GOOD_TIME_FORMATS[i]}" | ./vreme --clean | cut -c4-)
			expect_equal "${GOOD_TIME_FORMATS[i]}" "${GOOD_TIME_VALUES[i]}" "$RESULT"
		else
			test_now
		fi
	done
	output
}

#============================== Main test runner ==============================#
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
