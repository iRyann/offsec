mkdir "$1"
cd "$1" || exit
wget --http-user=ctf --http-password=promo2526 http://cytech.tcdn.casa:8080/"$1"

chmod 777 "$(*.bin)"

pwn template "$(*.bin)" >exploit.py
echo "# Write-up -- $1" >writeup.md
mkdir workspace
printf '# Logs\n' >logs
