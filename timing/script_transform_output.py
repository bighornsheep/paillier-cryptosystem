import os
def generate_file_list(prefix):
    files = []
    message_bits = [16, 80, 256]
    mod_bits = [3072]
    ext = '.txt'

    for mod in mod_bits:
        for message in message_bits:
            files.append(prefix + '_' + str(mod) + '_' + str(message) + ext)
    
    return files


def process_files(prefix):
    output_filename = f"{prefix}_output.txt"

    with open(output_filename, 'w') as output_file:
        # List files in the current directory
        files = generate_file_list(prefix)
        
        # Process each file
        for file in files:
            with open(file, 'r') as input_file:
                # Read lines and write to the output file with the prefix from the filename
                for line in input_file:
                    if line.strip():  # Skip empty lines
                        output_file.write(f"{file.split('_')[2].rstrip('.txt')} {line}\n")

# Process files starting with 'crc'
process_files('crc')

# Process files starting with 'fdv'
process_files('fdv')