#!/bin/bash
clear

ldd $1 > /tmp/tmp1   

nfound=`cat /tmp/tmp1|grep "not found"`  #проверяем все ли либы найдены

if  [ "$nfound" == "" ] 
then

a=a


else
	echo "Не все библиотеки найдены. Настройте переменные окружения (export LD_LIBRARY_PATH=/path). Нажимите любую клавишу для продолжения"
echo $nfound
read v

fi


	sed -e 's/ /\
/g' /tmp/tmp1 |grep "/" |sort|uniq 2>&1 >>/tmp/tmp2

	# заменяем все пробелы переводом строки, выделяем те, где есть путь, сортируем и на всякий удаляем дубликаты.

	rm -f /tmp/tmp1

	echo "Существующий каталог lib_collect будет удален. Нажимите любую клавишу для продолжения"
	read b

	rm -fr lib_collect
	mkdir lib_collect

	file="/tmp/tmp2"
	while read line
	do
	cp $line lib_collect/
	done < $file
	rm -f tmp2


echo "Либы скопированы в каталог lib_collect"






read b
#echo $text
