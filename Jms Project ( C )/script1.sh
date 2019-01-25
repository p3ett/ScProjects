
#!/bin/bash

if [ "$1" = "-l" ]
then
    if [ "$3" = "-c" ]
    then
        if [ "$4" = "list" ]
        then
            ls -l $2/
        elif [ "$4" = "size" ]       
        then
            cd $2
            ls -slr
        elif [ "$4" = "purge" ]
        then
            cd $2
            find . -type f -name jms\* -exec rm {} \;
        else
            echo "wrong command!"
        fi
    else
        echo "Need -c before command!"
    fi
fi
