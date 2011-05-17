#!/bin/bash
#verifica que tem o número certo de argumentos
if [ $# != 2 ]; then
	echo "Numero de argumentos errado!"
	echo "Uso:"
	echo "    ./copia.sh <projeto-original> <projeto-novo>"
	echo ""
	exit
fi

cp -r $1 $2
cd $2
test -e *.layout && rm *.layout
test -e *.depend && rm *.depend
mv $1.cbp $2.cbp

#pega o numero dos projetos
n1=$(echo "$1" | sed -e "s/\..*//")
n2=$(echo "$2" | sed -e "s/\..*//")

#troca o nome do projeto no arquivo
sed -i -e "s/$1/$2/g" $2.cbp
#troca o nome do arquivo de saida
sed -i -e "s/bin\\\\$n1/bin\\\\$n2/g" $2.cbp
cd ..

#adiciona o projeto no workspace
projeto="<Project filename=\"$2/$2.cbp\" active=\"1\" />"
sed -i "s:active=\"1\" ::" glsl.workspace
sed -i "s:</Workspace>:\t$projeto\n\t&:" glsl.workspace

