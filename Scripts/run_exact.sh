#!/bin/bash

cd ..

instance_folder="Instances/Small/";
#instance_folder="Instances/Debug/";

#files="6_2_143_3_S_1-9"
files="*"
instance_extension=".dat"

#Alpha
alpha[1]=0.9
alpha[2]=0.8
alpha[3]=0.7
alpha[4]=0.6
alpha[5]=0.5
alpha[6]=0.4
alpha[7]=0.3
alpha[8]=0.2
alpha[9]=0.1

#Pega quantos alphas serao executados
size_alpha=${#alpha[@]}

#Algoritmos
algorithm[1]="EXACT"
#Pega quantos algoritmos serao executados
size_algorithm=${#algorithm[@]}

folder_solution="Solutions/2020_05_12/"

#tempo em segundos (valor sera multiplicado pela numero de tarefas da instancia)
max_time_factor[1]=600
#Pega quantos tempos serao executados
size_max_time_factor=${#max_time_factor[@]}

if ! [ "$1" == "start" ] && ! [ "$1" == "restart" ]
then
    echo "opção inválida. digite: ./run.sh (start|restart)"
    exit 1
fi


#percorre os tempos size_max_time_factor
for((j=1;j<=$size_max_time_factor;j++))
do

    max_time_factor=${max_time_factor[$j]}

    #percorre os algoritmos
    for((k=1;k<=$size_algorithm;k++))
    do

        algorithm=${algorithm[$k]}
        
        #percorre todos os arquivos instance_extension
        find $instance_folder -maxdepth 1 -name $files$instance_extension -type f -print0 | while read -d $'\0' full_path_file; do


            #percorre os alphas
            for((l=1;l<=$size_alpha;l++))
            do

                alpha=${alpha[$l]}

                filename="${full_path_file##*/}"
                instance_name="${filename%.[^.]*}"
                #echo "Nome da instância "$instance_name

                #Arquivo para salvar o logs de erros
                file_log_error="log/error/"$algorithm".log"

                #se nao existe a pasta para guardar os logs
                if ! [ -d "log/error/" ];
                then
                    #crie a pasta
                    mkdir -p "log/error/"
                    #echo "Criando pasta"
                fi

                #Arquivo para salvar o logs de situações
                file_log="log/status/"$algorithm".log"

                #se nao existe a pasta para guardar os logs
                if ! [ -d "log/status/" ];
                then
                    #crie a pasta
                    mkdir -p "log/status/"
                    #echo "Criando pasta"
                fi

                #se nao existe a pasta para guardar as solucoes do algoritmo
                if ! [ -d $folder_solution ];
                then
                    #crie a pasta
                    mkdir -p $folder_solution
                    #echo "Criando pasta"
                fi

                if [ "$1" == "start" ]
                then
                    echo "Start: $(date)"
                    echo "  instance_folder: "$instance_folder
                    echo "  instance: "$instance_name$instance_extension
                    echo "  alpha: "$alpha
                    echo "  algorithm: "$algorithm
                    echo "  max_time_factor: "$max_time_factor
                    echo "  folder_solution: "$folder_solution

                     ./build/src/src $instance_folder $instance_name $instance_extension "1" $algorithm $max_time_factor $folder_solution $alpha

                    echo "End: $(date)"
                    #date
                    echo ""

                    #Se o comando falhou salva a hora
                    if [ $? -gt 0 ];
                    then
                        date "+%d/%m/%Y %H:%M:%S "$file_solution >> $file_log_error
                    else
                        #Se nao falhou escreve em outro log
                        date "+%d/%m/%Y %H:%M:%S Arquivo "$file_solution" Salvo com sucesso." >> $file_log
                    fi
                elif [ "$1" == "restart" ]
                then
                    FileName=$(echo $file | cut -d '/' -f 7)
                    #isInFile=$(cat $fileSolution | grep -c $FileName)
                    #if [ $isInFile -eq 0 ]; then
                    #   echo "Restart"
                    #   date  ## echo the date at start
                    #   #echo "Arquivo Não executado"
                    #   echo $file

                    #   ./build/src/src $file "1" $algorithm $fileSolution $time $NVezesMax $nivelMax $alpha 2>> $fileLogError

                    #   echo "End"
                    #   date
                    #fi
                else
                    echo "opção inválida. digite: ./run.sh (start|restart)"
                fi #start
            done #Arquivos dat
        done #Sementes
    done #Algoritmos
done #tempos
