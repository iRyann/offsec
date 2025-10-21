import sys
import os.path

def printQWORD(data : bytearray):
    buffer = "\x1b[0;31m00\x1b[0;0m > "
    address = 0
    counter = 0
    for byte in data:
        buffer += "\x1b[0;34m" * ((counter+1)%2) +  ("0" * (byte < 16)) + str(hex(byte))[2:] + "\x1b[0;0m"*((counter+1)%2) + " "
        counter += 1
        if counter == 8:
            buffer += " " * 4
        if counter == 16:
            address += 16
            buffer += "\n\x1b[0;31m" + str(hex(address))[2:] + "\x1b[0;0m > "
            counter = 0
    print(buffer)

def main(argv):
    filename = argv[1]
    with open(filename,"rb") as f:
        content = bytearray(f.read())
    content[0x3e] = 0x00
    printQWORD(content)
    with open(filename,"wb") as f:
        f.write(content)

if __name__ == "__main__":
    main(sys.argv)