#!/usr/bin/env bash
shopt -s lastpipe 
EXEC=../parser.out
CORR_TESTFILES=`ls *.nb | grep cor`
passed=0
failed=0
if [ -f ${EXEC} ]
then  
    echo "Checking correct programs"
    for file in ${CORR_TESTFILES}
    do
        ${EXEC} ${file} > log  2>&1
        error=`grep error log -c`
        warning=`grep warning log -c`
        if [[ ($error -eq 0) && ($warning -eq 0) ]]
        then
            echo -e "\e[32m\t [ PASSED ] \e[0m $file\n"
            passed=$(($passed+1));
        else
            echo -e "\e[31m\t [ FAILED ] \e[0m $file \t error: ${error} \t warning: $warning\n"
            cat log 
            failed=$(($failed+1))
        fi
        rm log
    done
else
    echo "Executable does not exists"
    exit 2
fi

# Checking for Error code

echo "Checking for incorrect programs"
ERROR_FILES=`ls *.nb | grep err`
for file in $ERROR_FILES
do
    ${EXEC} ${file} > log  2>&1
    expected_error=`grep ERROR_EXPECTED $file -c`
    expected_warning=`grep WARNING_EXPECTED $file -c`
    error=`grep error log -c`
    warning=`grep warning log -c`
    if [[ ($error -eq $expected_error) && ($warning -eq $expected_warning) ]]
    then
        echo -e "\e[32m\t [ PASSED ] \e[0m $file \tfound errors: ${error}/${expected_error} \tfound warnings: ${warning}/${expected_warning}\n"
        passed=$(($passed+1))
    else
        echo -e "\e[31m\t [ FAILED ] \e[0m $file \tfound errors: ${error}/${expected_error} \tfound warnings: ${warning}/${expected_warning}\n"
        failed=$(($failed+1))
    fi
    rm log
done

echo -e "\tTest summary"
echo -e "Total \t:\t$(($passed + $failed))"
echo -e "Passed \t:\t$passed"
echo -e  "Failed \t:\t$failed"

if [ $failed -ne 0 ]
then
    exit 1
fi





