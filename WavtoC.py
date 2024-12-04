bytesInAll = []
thing = False #used to check one time things

with open("audioFiles/yahooyippe.wav", "rb") as f:
    rBytes = True
    rBytes = f.read(16)
    while rBytes:
            rBytes = f.read(1)
            print(type(rBytes))

            if rBytes != b'':
                # print(rBytes)
                bytesInAll.append(rBytes[0])
            else:
                print("pain")
    
with open("sound.h", 'w') as fout:
    print("const unsigned char sound = {", end="", file=fout)
    for b in bytesInAll:
        print(f"0x{b:02x}, ", end="", file=fout)
    print("0x00};", end="", file=fout)