wget http://ctf:promo2526@cytech.tcdn.casa:8080/c/$1/main.c
wget http://ctf:promo2526@cytech.tcdn.casa:8080/c/$1/$1
wget http://ctf:promo2526@cytech.tcdn.casa:8080/c/$1/README.txt

chmod 777 $1

PORT=$(cat README.txt | grep a:)
PORT=${PORT#*:}
pwn template --host=cytech.tcdn.casa --port=$PORT $1 >exploit.py
echo "# Write-up -- $1" >writeup.md
