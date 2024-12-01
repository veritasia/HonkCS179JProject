bytesInAll = []
thing = False #used to check one time things

with open("audioFiles/yahooyippe.wav", "rb") as f:
    rBytes = True
    rBytes = f.read(16)
    while rBytes:
            rBytes = f.read(1)

            if rBytes != b'':
                print(rBytes)
                bytesInAll.append(rBytes)
            else:
                print("pain")
    
with open("sound.h", 'w') as fout:
    print("uint8_t sound = {", end="", file=fout)
    for b in bytesInAll:
        print(f"0x{b}, ", end="", file=fout)
    print("0x00};")