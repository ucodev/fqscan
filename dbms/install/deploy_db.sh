#!/bin/bash

echo -ne "\n\n************ Installing MySQL Database **************\n\n"
echo -ne "MySQL Hostname: "
read host
echo -ne "MySQL Username: "
read user
echo -ne "MySQL Password: "
read -s pass
echo -ne "\nMySQL Database: "
read dbname

#echo $host
#echo $user
#echo $pass
#echo $dbname

ret=`echo "CREATE DATABASE ${dbname}" | /usr/bin/mysql -u${user} -p${pass} -h${host} 2>&1`

if [ $? -ne 0 ]; then
	if [ "`echo ${ret} | cut -d' ' -f2`" = "1007" ]; then
		echo -ne "Database exists. Drop (N/y)? "
		read ask
		if [ ${ask} = "y" ]; then
			echo "DROP DATABASE ${dbname}" | /usr/bin/mysql -u${user} -p${pass} -h${host}
			echo "CREATE DATABASE ${dbname}" | /usr/bin/mysql -u${user} -p${pass} -h${host}

			if [ $? -ne 0 ]; then
				echo "Error while installing database."
				exit 1
			fi
		else
			echo "Error while installing database."
			exit 1
		fi
	else
		echo "Error while installing database."
		exit 1
	fi

fi

/usr/bin/mysql -u${user} -p${pass} -h${host} ${dbname} < ./model/fqscan.sql

if [ $? -ne 0 ]; then
	echo "Error while installing database."
	exit 1
fi

echo "Database successfully installed."
exit 0
