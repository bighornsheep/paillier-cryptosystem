with open('crc.sh', 'w') as output_file:
    for i in range(5, 501, 5):
        output_file.write(f"./crc {i} 3072\n")

with open('fdv.sh', 'w') as output_file:
    for i in range(5, 501, 5):
        output_file.write(f"./fdv {i} 3072\n")