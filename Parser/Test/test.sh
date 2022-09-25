#!/usr/bin/env bash
shopt -s lastpipe 
EXEC=../parser.out
TESTFILES=`ls *.nb`
passed=0
failed=0
if [ -f ${EXEC} ]
then  
    for file in ${TESTFILES}
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
            grep error log 
            failed=$(($failed+1))
        fi
        rm log
    done
else
    echo "Executable does not exists"
fi

echo -e "\tTest summary"
echo -e "Total \t:\t$(($passed + $failed))"
echo -e "Passed \t:\t$passed"
echo -e  "Failed \t:\t$failed"

if [ $failed -ne 0 ]
then
    exit 1
fi