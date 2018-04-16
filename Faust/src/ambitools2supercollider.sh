#!/bin/bash

#ambitools2supercollider compilation script
#Pierre Lecomte (c) 2017-2018

echo "ambitools compilation script"
#echo "What would you like to do?"
#echo "1: compile some tools"
#echo "2: exit"
#read reply
#if [ "$reply" != 1 ]
#    then echo "Ok, select the tools you would like to compile by entering their number separated by spaces:"
#    else exit
#fi

#extension=".dsp" # file extension for faust code
#prefix="hoa_" # prefix to chop to correctly generate the UGen (see faust2supcol)

# Tools selection
reply2=""
while [ "$reply2" != "Y" ]
do
liste2=""
    while [[ ! $liste2 =~ ^[0-9]+$ ]]
        do
            echo "Select the tools you would like to compile:"
            #liste=($(ls -1 *$extension))
            liste=($(ls -1 | grep hoa))
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

#echo "Do you want to remove the \" Mute \" toggles for the selected tools ?"

echo "Compilation starts for the selected tools"
echo "The tools are compiled from order m=1 up to maximum order Mmax allowed by each tool"

#outdir="../bin/ambitools2supercollider/HOA/"
outdir="../../Supercollider/src/"

mkdir -p "$outdir/sc/Classes" # create the outdir folder for supercollider

 for i in ${liste2[*]}
    do
    #outputdir="$outdir/${liste[$i]%$extension}"
    #name=${liste[$i]#$prefix} # Remove prefix "hoa_" from the name, for the declare name line
    #echo "Please enter the name you would like for the UGen (current name: ${liste[$i]})"
    #read name

    case ${liste[$i]} in
    "hoa_azimuth_rotator.dsp")
    suggest="HOAAzimuthRotator"
    ;;
    "hoa_beamforming_dirac_to_hoa.dsp")
    suggest="HOABeamDirac2HOA"
    ;;
    "hoa_beamforming_hypercardioid_to_hoa.dsp")
    suggest="HOABeamHCardio2HOA"
    ;;
    "hoa_beamforming_hypercardioid_to_mono.dsp")
    suggest="HOABeamHCardio2Mono"
    ;;
    "hoa_converter_acn_n3d_to_acn_sn3d.dsp")
    suggest="HOAConverterAcnN3d2AcnSn3d"
    ;;
    "hoa_converter_acn_n3d_to_fuma.dsp")
    suggest="HOAConverterAcnN3d2FuMa"
    ;;
    "hoa_converter_acn_sn3d_to_acn_n3d.dsp")
    suggest="HOAConverterAcnSn3d2AcnN3d.dsp"
    ;;
    "hoa_converter_acn_sn3d_to_fuma.dsp")
    suggest="HOAConverterAcnSn3d2FuMa"
    ;;
    "hoa_converter_fuma_to_acn_n3d.dsp")
    suggest="HOAConverterFuma2AcnN3d"
    ;;
    "hoa_converter_fuma_to_acn_sn3d.dsp")
    suggest="HOAConverterFuma2AcnSn3d"
    ;;
    "hoa_decoder_lebedev06.dsp")
    suggest="HOADecLebedev06"
    ;;
    "hoa_decoder_lebedev26.dsp")
    suggest="HOADecLebedev26"
    ;;
    "hoa_decoder_lebedev50.dsp")
    suggest="HOADecLebedev50"
    ;;
    "hoa_decoder_lebedev50_binaural.dsp")
    suggest="HOADecBinaural"
    ;;
    "hoa_encoder.dsp")
    suggest="HOAEncoder"
    ;;
    "hoa_mic_encoder_eigenmike32.dsp")
    suggest="HOAEncEigenMike"
    ;;
    "hoa_mic_encoder_lebedev50.dsp")
    suggest="HOAEncLebedev50"
    ;;
    "hoa_mirroring.dsp")
    suggest="HOAMirror"
    ;;
    "hoa_panning_lebedev06.dsp")
    suggest="HOAPanLebedev06"
    ;;
    "hoa_panning_lebedev26.dsp")
    suggest="HOAPanLebedev26"
    ;;
    "hoa_panning_lebedev50.dsp")
    suggest="HOAPanLebedev50"
    ;;
    "hoa_rotator.dsp")
    suggest="HOARotator"
    ;;
    esac    
    
    
    #read -e -p "Please enter the name you would like for the UGen: " -i $suggest name
    name=$suggest;
    
     Mmax=$(grep "M	=	" ${liste[$i]} | grep -o '[0-9]\+') # Extract the maximum order from the code
     echo "INFO: for the tool ${liste[$i]}, the maximum possible order is Mmax=$Mmax"
     l=$(grep -n "M	=	" ${liste[$i]} | cut -d : -f 1) # Extract the line number to replace
     
     #echo "From what order you want to start the compilation? (<= $Mmax)"
     #read seqmin
     seqmin=1;
    
     sequence=$(seq $seqmin $Mmax);
     
     for m in $sequence
               do
               #suffix="_"$m
               suffix=$m
               echo "INFO: compilation starts for the tool ${liste[$i]} at order m=$m"
                           #output=${liste[$i]/.dsp/$suffix".dsp"}
                           output=$name$suffix".dsp"
                           output2=$name$suffix"_2.dsp"
                           #output2=${liste[$i]/.dsp/$suffix"_2.dsp"}
                           sed "$l s/.*/M=$m;/g" ${liste[$i]} > $output
                           #sed "1s/.*/declare name     \"${name/.dsp/$suffix}\";/" $output > $output2
                           sed "1s/.*/declare name     \"$name$suffix\";/" $output > $output2
                           sed "s/metermute/meterm/g" $output2 > $output # Replace the meter with mute toggle
                           sed "s/*ma.PI\/180//g" $output > $output2 # Replace degree with radians
                           sed 's@0, *360@-1*ma.PI, ma.PI@g' $output2 > $output # Replace azimuth 0 360 to -PI PI
                           sed 's@-90, *90@-1*ma.PI/2, ma.PI/2@g' $output > $output2 # Replace elevation -90 90 to -PI/2 PI/2
                           mv  $output2 $output
                           
                           faust2supercollider -ks -noprefix -double -t 0 -time $output # Compilation with -ks (keep source) flag : keep the .cpp sources
                                                   
                           #tmp=$(ls -d */ | grep faust) # Temp dir generated by the compilation
                           #echo $tmp
                           #mv $tmp/*/* $tmp
                           #rmdir $tmp/*
                           #mv $tmp*/*.cpp $outdir # Move .cpp file to the output directory
                           #rm -R $tmp # Remove tmp directory
                           
                           mv $name$suffix".cpp" $outdir
                           mv $name$suffix".sc" $outdir/sc/Classes # Move sc file to the output directory
                           #mv *.so $outdir/sc/Classes # Movve
                           #mv *.scx $outdir/sc/Classes
                           
                           rm *.sc *.so *.scx
                           rm $output
               done
     done
