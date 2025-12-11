mkdir "$2"
cd "$2" || exit
wget "$1"

chmod 777 "$(*.bin)"

pwn template "$(*.bin)" >exploit.py
echo "# Write-up -- $1" >writeup.md
mkdir workspace
printf '# Logs\n' >logs
