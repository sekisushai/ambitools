#!/bin/bash

echo "ambitools compilation script"
#echo "What would you like to do?"
#echo "1: compile some tools"
#echo "2: exit"
#read reply
#if [ "$reply" != 1 ]
#    then echo "Ok, select the tools you would like to compile by entering their number separated by spaces:"
#    else exit
#fi

# Tools selection
reply2=""
while [ "$reply2" != "Y" ]
do
liste2=""
    while [[ ! $liste2 =~ ^[0-9]+$ ]]
        do
            echo "Select the tools you would like to compile:"
            liste=($(ls -1 *.dsp))
            for i in $(seq ${#liste[@]})
                do 
                    echo $((i-1)) ${liste[$((i-1))]}
                done
                #echo -1 all tools
            echo "Enter the corresponding number separated by spaces"
                read -a liste2
                if ! [[ "$liste2" =~ ^[0-9]+$ ]]
                    then
                    echo "Sorry integers only"
                fi
        done
                echo "You've selected the tools:"
                for i in ${liste2[*]}
                    do 
                        echo $i ${liste[$i]}
                    done
                echo "Is that correct? (Y/N)"
                read reply2
done

# Order selection
#echo "Which Ambisonic order would you like to compile? (M<=5)"
#read M
# Compilation
#mkdir tmp
echo "Compilation starts for the selected tools"
echo "The tools are compiled from order m=1 up to maximum order Mmax allowed by each tool"

 for i in ${liste2[*]}
    do
    Mmax=$(grep "M	=	" ${liste[$i]} | grep -o '[0-9]\+') # Extract the maximum order from the code
    echo "INFO: for the tool ${liste[$i]}, the maximum possible order is Mmax=$Mmax"
    l=$(grep -n "M	=	" ${liste[$i]} | cut -d : -f 1) # Extract the line number to replace
          for m in $(seq $Mmax)
              do
              suffix="_"$m
              echo "INFO: compilation starts for the tool ${liste[$i]} at order m=$m"
                          output=${liste[$i]/.dsp/$suffix".dsp"}
                        output2=${liste[$i]/.dsp/$suffix"-2.dsp"}
                          sed "$l s/.*/M=$m;/g" ${liste[$i]} > $output
                          sed "1s/.*/declare name     \"${liste[$i]/.dsp/$suffix}\";/" $output > $output2
#                           faust2jaqt -double -osc -t 0 $prefix${liste[$i]}
                          faust2supercollider -double -t 0 $output
                          rm $output $output2
              done
     done
#cd tmp
#find -perm /a+x -type f \( ! -iname "compilation.sh" \) -exec mv {} ../bin/faust2jaqt/ \;