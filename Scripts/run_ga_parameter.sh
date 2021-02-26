#!/bin/bash

cd ..

instance_folder="Instances/LARGE/";
#instance_folder="Instances/Debug/";

#files="I_50_10_S_1-9_1"
files="*"
instance_extension=".dat"

#Sementes
seed[1]=60543
#seed[2]=16086
#seed[3]=60048
#seed[4]=10217
#seed[5]=37451
#seed[6]=7451
#seed[7]=9330
#seed[8]=55717
#seed[9]=19025
#seed[10]=40822
#seed[11]=423
#seed[12]=6241
#seed[13]=26170
#seed[14]=45433
#seed[15]=27367
#seed[16]=56890
#seed[17]=54006
#seed[18]=10667
#seed[19]=61471
#seed[20]=43978
#seed[21]=42123
#seed[22]=9653
#seed[23]=55074
#seed[24]=50396
#seed[25]=44145
#seed[26]=21001
#seed[27]=34238
#seed[28]=16792
#seed[29]=31167
#seed[30]=19162

#Pega quantas sementes serao executadas
size_seed=${#seed[@]}

#Algoritmos
algorithm[1]="GA"
#Pega quantos algoritmos serao executados
size_algorithm=${#algorithm[@]}

folder_solution="Solutions/Parameter/2020_06_02_14_00/"

#tempo em milisegundos (valor sera multiplicado pela numero de tarefas da instancia)
max_time_factor[1]=3000
max_time_factor[2]=4000
max_time_factor[3]=5000

#Pega quantos tempos serao executados
size_max_time_factor=${#max_time_factor[@]}

#Tamanho da população
tam_population[1]=80
tam_population[2]=100
tam_population[3]=120
tam_population[4]=140

#Pega quantos tamanhos serão executados
size_tam_population=${#tam_population[@]}

prob_mutation[1]=0.3
prob_mutation[2]=0.5
prob_mutation[3]=0.7

#Pega quantos probabilidades de mutação que serão executados
size_prob_mutation=${#prob_mutation[@]}


if ! [ "$1" == "start" ] && ! [ "$1" == "restart" ]
then
    echo "opção inválida. digite: ./run.sh (start|restart)"
    exit 1
fi




#percorre os algoritmos
for((i=1;i<=$size_algorithm;i++))
do

    algorithm=${algorithm[$i]}

    #percorre todos os arquivos instance_extension
    find $instance_folder -maxdepth 1 -name $files$instance_extension -type f -print0 | while read -d $'\0' full_path_file; do

        filename="${full_path_file##*/}"
        instance_name="${filename%.[^.]*}"
        #echo "Nome da instância "$instance_name

        #percorre as sementes
        for((j=1;j<=$size_seed;j++))
        do

            seed=${seed[$j]}

            #percorre os fatores de tempo
            for((k=1;k<=$size_max_time_factor;k++))
            do

                max_time_factor=${max_time_factor[$k]}

                #percorre os tamanhos de população
                for((l=1;l<=$size_tam_population;l++))
                do

                    tam_population=${tam_population[$l]}

                    #percorre as probabilidades de mutação
                    for((m=1;m<=$size_prob_mutation;m++))
                    do

                        prob_mutation=${prob_mutation[$m]}

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
                            echo "  instance: "$instance_name
                            echo "  extensao: "$instance_extension
                            echo "  seed: "$seed
                            echo "  algorithm: "$algorithm
                            echo "  max_time_factor: "$max_time_factor
                            echo "  folder_solution: "$folder_solution

                            ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $max_time_factor $folder_solution "1" $tam_population $prob_mutation

                            #Se o comando falhou salva a hora
                            if [ $? -gt 0 ];
                            then
                                date "+%d/%m/%Y %H:%M:%S "$file_solution >> $file_log_error
                            else
                                #Se nao falhou escreve em outro log
                                date "+%d/%m/%Y %H:%M:%S Arquivo "$file_solution" Salvo com sucesso." >> $file_log
                            fi

                            echo "End: $(date)"
                            #date
                            echo ""
                        elif [ "$1" == "restart" ]
                        then
                            FileName=$(echo $file | cut -d '/' -f 7)
                            #isInFile=$(cat $fileSolution | grep -c $FileName)
                            #if [ $isInFile -eq 0 ]; then
                            #   echo "Restart"
                            #   date  ## echo the date at start
                            #   #echo "Arquivo Não executado"
                            #   echo $file

                            #   ./build/src/src $file $seed $algorithm $fileSolution $time $NVezesMax $nivelMax 2>> $fileLogError

                            #   echo "End"
                            #   date
                            #fi
                        else
                            echo "opção inválida. digite: ./run.sh (start|restart)"
                        fi #start
                    done #Probabilidade de mutação
                done #Tamanho da população
            done #Fator tempo
        done #Sementes
    done #Instâncias
done #Algoritmos
